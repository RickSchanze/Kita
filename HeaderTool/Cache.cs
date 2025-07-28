namespace HeaderTool;

public class FileCache
{
    public string Path;
    public string Hash;
}

public class ProjectCache
{
    public string ProjectName;

    public string GetGeneratedFilesPath(string generatedOutputPath)
    {
        return Path.Combine(generatedOutputPath, ProjectName);
    }

    public List<FileCache> Files = new();
}

public class Cache
{
    public Dictionary<string, ProjectCache> Projects = new();
}