#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <unistd.h>
#include <limits.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>

void build_vulkan_path_env(const char *relative_dir, const char *appdir, const char *env_var_name) {
    char full_dir_path[PATH_MAX];
    snprintf(full_dir_path, sizeof(full_dir_path), "%s/%s", appdir, relative_dir);

    DIR *dir = opendir(full_dir_path);
    if (!dir) {
        fprintf(stderr, "Warning: Could not open directory %s for %s: %s\n",
                full_dir_path, env_var_name, strerror(errno));
        return;
    }

    struct dirent *entry;
    char path_buffer[8192] = {0};
    int first = 1;

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type != DT_REG && entry->d_type != DT_LNK) continue;

        char file_path[PATH_MAX];
        snprintf(file_path, sizeof(file_path), "%s/%s", full_dir_path, entry->d_name);

        char resolved_path[PATH_MAX];
        if (realpath(file_path, resolved_path)) {
            if (!first) {
                strncat(path_buffer, ":", sizeof(path_buffer) - strlen(path_buffer) - 1);
            }
            strncat(path_buffer, resolved_path, sizeof(path_buffer) - strlen(path_buffer) - 1);
            first = 0;
        }
    }
    closedir(dir);

    if (strlen(path_buffer) > 0) {
        setenv(env_var_name, path_buffer, 1);
    }
}

int main(int argc, char *argv[]) {
    char appdir_buf[PATH_MAX];
    strncpy(appdir_buf, argv[0], PATH_MAX - 1);
    appdir_buf[PATH_MAX - 1] = '\0';

    char *appdir = dirname(appdir_buf);

    // --- Resolve LD_LIBRARY_PATH ---
    char ld_library_path_temp[PATH_MAX];
    snprintf(ld_library_path_temp, sizeof(ld_library_path_temp), "%s/../lib/x86_64-linux-gnu", appdir);
    char ld_library_path[PATH_MAX];
    if (!realpath(ld_library_path_temp, ld_library_path)) {
        perror("realpath for LD_LIBRARY_PATH failed");
        return 1;
    }

    // --- Resolve FONTCONFIG_PATH ---
    char fontconfig_path_temp[PATH_MAX];
    snprintf(fontconfig_path_temp, sizeof(fontconfig_path_temp), "%s/../etc/fonts", appdir);
    char fontconfig_path[PATH_MAX];
    if (!realpath(fontconfig_path_temp, fontconfig_path)) {
        perror("realpath for FONTCONFIG_PATH failed");
        return 1;
    }

    // --- Set Vulkan-related environment variables ---
    build_vulkan_path_env("../usr/share/vulkan/icd.d", appdir, "VK_DRIVER_FILES");
    build_vulkan_path_env("../usr/share/vulkan/explicit_layer.d", appdir, "VK_LAYER_PATH");
    build_vulkan_path_env("../usr/share/vulkan/implicit_layer.d", appdir, "VK_IMPLICIT_LAYER_PATH");

    // --- Set base environment variables ---
    setenv("LD_LIBRARY_PATH", ld_library_path, 1);
    setenv("FONTCONFIG_PATH", fontconfig_path, 1);

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
