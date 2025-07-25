/* 
Name: Ethan Wylie
BlazerId: eowylie
Project #: 3
To compile: 
To run: 
*/ 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <ctype.h>

// Count words in a text file
int count_words(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return -1; 
    }
    
    int word_count = 0;
    int in_word = 0;
    int ch;
    
    while ((ch = fgetc(file)) != EOF) {
        if (isspace(ch)) {
            in_word = 0;
        } else if (!in_word) {
            in_word = 1;
            word_count++;
        }
    }
    
    fclose(file);
    return word_count;
}

// Function to check if file is .txt
int is_text_file(const char *filename) {
    const char *dot = strrchr(filename, '.');
    if (dot && strcmp(dot, ".txt") == 0) {
        return 1;
    }
    return 0;
}

// Child process function to analyze a file
void analyze_file(const char *directory, const char *filename) {
    char filepath[1024];
    struct stat file_stat;
    
    // Construct full file path
    snprintf(filepath, sizeof(filepath), "%s/%s", directory, filename);
    
    // Get file statistics
    if (stat(filepath, &file_stat) == -1) {
        fprintf(stderr, "Error: Cannot get file statistics for %s: %s\n", 
                filename, strerror(errno));
        exit(1);
    }
    
    // Check if it's a regular file
    if (!S_ISREG(file_stat.st_mode)) {
        // Skip non-regular files (directories, etc.)
        exit(0);
    }
    
    // Get file size
    long file_size = file_stat.st_size;
    
    // Count words only for .txt files
    if (is_text_file(filename)) {
        int word_count = count_words(filepath);
        if (word_count == -1) {
            fprintf(stderr, "Error: Cannot read file %s: %s\n", 
                    filename, strerror(errno));
            printf("File: %s | Size: %ld bytes | Words: Unable to read\n", 
                   filename, file_size);
        } else {
            printf("File: %s | Size: %ld bytes | Words: %d\n", 
                   filename, file_size, word_count);
        }
    } else {
        // For non-text files, don't count words
        printf("File: %s | Size: %ld bytes | Words: N/A (not a .txt file)\n", 
               filename, file_size);
    }
}

int main(int argc, char *argv[]) {
    // Check command line arguments
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <directory_name>\n", argv[0]);
        exit(1);
    }
    
    const char *directory = argv[1];
    DIR *dir;
    struct dirent *entry;
    pid_t *child_pids;
    int child_count = 0;
    int max_children = 100; // Initial allocation
    
    // Open the directory
    dir = opendir(directory);
    if (dir == NULL) {
        fprintf(stderr, "Error: Directory not found or cannot be accessed: %s\n", 
                strerror(errno));
        exit(1);
    }
    
    // Allocate memory for child PIDs
    child_pids = malloc(max_children * sizeof(pid_t));
    if (child_pids == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        closedir(dir);
        exit(1);
    }
    
    // Read directory entries and create child processes
    while ((entry = readdir(dir)) != NULL) {
        // Skip current directory and parent directory
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        
        // Check if we need more space for child PIDs
        if (child_count >= max_children) {
            max_children *= 2;
            child_pids = realloc(child_pids, max_children * sizeof(pid_t));
            if (child_pids == NULL) {
                fprintf(stderr, "Error: Memory reallocation failed\n");
                closedir(dir);
                exit(1);
            }
        }
        
        // Create child process
        pid_t pid = fork();
        
        if (pid == -1) {
            // Fork failed
            fprintf(stderr, "Error: Failed to create child process: %s\n", 
                    strerror(errno));
            closedir(dir);
            free(child_pids);
            exit(1);
        } else if (pid == 0) {
            // Child process
            closedir(dir); // Close directory in child
            analyze_file(directory, entry->d_name);
            exit(0);
        } else {
            // Store child PID
            child_pids[child_count] = pid;
            child_count++;
        }
    }
    
    closedir(dir);
    
    // Handle case where directory has no files
    if (child_count == 0) {
        printf("No files found in directory: %s\n", directory);
        free(child_pids);
        return 0;
    }
    
    // Wait for child processes to complete
    int status;
    int failed_children = 0;
    
    for (int i = 0; i < child_count; i++) {
        if (waitpid(child_pids[i], &status, 0) == -1) {
            fprintf(stderr, "Error: Failed to wait for child process %d: %s\n", 
                    child_pids[i], strerror(errno));
            failed_children++;
        } else if (WEXITSTATUS(status) != 0) {
            failed_children++;
        }
    }
    
    // Report summary
    if (failed_children > 0) {
        fprintf(stderr, "Warning: %d child processes failed\n", failed_children);
    }
    
    free(child_pids);
    return 0;
}
