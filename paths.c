#include "osh.h"

// returns path to executable. Will be extended.
extern int dbg, logs, sarg, mem;
void set_path(char ** arg) {
    FILE * f; // check for existence

    if(logs) printf("Path.\n");
    if(fopen(arg[0], "r")) return; // current directory


    char * path = calloc(6 + strlen(arg[0]), sizeof(char));
    sprintf(path, "/bin/");
    sprintf(path + 5, "%s", arg[0]);
    if(dbg) printf("%s\n", path);

    if((f = fopen(path, "r")) != 0){
        if(mem) free(arg[0]);
        arg[0] = path;
        return;
    }
    if(logs) printf("Not in /bin\n");

    char * temp = realloc(path, (10 + strlen(arg[0])) * sizeof(char));
    if(mem) free(path);
    path = temp;

    sprintf(path, "/usr/bin/");
    sprintf(path + 9, "%s", arg[0]);
    if(dbg) printf("%s\n", path);

    if((f = fopen(path, "r")) != 0){
        if(mem) free(arg[0]);
        arg[0] = path;
        return;
    }
    
}
