#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    char *appdir = dirname(argv[0]);

    // Construct paths
    char ld_library_path[1024];
    snprintf(ld_library_path, sizeof(ld_library_path), "%s/../lib/x86_64-linux-gnu", appdir);

    // char fontconfig_sysroot[1024];
    // snprintf(fontconfig_sysroot, sizeof(fontconfig_sysroot), "%s/../", appdir);

    char fontconfig_path[1024];
    snprintf(fontconfig_path, sizeof(fontconfig_path), "%s/../etc/fonts", appdir);

    // char fontconfig_file[1024];
    // snprintf(fontconfig_file, sizeof(fontconfig_file), "%s/../etc/fonts/fonts.conf", appdir);

    // Set environment variables
    setenv("LD_LIBRARY_PATH", ld_library_path, 1);
    // setenv("FONTCONFIG_SYSROOT", fontconfig_sysroot, 1);
    setenv("FONTCONFIG_PATH", fontconfig_path, 1);
    // setenv("FONTCONFIG_FILE", fontconfig_file, 1);

    // Change to the application directory
    chdir(appdir);

    // Prepare arguments for execv
    char *chrome_path = "./chrome";
    char *new_argv[argc + 1];
    new_argv[0] = chrome_path;
    for (int i = 1; i < argc; i++) {
        new_argv[i] = argv[i];
    }
    new_argv[argc] = NULL;

    execv(chrome_path, new_argv);

    perror("execv failed.");
    return 1;
}
