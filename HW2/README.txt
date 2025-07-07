CS 332 HW 2 - Search Program
Ethan Wylie - eowylie

This program implements a file system traversal system similar to the 'find' command.
It recursively traverses directories and displays files using function pointers and system calls.

Command-line options:
   -S: Show file stats (size, permissions, last access time)
   -s <size>: Filter files by maximum size (bytes)
   -f <pattern> <depth>: Filter by filename pattern and maximum depth

To compile the program:
    make

USAGE:
./search [directory] [-S] [-s <size>] [-f <pattern> <depth>]

Examples:
1. List current directory:
   ./search

2. List specific directory:
   ./search projects

3. Show file stats:
   ./search -S projects

4. Filter by file size (1024 bytes):
   ./search -s 1024 projects

5. Filter by pattern and depth:
   ./search -f docx 2 projects

6. Combine multiple options:
   ./search -S -s 1024 -f README 1 projects

Test Cases Performed:
1. Basic functionality - traversing directory without options
2. Statistics display with -S option
3. Size filtering with various byte limits
4. Pattern matching with different depths
5. Multiple option combinations
6. Error handling for invalid paths and options
7. Symbolic link handling (if present in test directory)

Test Results:
- Correct hierarchical display with proper indentation
- Accurate file filtering based on size, pattern, and depth
- Proper statistics display (size, permissions, access time)
- Handling of edge cases (empty directories, permission issues)
- Command-line argument parsing and validation

IMPLEMENTATION DETAILS:
Calls Used:
   - opendir(), readdir(), closedir() for directory traversal
   - lstat() for file information retrieval
   - readlink() for symbolic link target resolution

Key Functions:
   - traverse_directory(): Recursive directory traversal using function pointers
   - process_file(): File processing callback function
   - matches_criteria(): Filtering logic for command-line options
   - print_file_info(): Output formatting with proper indentation

Data Structures:
   - SearchOptions struct: Encapsulates all command-line options
   - Function pointer type (ProcessFunc): Enables modular file processing

ERROR HANDLING:
- Invalid command-line arguments
- Inaccessible files
- Incorrect option parameters
- Permission denied

CLEANUP:
To remove compiled files:
    make clean
