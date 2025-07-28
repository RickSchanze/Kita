using System.Collections.Concurrent;
using System.Security.Cryptography;
using Newtonsoft.Json;

namespace HeaderTool;

public class FileSelectedResult
{
    public Cache OldCache = new();
    public Cache NewCache = new();
}

public class GeneratedFileInfo
{
    public string Path;
    public string GeneratedHeaderFilePath;
    public string GeneratedSourceFilePath;
}

public class FileSelector
{
    private Config _khtConfig;

    public FileSelector(Config khtConfig)
    {
        _khtConfig = khtConfig;
    }

    public static string CalculateFileHash(string filePath)
    {
        using var stream = File.OpenRead(filePath);
        byte[] hashBytes = SHA256.Create().ComputeHash(stream);
        return BitConverter.ToString(hashBytes).Replace("-", "").ToLower();
    }

    public FileSelectedResult SelectFilesToProcess(string cachePath)
    {
        FileSelectedResult fileSelectedResult = new FileSelectedResult();
        Cache cache = new();
        try
        {
            if (File.Exists(cachePath))
            {
                cache = JsonConvert.DeserializeObject<Cache>(File.ReadAllText(cachePath))!;
            }
        }
        catch (Exception e)
        {
            cache = new Cache();
        }

        fileSelectedResult.OldCache = cache;

        List<string> allCMakeFiles = Directory.GetFiles(Path.Combine(_khtConfig.ProjectPath, "Engine"),
            "CMakeLists.txt",
            SearchOption.AllDirectories).ToList();
        Cache newCache = new();
        // 寻找CMake包含的所有文件
        ConcurrentDictionary<string, ProjectCache> projects = new();
        List<Task> outerTasks = new();
        foreach (var cmakePath in allCMakeFiles)
        {
            outerTasks.Add(Task.Run(() =>
            {
                ProjectCache cmakeProject = new()
                {
                    ProjectName = new DirectoryInfo(Path.GetDirectoryName(cmakePath)!).Name
                };
                string cmakeContent = File.ReadAllText(cmakePath);
                ConcurrentQueue<FileCache> fileCacheQueue = new();
                List<Task> innerTasks = new();
                foreach (var word in cmakeContent.Split())
                {
                    innerTasks.Add(Task.Run(() =>
                    {
                        if (word.EndsWith(".h"))
                        {
                            FileCache fileCache = new()
                            {
                                Path = Path.Combine(Path.GetDirectoryName(cmakePath)!, word)
                            };
                            fileCache.Hash = CalculateFileHash(fileCache.Path);
                            fileCacheQueue.Enqueue(fileCache);
                        }
                    }));
                    Task.WaitAll(innerTasks.ToArray());
                }

                cmakeProject.Files = fileCacheQueue.ToList();
                projects.TryAdd(cmakeProject.ProjectName, cmakeProject);
            }));
        }

        Task.WaitAll(outerTasks.ToArray());
        newCache.Projects = projects.ToDictionary();
        fileSelectedResult.NewCache = newCache;
        return fileSelectedResult;
    }

    public List<string> FindFilesToDelete(FileSelectedResult result, string outputPath)
    {
        List<string> allFilesToDelete = new List<string>();
        foreach (var (projectName, oldProjectCache) in result.OldCache.Projects)
        {
            if (!result.NewCache.Projects.TryGetValue(projectName, out var newProjectCache)) continue;
            // 找出老的有新的没有的
            foreach (var oldFileCache in oldProjectCache.Files)
            {
                if (newProjectCache.Files.Find((cache => cache.Path == oldFileCache.Path)) != null) continue;
                string oldGeneratedHeader =
                    Path.Combine(outputPath, projectName, Path.GetFileName(oldFileCache.Path).Replace(".h", "generated.h"));
                string oldGeneratedSource =
                    Path.Combine(outputPath, projectName, Path.GetFileName(oldFileCache.Path).Replace(".h", "generated.cpp"));
                allFilesToDelete.Add(oldGeneratedHeader);
                allFilesToDelete.Add(oldGeneratedSource);
            }
        }

        return allFilesToDelete;
    }

    public Dictionary<string, GeneratedFileInfo> FindAllFilesToProcess(FileSelectedResult result, string outputPath)
    {
        Dictionary<string, GeneratedFileInfo> allFilesToProcess = new();
        foreach (var (newProjectName, newProjectCache) in result.NewCache.Projects)
        {
            if (!result.OldCache.Projects.TryGetValue(newProjectName, out var oldProjectCache))
            {
                foreach (var file in newProjectCache.Files)
                {
                    GeneratedFileInfo info = new GeneratedFileInfo()
                    {
                        Path = file.Path,
                        GeneratedHeaderFilePath = Path.Combine(outputPath, newProjectName,
                            Path.GetFileName(file.Path).Replace(".h", ".generated.h")),
                        GeneratedSourceFilePath = Path.Combine(outputPath, newProjectName,
                            Path.GetFileName(file.Path).Replace(".h", ".generated.cpp"))
                    };
                    allFilesToProcess.Add(info.Path, info);
                }
                continue;
            }

            foreach (var newFileCache in newProjectCache.Files)
            {
                FileCache? oldFileCache = oldProjectCache.Files.Find((cache => cache.Path == newFileCache.Path));
                if (oldFileCache != null && oldFileCache.Hash == newFileCache.Hash) continue;
                GeneratedFileInfo newFileInfo = new GeneratedFileInfo
                {
                    Path = newFileCache.Path,
                    GeneratedHeaderFilePath = Path.Combine(outputPath, newProjectName,
                        Path.GetFileName(newFileCache.Path)).Replace(".h", ".generated.h"),
                    GeneratedSourceFilePath = Path.Combine(outputPath, newProjectName,
                        Path.GetFileName(newFileCache.Path)).Replace(".h", ".generated.cpp"),
                };
                allFilesToProcess.TryAdd(newFileCache.Path, newFileInfo);
            }
        }

        return allFilesToProcess;
    }
}