#include "osh.h"
extern History * his;
Exec * build_exec(char ** cmd, int bg) {
    Exec * the = parse(*cmd, bg);

    if(!the) printf("WTF."); // Not supposed to happen.
    
    what_exec(the);

    if(check_env("logs")) printf("Parsed.\n");

    // int e;
    if(!env(the->args)) { // separates ecalls from actual commands.
        if(check_env("logs")) printf("Ecall.\n");
        // if(check_env("mem")) clean(arg);
        return NULL;
    }
    if(check_env("logs")) printf("Not env.\n");

    if(set_path(the->args)) { // finds unknown commands. 
        printf("OSH: Unknown command: %s\n", the->args[0]);
        fflush(stdin);
        fflush(stdout);
        return NULL;
    };
    if(check_env("logs")) printf("Path.\n");

    return the;
}