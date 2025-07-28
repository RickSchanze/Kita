namespace HeaderTool;

public class Config
{
    public string ProjectPath { get; set; } = "C:\\Users\\kita\\Documents\\Projects\\Kita";
    public List<string> ExtraIncludePath;
    public Config()
    {
        ExtraIncludePath =
        [
            Path.Combine(ProjectPath, @"cmake-build-debug\vcpkg_installed\x64-windows\include")
        ];
    }
}