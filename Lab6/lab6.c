/*
 * Lab 6 - CS 332
 * Ethan Wylie - eowylie
 *
 * This program reads commands from a file and executes them using fork-exec.
 *
 * To Compile: gcc -Wall -o lab6 lab6.c
 * To Run: ./lab6 <input_file>
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>

#define MAX_LINE_LENGTH 256
#define MAX_ARGS 64
#define LOG_FILE "output.log"

/*
 * Parse a command line into an array of arguments
 * Returns the number of arguments
 */
int parse_command(char *line, char **args) {
    int argc = 0;
    char *token;
    
    // Remove newline if present
    line[strcspn(line, "\n")] = 0;
    
    // Skip empty lines
    if (strlen(line) == 0) {
        return 0;
    }
    
    // Tokenize
    token = strtok(line, " \t");
    while (token != NULL && argc < MAX_ARGS - 1) {
        args[argc] = token;
        argc++;
        token = strtok(NULL, " \t");
    }
    args[argc] = NULL; // Null-terminate
    
    return argc;
}

// Execute a command using fork-exec and log the time

void execute_command(char **args, char *original_line, FILE *log_file) {
    pid_t pid;
    int status;
    time_t start_time, end_time;
    char *start_str, *end_str;
    
    // Record start time
    start_time = time(NULL);
    start_str = ctime(&start_time);
    // Remove newline from ctime output
    start_str[strlen(start_str) - 1] = '\0';
    
    pid = fork();
    
    if (pid == 0) {
        // Child process - execute the command
        execvp(args[0], args);
        
        // If it reached here, execvp failed
        perror("execvp failed");
        exit(-1);
        
    } else if (pid > 0) {
        // Wait for child to complete
        wait(&status);
        
        // Record end time
        end_time = time(NULL);
        end_str = ctime(&end_time);
        // Remove newline from ctime output
        end_str[strlen(end_str) - 1] = '\0';
        
        // Write to log file
        fprintf(log_file, "%s\t%s\t%s\n", original_line, start_str, end_str);
        fflush(log_file); // Ensure data is written immediately
        
        if (WIFEXITED(status)) {
            printf("Command '%s' executed successfully with exit status %d\n", 
                   args[0], WEXITSTATUS(status));
        } else {
            printf("Command '%s' did not terminate normally\n", args[0]);
        }
        
    } else {
        // Fork failed
        perror("fork failed");
        return;
    }
}

int main(int argc, char **argv) {
    FILE *input_file, *log_file;
    char line[MAX_LINE_LENGTH];
    char original_line[MAX_LINE_LENGTH];
    char *args[MAX_ARGS];
    int arg_count;
    
    // Check arguments
    if (argc != 2) {
        printf("Usage: %s <input_file>\n", argv[0]);
        exit(-1);
    }
    
    // Read input file
    input_file = fopen(argv[1], "r");
    if (input_file == NULL) {
        perror("Error opening input file");
        exit(-1);
    }
    
    // Open log file
    log_file = fopen(LOG_FILE, "w");
    if (log_file == NULL) {
        perror("Error opening log file");
        fclose(input_file);
        exit(-1);
    }
    
    printf("Processing commands from file: %s\n", argv[1]);
    printf("Logging execution times to: %s\n", LOG_FILE);
    printf("----------------------------------------\n");
    
    // Read and process each line from the input file
    while (fgets(line, sizeof(line), input_file) != NULL) {
        // Save original line for logging
        strcpy(original_line, line);
        // Remove newline for display
        original_line[strcspn(original_line, "\n")] = 0;
        
        // Skip empty lines
        if (strlen(original_line) == 0) {
            continue;
        }
        
        printf("Executing: %s\n", original_line);
        
        // Parse the command line
        arg_count = parse_command(line, args);
        
        if (arg_count > 0) {
            // Execute the command
            execute_command(args, original_line, log_file);
        }
        
        printf("----------------------------------------\n");
    }
    
    // Clean up
    fclose(input_file);
    fclose(log_file);
    
    printf("All commands were executed. Check %s for timing information.\n", LOG_FILE);
    
    return 0;
}
