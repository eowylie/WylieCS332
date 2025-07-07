/*
Name: Ethan Wylie
BlazerId: eowylie
Project #: 2
To compile: make
To run: ./search [directory] [-S] [-s <size>] [-f <pattern> <depth>]
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
#include <getopt.h>
#include <sys/types.h>

// Structure to hold command-line options
typedef struct {
    int show_stats;           // -S option
    long max_size;           // -s option
    char *pattern;           // -f option pattern
    int max_depth;           // -f option depth
    int has_size_filter;     // flag for -s option
    int has_pattern_filter;  // flag for -f option
} SearchOptions;

// Function pointer type for the file processing function
typedef int (*ProcessFunc)(const char *, const struct stat *, int, SearchOptions *);

// Function prototypes
int traverse_directory(const char *pathname, ProcessFunc func, SearchOptions *opts);
int process_file(const char *pathname, const struct stat *statptr, int type, SearchOptions *opts);
void print_file_info(const char *pathname, const struct stat *statptr, int depth, SearchOptions *opts);
int matches_criteria(const char *pathname, const struct stat *statptr, int depth, SearchOptions *opts);
char *get_permissions(mode_t mode);
void print_usage(const char *program_name);

// Constants for file types
#define FTW_F   1    // Regular file
#define FTW_D   2    // Directory
#define FTW_DNR 3    // Directory that can't be read
#define FTW_NS  4    // File that we can't stat

// Global variable to track current depth
static int current_depth = 0;

int main(int argc, char *argv[]) {
    SearchOptions opts = {0};
    opts.max_size = -1;  // No size limit by default
    opts.max_depth = -1; // No depth limit by default
    
    int opt;
    char *start_path = ".";  // Default to current directory
    
    // Parse command line options
    while ((opt = getopt(argc, argv, "Ss:f:")) != -1) {
        switch (opt) {
            case 'S':
                opts.show_stats = 1;
                break;
            case 's':
                opts.max_size = atol(optarg);
                opts.has_size_filter = 1;
                break;
            case 'f':
                opts.pattern = optarg;
                opts.has_pattern_filter = 1;
                // Next argument should be depth
                if (optind < argc) {
                    opts.max_depth = atoi(argv[optind]);
                    optind++;
                } else {
                    fprintf(stderr, "Error: -f option requires pattern and depth\n");
                    print_usage(argv[0]);
                    exit(1);
                }
                break;
            default:
                print_usage(argv[0]);
                exit(1);
        }
    }
    
    // Check if directory path is provided
    if (optind < argc) {
        start_path = argv[optind];
    }
    
    // Start traversing from the specified directory
    if (traverse_directory(start_path, process_file, &opts) < 0) {
        perror("traverse_directory failed");
        exit(1);
    }
    
    return 0;
}

int traverse_directory(const char *pathname, ProcessFunc func, SearchOptions *opts) {
    struct stat statbuf;
    DIR *dp;
    struct dirent *dirp;
    char *ptr;
    char full_path[1024];
    int ret = 0;
    
    // Get file/directory information
    if (lstat(pathname, &statbuf) < 0) {
        return func(pathname, &statbuf, FTW_NS, opts);
    }
    
    // If it's not a directory, just process it
    if (!S_ISDIR(statbuf.st_mode)) {
        return func(pathname, &statbuf, FTW_F, opts);
    }
    
    // Process the directory itself
    if ((ret = func(pathname, &statbuf, FTW_D, opts)) != 0) {
        return ret;
    }
    
    // Prepare for directory traversal
    ptr = full_path + strlen(pathname);
    strcpy(full_path, pathname);
    if (ptr[-1] != '/') {
        *ptr++ = '/';
    }
    
    // Open directory
    if ((dp = opendir(pathname)) == NULL) {
        return func(pathname, &statbuf, FTW_DNR, opts);
    }
    
    // Increase depth for subdirectory processing
    current_depth++;
    
    // Read directory entries
    while ((dirp = readdir(dp)) != NULL) {
        if (strcmp(dirp->d_name, ".") == 0 || strcmp(dirp->d_name, "..") == 0) {
            continue;  // Skip . and ..
        }
        
        strcpy(ptr, dirp->d_name);
        
        if ((ret = traverse_directory(full_path, func, opts)) != 0) {
            break;
        }
    }
    
    // Decrease depth when returning from subdirectory
    current_depth--;
    
    closedir(dp);
    return ret;
}

int process_file(const char *pathname, const struct stat *statptr, int type, SearchOptions *opts) {
    static int first_call = 1;
    static char root_dir[1024];
    
    // Store the root directory on first call
    if (first_call) {
        strcpy(root_dir, pathname);
        first_call = 0;
    }
    
    // Skip processing if file doesn't match criteria
    if (!matches_criteria(pathname, statptr, current_depth, opts)) {
        return 0;
    }
    
    switch (type) {
        case FTW_F:  // Regular file
            if (matches_criteria(pathname, statptr, current_depth, opts)) {
                print_file_info(pathname, statptr, current_depth, opts);
            }
            break;
            
        case FTW_D:  // Directory
            if (current_depth == 0) {
                // Root directory - just print name
                printf("%s\n", pathname);
            } else {
                // Subdirectory - print with proper indentation
                for (int i = 0; i < current_depth; i++) {
                    printf("\t");
                }
                
                // Extract just the directory name
                const char *dir_name = strrchr(pathname, '/');
                if (dir_name) {
                    dir_name++;  // Skip the '/'
                } else {
                    dir_name = pathname;
                }
                printf("%s\n", dir_name);
            }
            break;
            
        case FTW_DNR: // Directory that can't be read
            fprintf(stderr, "can't read directory %s\n", pathname);
            break;
            
        case FTW_NS:  // File that can't be stat
            fprintf(stderr, "stat error for %s\n", pathname);
            break;
            
        default:
            fprintf(stderr, "unknown type %d for pathname %s\n", type, pathname);
    }
    
    return 0;
}

void print_file_info(const char *pathname, const struct stat *statptr, int depth, SearchOptions *opts) {
    // Print indentation based on depth
    for (int i = 0; i < depth; i++) {
        printf("\t");
    }
    
    // Extract filename from path
    const char *filename = strrchr(pathname, '/');
    if (filename) {
        filename++;  // Skip the '/'
    } else {
        filename = pathname;
    }
    
    // Check if it's a symbolic link
    if (S_ISLNK(statptr->st_mode)) {
        char link_target[1024];
        ssize_t len = readlink(pathname, link_target, sizeof(link_target) - 1);
        if (len != -1) {
            link_target[len] = '\0';
            if (current_depth == 0) {
                printf("%s (%s)\n", filename, link_target);
            } else {
                printf("\t%s (%s)\n", filename, link_target);
            }
        } else {
            printf("%s\n", filename);
        }
    } else {
        printf("%s", filename);
        
        // Print additional statistics if -S option is specified
        if (opts && opts->show_stats) {
            printf(" (");
            if (S_ISDIR(statptr->st_mode)) {
                printf("0, ");  // Size 0 for directories
            } else {
                printf("%ld, ", statptr->st_size);
            }
            
            char *perms = get_permissions(statptr->st_mode);
            printf("%s, ", perms);
            free(perms);
            
            char time_str[100];
            struct tm *tm_info = localtime(&statptr->st_atime);
            strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", tm_info);
            printf("%s", time_str);
            printf(")");
        }
        printf("\n");
    }
}

int matches_criteria(const char *pathname, const struct stat *statptr, int depth, SearchOptions *opts) {
    // Check size filter
    if (opts->has_size_filter && !S_ISDIR(statptr->st_mode)) {
        if (statptr->st_size > opts->max_size) {
            return 0;
        }
    }
    
    // Check pattern and depth filter
    if (opts->has_pattern_filter) {
        const char *filename = strrchr(pathname, '/');
        if (filename) {
            filename++;  // Skip the '/'
        } else {
            filename = pathname;
        }
        
        // Check if filename contains the pattern and depth is within limit
        if (strstr(filename, opts->pattern) == NULL || depth > opts->max_depth) {
            return 0;
        }
    }
    
    return 1;
}

char *get_permissions(mode_t mode) {
    char *perms = malloc(11);
    strcpy(perms, "----------");
    
    if (S_ISDIR(mode)) perms[0] = 'd';
    else if (S_ISLNK(mode)) perms[0] = 'l';
    else if (S_ISREG(mode)) perms[0] = '-';
    
    if (mode & S_IRUSR) perms[1] = 'r';
    if (mode & S_IWUSR) perms[2] = 'w';
    if (mode & S_IXUSR) perms[3] = 'x';
    if (mode & S_IRGRP) perms[4] = 'r';
    if (mode & S_IWGRP) perms[5] = 'w';
    if (mode & S_IXGRP) perms[6] = 'x';
    if (mode & S_IROTH) perms[7] = 'r';
    if (mode & S_IWOTH) perms[8] = 'w';
    if (mode & S_IXOTH) perms[9] = 'x';
    
    return perms;
}

void print_usage(const char *program_name) {
    printf("Usage: %s [directory] [-S] [-s <size>] [-f <pattern> <depth>]\n", program_name);
    printf("Options:\n");
    printf("  -S              Show file size, permissions, and last access time\n");
    printf("  -s <size>       Show files with size <= <size> bytes\n");
    printf("  -f <pattern> <depth>  Show files containing <pattern> with depth <= <depth>\n");
}
