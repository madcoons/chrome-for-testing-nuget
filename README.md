# Portable Chrome for Testing
This is portable chrome for testing browser binary distributed as nuget package that doesn't require any installation on system for deployment.

![test workflow](https://github.com/madcoons/chrome-for-testing-nuget/actions/workflows/test.yaml/badge.svg) [![NuGet version](https://img.shields.io/nuget/v/ChromeForTesting.svg)](https://www.nuget.org/packages/ChromeForTesting)

Nuget version semantic is X.X.X.X0Y where:
- X is [Chrome for Testing Version](https://googlechromelabs.github.io/chrome-for-testing)
- Original version is followed by 0
- Y is non-zero suffix on last version part

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

Notes:
- GPU on Linux is tested only using Vulkan
    - For AMD GPU copy `chrome-root/usr/share/libdrm/amdgpu.ids` to `/usr/share/libdrm/amdgpu.ids` unitil [this PR](https://gitlab.freedesktop.org/mesa/libdrm/-/merge_requests/273) is merged, released, deployed, ...