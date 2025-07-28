// See https://aka.ms/new-console-template for more information

using System.Diagnostics;
using HeaderTool;
using Newtonsoft.Json;

class Program
{
    static int Main(string[] args)
    {
        if (args.Length != 1)
        {
            Console.Error.WriteLine("第一个输入参数为生成文件输出路径");
            return -1;
        }

        string outputPath = args[0];
        if (!Directory.Exists(outputPath))
        {
            Directory.CreateDirectory(outputPath);
        }

        string configPath = Path.Combine(outputPath, "Config.json");
        Config config = new();
        try
        {
            if (!File.Exists(configPath))
            {
                File.WriteAllText(configPath, JsonConvert.SerializeObject(config));
            }
            else
            {
                string content = File.ReadAllText(configPath);
                config = JsonConvert.DeserializeObject<Config>(content)!;
            }
        }
        catch (Exception ex)
        {
            File.WriteAllText(configPath, JsonConvert.SerializeObject(config));
        }

        Stopwatch stopwatch = new();
        stopwatch.Start();
        string outputFilePaths = args[0];
        FileSelector fileSelector = new FileSelector(config);
        string cachePath = Path.Combine(outputPath, "Cache.json");
        var fileSelected = fileSelector.SelectFilesToProcess(cachePath);
        var filesToDelete = fileSelector.FindFilesToDelete(fileSelected, outputFilePaths);
        foreach (var fileToDelete in filesToDelete)
        {
            if (File.Exists(fileToDelete))
            {
                File.Delete(fileToDelete);
            }
        }
        var filesToProcess = fileSelector.FindAllFilesToProcess(fileSelected, outputFilePaths);
        stopwatch.Stop();
        Console.WriteLine("finding files use time: " + stopwatch.Elapsed);
        stopwatch.Reset();
        stopwatch.Start();
        var uniformedFilesToProcess = new Dictionary<string, GeneratedFileInfo>();
        foreach (var file in filesToProcess)
        {
            uniformedFilesToProcess.Add(file.Key.Replace("\\", "/"), file.Value);
        }
        bool success = CodeGenerator.Generate(uniformedFilesToProcess, config);
        stopwatch.Stop();
        Console.WriteLine("generating files use time:" + stopwatch.Elapsed);
        if (success)
        {
            File.WriteAllText(cachePath, JsonConvert.SerializeObject(fileSelected.NewCache));
            return 0;
        }
        else
        {
            return -1;
        }
    }
}