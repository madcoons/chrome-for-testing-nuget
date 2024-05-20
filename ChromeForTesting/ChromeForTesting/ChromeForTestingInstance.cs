using System.Runtime.InteropServices;

namespace ChromeForTesting;

public partial class ChromeForTestingInstance
{
    private enum OS
    {
        Linux,
        MacOS,
        Windows
    }

    public static string ChromePath
        => CurrentPlatform switch
        {
            (OS.Linux, _) => Path.Join([
                AbsoluteRootPath,
                "chrome-root",
                "chrome",
                "chrome-entry",
            ]),
            (OS.MacOS, _) => Path.Join([
                AbsoluteRootPath,
                $"chrome-{ChromePlatform}",
                "Google Chrome for Testing.app",
                "Contents",
                "MacOS",
                "Google Chrome for Testing",
            ]),
            _ => throw new($"Unsupported platform '{CurrentPlatform}'")
        };

    private static string AbsoluteRootPath
        => Path.Join(AppDomain.CurrentDomain.BaseDirectory, "chrome-for-testing-" + NUGET_PACKAGE_VERSION);

    private static string ChromePlatform
        => CurrentPlatform switch
        {
            (OS.Linux, Architecture.X64) => "linux64",
            (OS.MacOS, Architecture.X64) => "mac-x64",
            (OS.MacOS, Architecture.Arm64) => "mac-arm64",
            (OS.Windows, Architecture.X86) => "win32",
            (OS.Windows, Architecture.X64) => "win64",
            _ => throw new($"Unsupported platform '{CurrentPlatform}'.")
        };

    private static (OS OS, Architecture Architecture) CurrentPlatform
        => (GetPlatform(), RuntimeInformation.OSArchitecture);

    private static OS GetPlatform()
    {
        if (OperatingSystem.IsLinux())
        {
            return OS.Linux;
        }

        if (OperatingSystem.IsMacOS())
        {
            return OS.MacOS;
        }

        if (OperatingSystem.IsWindows())
        {
            return OS.Windows;
        }

        throw new($"Unsupported operating system '{RuntimeInformation.OSDescription}'.");
    }
}