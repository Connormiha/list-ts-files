#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

void listFiles(const char *path);

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <directory>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    listFiles(argv[1]);
    return 0;
}

int hasExtension(const char *filename, const char *ext) {
    size_t len_filename = strlen(filename);
    size_t len_ext = strlen(ext);
    if (len_filename < len_ext) {
        return 0;
    }
    return strcmp(filename + len_filename - len_ext, ext) == 0;
}

void listFiles(const char *path) {
    struct dirent *entry;
    DIR *dp = opendir(path);

    if (dp == NULL) {
        perror("opendir");
        return;
    }

    while ((entry = readdir(dp))) {
        char fullPath[1024];
        struct stat statbuf;

        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        snprintf(fullPath, sizeof(fullPath), "%s/%s", path, entry->d_name);

        if (stat(fullPath, &statbuf) == -1) {
            perror("stat");
            continue;
        }

        if (S_ISDIR(statbuf.st_mode)) {
            listFiles(fullPath);
        } else {
            if (hasExtension(entry->d_name, ".ts") || hasExtension(entry->d_name, ".tsx")) {
                printf("File: %s\n", fullPath);
            }
        }
    }

    closedir(dp);
}
