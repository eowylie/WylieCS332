CS 332
hw3 README
Ethan Wylie - eowylie

COMPILATION INSTRUCTIONS:
=========================
To compile the program:
    make

Or:
    gcc -Wall -Wextra -std=c99 -pedantic -o hw3 hw3.c

RUNNING INSTRUCTIONS:
====================
To run the program:
    ./hw3 <directory_name>

Example:
    ./hw3 /WylieCS332/HW3/projects

DESCRIPTION:
===================
This program analyzes files in a specified directory by creating child processes
for each file. Each child process reports:
- File name
- File size in bytes
- Word count (if .txt)

FEATURES:
=========
- Handles the command-line argument and opens the desired directory
- Creates child processes to read each file
- Uses stat() to get file sizes
- Counts words only in .txt files
- Parent process waits for all children to complete
- Error handling (see below)

ERROR HANDLING:
==============
The program handles the following conditions:
- No command-line argument provided
- Invalid/non-existent directory
- Failed fork() calls
- Files that cannot be opened or read
- Memory allocation failures
- Empty directories
- Permission issues

TESTING EDGE CASES:
==================
- Empty files (word count = 0)
- Directories with no files
- Non-text files (shows "N/A" for word count)
- Directories that don't exist
- Mixed file types in directory

FILES INCLUDED:
==============
- hw3.c 
- Makefile
- README
