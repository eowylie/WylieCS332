/* To Compile: gcc -Wall -o forkexecvp_2 forkexecvp_2.c
 * To Run: ./forkexecvp_2 <command> [args]
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

pid_t child_pid = 0;  // Store child PID

// Signal handler for SIGQUIT
static void sig_quit(int signo) {
    printf("\nReceived SIGQUIT. Terminating parent process...\n");
    if (child_pid > 0) {
        kill(child_pid, SIGTERM);  // Terminate child if still running
    }
    exit(0);
}

// Signal handler for SIGCHLD - when child process changes state
static void sig_child(int signo) {
    int status;
    pid_t pid;
    
    // Use waitpid with WNOHANG to avoid blocking 
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        if (WIFEXITED(status)) {
            printf("Child process [%d] exited with status = %d\n", 
                   pid, WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            printf("Child process [%d] terminated by signal %d\n", 
                   pid, WTERMSIG(status));
        } else if (WIFSTOPPED(status)) {
            printf("Child process [%d] stopped by signal %d\n", 
                   pid, WSTOPSIG(status));
        } else if (WIFCONTINUED(status)) {
            printf("Child process [%d] continued\n", pid);
        }
    }
    
    // Reinstall the signal handler 
    signal(signo, sig_child);
}

// Signal handler for SIGINT (Control-C) - ignore in parent 
static void sig_int(int signo) {
    printf("\nParent: Received SIGINT (Control-C), but ignoring it.\n");
    printf("Child process may be affected. Use Control-\\ to quit parent.\n");
    signal(signo, sig_int);  /* Reinstall handler */
}

// Signal handler for SIGTSTP (Control-Z) - ignore in parent 
static void sig_tstp(int signo) {
    printf("\nParent: Received SIGTSTP (Control-Z), but ignoring it.\n");
    printf("Child process may be suspended. Use Control-\\ to quit parent.\n");
    signal(signo, sig_tstp);  // Reinstall handler 
}

int main(int argc, char **argv) {
    int status;

    if (argc < 2) {
        printf("Usage: %s <command> [args]\n", argv[0]);
        exit(-1);
    }

    // Set up signal handlers for the parent process 
    if (signal(SIGQUIT, sig_quit) == SIG_ERR) {
        perror("signal(SIGQUIT)");
        exit(EXIT_FAILURE);
    }
    
    if (signal(SIGCHLD, sig_child) == SIG_ERR) {
        perror("signal(SIGCHLD)");
        exit(EXIT_FAILURE);
    }
    
    // Install handlers that ignore SIGINT and SIGTSTP in parent 
    if (signal(SIGINT, sig_int) == SIG_ERR) {
        perror("signal(SIGINT)");
        exit(EXIT_FAILURE);
    }
    
    if (signal(SIGTSTP, sig_tstp) == SIG_ERR) {
        perror("signal(SIGTSTP)");
        exit(EXIT_FAILURE);
    }

    child_pid = fork();
    
    if (child_pid == 0) { 
        // This is the child process 
        // Reset signal handlers to default behavior in child 
        signal(SIGINT, SIG_DFL);
        signal(SIGTSTP, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
        signal(SIGCHLD, SIG_DFL);
        
        // Execute the command 
        execvp(argv[1], &argv[1]);
        printf("If you see this statement then execvp failed ;-(\n");
        perror("execvp");
        exit(-1);
        
    } else if (child_pid > 0) { 
        // This is the parent process 
        printf("Parent process [%d] waiting for child process [%d] to terminate\n", 
               getpid(), child_pid);
        printf("Use Control-C or Control-Z to send signals to child\n");
        printf("Use Control-\\ to terminate parent process\n");
        
        // Wait indefinitely for signals - the SIGCHLD handler will process child termination 
        while (1) {
            pause();  // Wait for signals 
        }
        
    } else { 
        // Fork failed 
        perror("fork");
        exit(EXIT_FAILURE);
    }
    
    printf("[%ld]: Exiting program .....\n", (long)getpid());
    return 0;
}
