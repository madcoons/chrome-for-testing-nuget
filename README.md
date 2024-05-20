# Portable Chrome for Testing
This is portable chrome for testing browser binary distributed as nuget package that doesn't require any installation on system for deployment.

It works on:
- [x] MacOS arm64
- [x] MacOS x64
- [x] Linux x64
- [x] Windows x86
- [x] Windows x64

Example:
```cs
using CliWrap;
using CliWrap.Buffered;
using ChromeForTesting;

BufferedCommandResult res = await Cli.Wrap(ChromeForTestingInstance.ChromePath)
            .WithArguments("--version")
            .WithValidation(CommandResultValidation.None)
            .ExecuteBufferedAsync();

Console.WriteLine($"Exit code: {res.ExitCode}");
Console.WriteLine($"Output: {res.StandardOutput}");
```
