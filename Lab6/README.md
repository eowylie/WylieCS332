# Lab 6
# lab6.c
# Ethan Wylie - eowylie

## Overview
This program demonstrates process creation and management using fork-exec-wait.
It reads commands from an input file, executes each command in a separate child process, and logs the execution times.

## Features
- Reads commands from a text file
- Creates child processes that execute the commands
- Measures execution time for each command
- Handles command-line arguments
- Error handling for file operations and process creation

## Compilation

```bash
make
```
or
```bash
gcc -Wall -O -o lab6 lab6.c
```

## Usage

```bash
./lab6 <input_file>
```

### Example:
```bash
./lab6 commands.txt
```

## Input File Format
Input file should contain one command per line. Each line can include the command and its arguments separated by spaces.

### Sample Input File
```
uname -a
date
whoami
ls -la
echo "Hello, World!"
sleep 2
ps aux
```

## Output
The program produces two types of output:

### 1. Console Output
- Progress messages showing which commands are being executed
- Success/failure status for each command

### 2. Log File (`output.log`)
Tab-separated format with three columns:
- Command (with arguments)
- Start time
- End time

### Sample Output
```
uname -a	Mon Jul 21 03:58:47 2025	Mon Jul 21 03:58:47 2025
date	Mon Jul 21 03:58:47 2025	Mon Jul 21 03:58:47 2025
whoami	Mon Jul 21 03:58:47 2025	Mon Jul 21 03:58:47 2025
ls -la	Mon Jul 21 03:58:47 2025	Mon Jul 21 03:58:47 2025
echo "Hello, World!"	Mon Jul 21 03:58:47 2025	Mon Jul 21 03:58:47 2025
sleep 2	Mon Jul 21 03:58:49 2025	Mon Jul 21 03:58:49 2025
ps aux	Mon Jul 21 03:58:49 2025	Mon Jul 21 03:58:49 2025
```

### Error Handling
- File opening errors
- Process creation failures
- Execution failures
- Cleanup
