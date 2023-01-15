#include "osh.h"

void set_pipes(Exec * the) {
    if(check_env("dbg")) printf("Pipe be.\n");
    // if(check_env("caret")) return;
    int piping[2];
    pipe(piping);
    the->fd[1] = piping[1];
    the->pipee->fd[0] = piping[0];
    if(check_env("dbg")) printf("Piped be: %d %d.\n", piping[0], piping[1]);
    what_exec(the);
    if(the->pipee->pipee) {set_pipes(the->pipee);}
    if(check_env("dbg")) printf("Pipes set.\n");
}

void bb_wait(int f) {
    while (waitpid(f, NULL, 0) == -1) if(check_env("dbg")) printf(".");
}

void pipist(Exec * the) {
    int g = fork();
    if(g) {
        dup2(the->fd[0], 0);
        dup2(the->fd[1], 1);
        if(check_env("dbg")) printf("Output piped.\n");
        execvp(the->args[0], the->args);
    }
    else {
        what_exec(the->pipee);
        dup2(the->pipee->fd[0], 0);
        if(check_env("dbg")) printf("Input piped.\n");
        dup2(the->pipee->fd[1], 1);
        if(check_env("dbg") && the->pipee->fd[1] != 1) printf("Pipe redirected: %d\n", the->pipee->fd[1]);
        what_exec(the->pipee);
        execvp(the->pipee->args[0], the->pipee->args);
    }
}

// multiprocessing part.
void spawn(Exec * the) {
    if(check_env("logs")) 
        printf("Spawn.\n");
    if(check_env("dbg"))
        printf("bg: %d\n", the->fgbg);
    if(the->pipee) set_pipes(the);
    
    int f = fork();
    if(f) {
        if(check_env("logs")) printf("Parent.\n");
        if(the->fgbg) {
            if(check_env("logs")) printf("No.\n");
        }
        else {
            if(check_env("logs")) printf("Wait.\n"); 
            bb_wait(f); // blank (no data) blocking wait
        }
        if(check_env("logs")) printf("Done.\n");

    }
    else {
        if(check_env("logs")) printf("Child.\n");
        what_exec(the);
        if(the->pipee) {
            pipist(the);
        }
        else {
            dup2(the->fd[0], 0);
            dup2(the->fd[1], 1);
            execvp(the->args[0], the->args);
        }
        
        
    }
}
