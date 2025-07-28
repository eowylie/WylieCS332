# Lab 7
## CS 332
## Ethan Wylie - eowylie

### Assignment Overview
This lab assignment demonstrates signal handling by modifying `forkexecvp.c`. The goal is to ensure that when CTRL-C (SIGINT) or CTRL-Z (SIGTSTP) are pressed, the child process is stopped, and the parent process continues until it receives a quit signal (CTRL-\ / SIGQUIT).

### Features

#### Signal Handling
1. **Parent Process Signal Handlers:**
   - `SIGINT` (CTRL-C) and `SIGTSTP` (Control-Z): Ignored by parent, displays message
   - `SIGQUIT` (Control-\): Terminates parent process cleanly
   - `SIGCHLD`: Handles child process state changes

2. **Child Process Signal Handling:**
   - Child responds normally to Control-C and Control-Z

3. **Process Communication:**
   - Parent monitors child through SIGCHLD
   - Proper cleanup when parent receives SIGQUIT
   - Messages about signal handling behavior

### Compilation Instructions

#### Using Makefile
```bash
make
```

#### Manual
```bash
gcc -Wall -g -o forkexecvp_2 forkexecvp_2.c
```

### Running the Program

#### Basic Usage
```bash
./forkexecvp_2 <command> [arguments]
```

### Testing Signal Behavior

1. **Start the program:**
   ```bash
   ./forkexecvp_2 sleep 30
   ```

2. **Test Control-C (SIGINT):**
   - Press `Ctrl+C`
   - Child process terminates, parent continues and shows message

3. **Test Control-Z (SIGTSTP):**
   - Press `Ctrl+Z`
   - Child process suspends, parent continues and shows message
   - Use `ps` to verify both processes exist
   - Send `SIGCONT` to child to resume: `kill -CONT <child_pid>`

4. **Test Control-\ (SIGQUIT):**
   - Press `Ctrl+\`
   - Parent process terminates
   - Any running child should also be terminated
