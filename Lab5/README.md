Lab 5 - Recursive Directory Traversal
CS 332

Overview
This program extends the basic `readdir` functionality to recursively traverse a directory.

Features
- Recursive directory traversal
- File type identification and display
- Proper error handling for inaccessible directories
- Indented output to show directory hierarchy
- Skip hidden directories (. and ..)
- Validation of input arguments

Compilation

Using Makefile
```bash
make
```

Usage
```bash
./lab5 <directory_name>
```

Examples
```bash
# List current directory recursively
./lab5 .

# List home directory recursively
./lab5 ~

# List a specific directory
./lab5 /usr/local

# List relative path
./lab5 ../Documents
```
Sample Output

Recursive directory listing for '.':
=====================================
  lab5.c (regular file)
  Makefile (regular file)
  README.md (regular file)
  lab5 (regular file)
  test_dir (directory)
    file1.txt (regular file)
    file2.txt (regular file)
    subdir (directory)
      nested_file.txt (regular file)

Program Structure

traverse_directory Function
- Opens and reads directory entries using `opendir()` and `readdir()`
- Filters out current (.) and parent (..) directories
- Prints files and directories with proper indentation
- Recursively calls itself for subdirectories
- Constructs full paths for nested directories

filetype Function
- Converts dirent file type constants to human-readable strings
- Handles various file types (regular files, directories, devices, etc.)


Error Handling
- Validates number of command line arguments
- Checks if the target path exists and is accessible
- Verifies that the target is actually a directory
- Handles permission errors when accessing subdirectories
- Provides meaningful error messages with context

Makefile Targets
- `make` or `make all` - Build the program
- `make clean` - Remove compiled files
- `make test` - Test with current directory
- `make test-home` - Test with home directory
- `make help` - Show available targets

Technical Details

Directory Traversal Algorithm
1. Open the directory using `opendir()`
2. Read with `readdir()`
3. Skip "." and ".." entries
4. Print current entry with indentation
5. If entry is a directory, recursively call traversal function
6. Close directory with `closedir()`

Testing
The program has been tested with:
- Empty directories
- Deeply nested hierarchies
- Directories with mixed file types
- Directories with special characters in names
- Large directories with many files
