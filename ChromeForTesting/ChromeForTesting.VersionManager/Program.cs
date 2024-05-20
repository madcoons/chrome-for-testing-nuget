using System.Net.Http.Json;
using System.Text.Json.Nodes;

string versionIncrementVariable = Environment.GetEnvironmentVariable("VERSION_INCREMENT") ?? "1";

if (versionIncrementVariable.Contains('0'))
{
    throw new("VERSION_INCREMENT can not container zeros.");
}

int versionIncrement = int.Parse(versionIncrementVariable);

using HttpClient client = new();
client.DefaultRequestHeaders.Add("User-Agent", "DotNet-HttpClient");

JsonObject lastKnownGoodVersions =
    await client.GetFromJsonAsync<JsonObject>(
        "https://googlechromelabs.github.io/chrome-for-testing/last-known-good-versions.json")
    ?? throw new("Failed to get last known good versions.");

string stableVersion = lastKnownGoodVersions["channels"]?["Stable"]?["version"]?.ToString()
                       ?? throw new("Stable version not found.");

Version chromeVersion = Version.Parse(stableVersion);

await SetOutputAsync("CHROME_VERSION", chromeVersion.ToString());

int versionIncrementNumOfDigits = Convert.ToInt32(Math.Floor(Math.Log10(versionIncrement))) + 1;
int suffixNumOfDigits = versionIncrementNumOfDigits + 1;
int revisionMultiplier = Convert.ToInt32(Math.Pow(10, suffixNumOfDigits));

Version nugetPackageVersion = new(
    chromeVersion.Major,
    chromeVersion.Minor,
    chromeVersion.Build,
    chromeVersion.Revision == -1 ? versionIncrement : chromeVersion.Revision * revisionMultiplier + versionIncrement
);

await SetOutputAsync("NUGET_PACKAGE_VERSION", nugetPackageVersion.ToString());

async Task SetOutputAsync(string key, string value)
{
    Console.WriteLine($"Setting output {key}={value}");
    if (Environment.GetEnvironmentVariable("GITHUB_OUTPUT") is not { } githubOutputFile)
    {
        return;
    }

    await File.AppendAllLinesAsync(githubOutputFile, [$"{key}={value}"]);
}