#include "osh.h"



void bb_wait(int f) {
    while (waitpid(f, NULL, 0) == -1) if(check_env("dbg")) printf(".");
}

// multiprocessing part.
void spawn(char ** arg, int bg) {
    if(check_env("logs")) 
        printf("Spawn.\n");
    if(check_env("dbg"))
        printf("bg: %d\n", bg);
    
    int f = fork();
    if(f) {
        if(check_env("logs")) printf("Parent.\n");
        if(bg) {
            if(check_env("logs")) printf("No.\n");
        }
        else {
            if(check_env("logs")) printf("Wait.\n"); 
            bb_wait(f); // blank (no data) blocking wait
        }
        // sleep(0.5);
        if(check_env("logs")) printf("Done.\n");
        setenv("file", "-1", 1);
    }
    else {
        if(check_env("logs")) printf("Child.\n");
        what_args(arg);
        int fd, io = check_env("io");
        if((fd = check_env("file")) != -1) dup2(fd, io);
        setenv("io", "-1", 1);
        
        execvp(arg[0], arg);
    }
}
