/*
To compile: gcc -Wall -o lab4 lab4.c
To run: ./lab4 <file1> <file2>
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>

#define BUFFSIZE 4096

int main(int argc, char *argv[]) {
    int file1_fd, file2_fd;
    ssize_t n;
    char buf[BUFFSIZE];
    struct stat stat1, stat2;
    
    // Check if correct number of arguments is provided
    if (argc != 3) {
        printf("Usage: %s <file1> <file2>\n", argv[0]);
        printf("This program appends the contents of file2 to file1\n");
        exit(-1);
    }
    
    // Check if the two filenames are the same
    if (strcmp(argv[1], argv[2]) == 0) {
        printf("Error: Source and destination files cannot be the same\n");
        exit(-1);
    }
    
    // Compare to handle hard links and symbolic links
    if (stat(argv[1], &stat1) == 0 && stat(argv[2], &stat2) == 0) {
        if (stat1.st_dev == stat2.st_dev && stat1.st_ino == stat2.st_ino) {
            printf("Error: Source and destination files are the same (same inode)\n");
            exit(-1);
        }
    }
    
    // Open file1 in append mode
    file1_fd = open(argv[1], O_WRONLY | O_APPEND);
    if (file1_fd == -1) {
        printf("Error: Cannot open file '%s' for writing\n", argv[1]);
        printf("Make sure the file exists and you have write permissions\n");
        exit(-1);
    }
    
    // Open file2 in read-only mode
    file2_fd = open(argv[2], O_RDONLY);
    if (file2_fd == -1) {
        printf("Error: Cannot open file '%s' for reading\n", argv[2]);
        printf("Make sure the file exists and you have read permissions\n");
        close(file1_fd);  // Clean up already opened file
        exit(-1);
    }
    
    // Read from file2 and append to file1
    while ((n = read(file2_fd, buf, BUFFSIZE)) > 0) {
        if (write(file1_fd, buf, n) != n) {
            printf("Error: Failed to write to file '%s'\n", argv[1]);
            close(file1_fd);
            close(file2_fd);
            exit(-1);
        }
    }
    
    // Check if read operation encountered an error
    if (n < 0) {
        printf("Error: Failed to read from file '%s'\n", argv[2]);
        close(file1_fd);
        close(file2_fd);
        exit(-1);
    }
    
    // Close both files
    if (close(file1_fd) == -1) {
        printf("Warning: Error closing file '%s'\n", argv[1]);
    }
    
    if (close(file2_fd) == -1) {
        printf("Warning: Error closing file '%s'\n", argv[2]);
    }
    
    printf("Successfully appended contents of '%s' to '%s'\n", argv[2], argv[1]);
    
    return 0;
}
