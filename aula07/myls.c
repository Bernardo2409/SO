#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>
#include <getopt.h>

static int has_extension(const char *name, const char *ext) {
    if (ext == NULL) return 1; // no filter
    const char *dot = strrchr(name, .);
    if (!dot || dot == name) return 0; // no dot or leading dot only
    return strcmp(dot + 1, ext) == 0;
}

static void list_dir(const char *dirpath, int only_files, int only_dirs, const char *ext) {
    DIR *dir = opendir(dirpath);
    if (!dir) {
        perror(dirpath);
        return;
    }

    struct dirent *entry;
    char path[PATH_MAX];

    while ((entry = readdir(dir)) != NULL) {
        // Skip "." and ".."
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        // Build full path
        int n = snprintf(path, sizeof(path), "%s/%s", dirpath, entry->d_name);
        if (n < 0 || (size_t)n >= sizeof(path)) {
            fprintf(stderr, "Path too long: %s/%s\n", dirpath, entry->d_name);
            continue;
        }

        struct stat st;
        if (lstat(path, &st) != 0) {
            perror(path);
            continue;
        }

        int is_dir = S_ISDIR(st.st_mode);
        int is_reg = S_ISREG(st.st_mode);

        // Decide if this entry should be listed
        int ok = 0;
        if (is_dir) {
            if (only_files) {
                ok = 0;
            } else if (only_dirs) {
                ok = 1; // -d lists directories
            } else {
                ok = 1; // default lists everything
            }
        } else if (is_reg) {
            if (only_dirs) {
                ok = 0;
            } else {
                // files allowed (either -f or default) but need to match extension if provided
                if (has_extension(entry->d_name, ext)) ok = 1;
            }
        } else {
            // other types (symlinks, devices)... include only if neither -f nor -d (default)
            if (!only_files && !only_dirs) ok = 1;
        }

        if (ok) {
            printf("%s\n", entry->d_name);
        }
    }

    closedir(dir);
}

static void usage(const char *prog) {
    fprintf(stderr, "Usage: %s [-f|--file] [-d|--dir] [-e|--ext ext] [dir ...]\n", prog);
}

int main(int argc, char *argv[]) {
    int opt;
    int only_files = 0;
    int only_dirs = 0;
    const char *ext = NULL;

    static struct option long_options[] = {
        {"file", no_argument,       0, f},
        {"dir",  no_argument,       0, d},
        {"ext",  required_argument, 0, e},
        {0, 0, 0, 0}
    };

    while ((opt = getopt_long(argc, argv, "fde:", long_options, NULL)) != -1) {
        switch (opt) {
            case f:
                only_files = 1;
                break;
            case d:
                only_dirs = 1;
                break;
            case e:
                ext = optarg; // expects extension without leading dot
                break;
            default:
                usage(argv[0]);
                return EXIT_FAILURE;
        }
    }

    // If both -f and -d are set, ambiguous; choose to error to avoid empty output surprises
    if (only_files && only_dirs) {
        fprintf(stderr, "Options -f/--file and -d/--dir are mutually exclusive.\n");
        usage(argv[0]);
        return EXIT_FAILURE;
    }

    int dirs_provided = argc - optind;
    if (dirs_provided <= 0) {
        list_dir(".", only_files, only_dirs, ext);
    } else {
        for (int i = optind; i < argc; i++) {
            const char *d = argv[i];
            // If multiple dirs, print header like ls does
            if (dirs_provided > 1) {
                printf("%s:\n", d);
            }
            list_dir(d, only_files, only_dirs, ext);
            if (dirs_provided > 1 && i < argc - 1) printf("\n");
        }
    }

    return EXIT_SUCCESS;
}
