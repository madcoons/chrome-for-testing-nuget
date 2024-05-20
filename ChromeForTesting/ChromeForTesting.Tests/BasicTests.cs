using CliWrap;
using CliWrap.Buffered;

namespace ChromeForTesting.Tests;

public class BasicTests
{
    [Test]
    public async Task Test_Get_Version_Works_Async()
    {
        BufferedCommandResult res = await Cli.Wrap(ChromeForTestingInstance.ChromePath)
            .WithArguments("--version")
            .WithValidation(CommandResultValidation.None)
            .ExecuteBufferedAsync();

        Assert.That(res.ExitCode, Is.EqualTo(0));
        Assert.That(res.StandardOutput, Is.Not.Empty);
    }
}