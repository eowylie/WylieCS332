CS 332 Lab 4 - File Concatenation Program
Ethan Wylie - eowylie

DESCRIPTION:
This program implements a utility that appends the contents
of a second file to the end of a first file.
The program uses open(), read(), write(), and close().

FEATURES:
- Concatenates contents of file2 to the end of file1
- Preserves original content of file1
- Leaves file2 unchanged
- Error checking for same filename
- Error handling for file operations
- Buffered I/O using 4KB buffer
- Proper file descriptor management

COMPILATION:
Using Makefile:
    make

USAGE:
./lab4 <file1> <file2>

- file1: The destination file
- file2: The source file (contents will be appended to file1)

Basic Usage:
$ echo "This is file one!" > file1
$ echo "This is file two!" > file2
$ ./lab4 file1 file2
$ cat file1
This is file one!
This is file two!

TESTING:
The program has been tested with scenarios such as:

1. Basic Functionality Tests:
   - Small text files
   - Files with different content types
   - Empty files
   - Files with special characters

2. Error Condition Tests:
   - Same filename for both arguments
   - Hard links pointing to same file
   - Symbolic links pointing to same file
   - Non-existent source files
   - Permission denied scenarios
   - Missing command-line arguments

3. Performance Tests:
   - Large files (tested up to several MB)
   - Binary files
   - Files with different sizes

Test Commands:
# Run basic functionality tests
make test

# Test error handling
make test-errors

# Test with large files
make test-large

# Clean up test files
make clean-test

SYSTEM CALLS USED:
- open(): Opens files with appropriate flags
- read(): Reads data from source file in chunks
- write(): Writes data to destination file
- close(): Closes file descriptors
- stat(): Gets file metadata for same-file detection

ERROR HANDLING:
- Incorrect number of command-line arguments
- Same source and destination files
- File opening failures
- File reading/writing failures
- File closing failures

PERFORMANCE CHARACTERISTICS:
- Time Complexity: O(n)
- Space Complexity: O(1)

CLEANUP:
========
To remove compiled files:
    make clean

To remove test files:
    make clean-test
