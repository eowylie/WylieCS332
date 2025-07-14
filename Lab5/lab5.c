/*
Lab 5 - Recursive Directory Traversal
Ethan Wylie - eowylie
Compile: gcc -Wall -o lab5 lab5.c
Run: ./lab5 <directory_name>
This program recursively traverses a directory hierarchy and lists all files and subdirectories.
*/

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

// Function to get file type as a string
char *filetype(unsigned char type) {
    char *str;
    switch(type) {
        case DT_BLK: str = "block device"; break;
        case DT_CHR: str = "character device"; break;
        case DT_DIR: str = "directory"; break;
        case DT_FIFO: str = "named pipe (FIFO)"; break;
        case DT_LNK: str = "symbolic link"; break;
        case DT_REG: str = "regular file"; break;
        case DT_SOCK: str = "UNIX domain socket"; break;
        case DT_UNKNOWN: str = "unknown file type"; break;
        default: str = "UNKNOWN";
    }
    return str;
}

// Recursive function to traverse directory
void traverse_directory(const char *path, int depth) {
    DIR *dir;
    struct dirent *entry;
    char full_path[1024];
    int i;

    // Open the directory
    dir = opendir(path);
    if (dir == NULL) {
        printf("Error: Cannot open directory '%s'\n", path);
        return;
    }

    // Read directory entries
    while ((entry = readdir(dir)) != NULL) {
        // Skip current directory and parent directory
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        // Print indentation based on depth
        for (i = 0; i < depth; i++) {
            printf("  ");
        }

        // Print the file name and type
        printf("%s (%s)\n", entry->d_name, filetype(entry->d_type));

        // If directory, traverse it
        if (entry->d_type == DT_DIR) {
            // Make full path
            snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);
            
            // Recursively call the function
            traverse_directory(full_path, depth + 1);
        }
    }

    closedir(dir);
}

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: %s <directory_name>\n", argv[0]);
        printf("This program recursively traverses a directory hierarchy\n");
        printf("and lists all files and subdirectories.\n");
        exit(-1);
    }

    // Check if the provided path is a directory
    struct stat path_stat;
    if (stat(argv[1], &path_stat) != 0) {
        printf("Error: Cannot access '%s'\n", argv[1]);
        perror("stat");
        exit(-1);
    }

    if (!S_ISDIR(path_stat.st_mode)) {
        printf("Error: '%s' is not a directory\n", argv[1]);
        exit(-1);
    }

    printf("Recursive directory listing for '%s':\n", argv[1]);
    printf("=====================================\n");
    
    traverse_directory(argv[1], 0);

    return 0;
}
