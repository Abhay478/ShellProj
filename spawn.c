#include "osh.h"

extern int dbg, logs, sarg, mem;


void bb_wait(int f) {
    while (waitpid(f, NULL, 0) == -1) if(dbg) printf(".");
}

// multiprocessing part.
void spawn(char ** arg, int bg) {
    if(logs) 
        printf("Spawn.\n");
    if(dbg)
        printf("bg: %d\n", bg);
    
    int f = fork();
    if(f) {
        if(logs) printf("Parent.\n");
        if(bg) {
            if(logs) printf("No.\n");
        }
        else {
            if(logs) printf("Wait.\n"); 
            bb_wait(f); // blank (no data) blocking wait
        }
        // sleep(0.5);
        if(logs) printf("Done.\n");
    }
    else {
        if(logs) printf("Child.\n");
        if(sarg) what_args(arg);
        
        execvp(arg[0], arg);
    }
}
