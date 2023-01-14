#include "osh.h"

// returns cmd to executable.
int set_path(char ** arg) {
    FILE * f; // check for existence
    if((f = fopen(arg[0], "r"))) {fclose(f); return 0;} // current directory
    if(check_env("logs")) printf("Exploring path.\n");
    char * places = calloc(strlen(getenv("PATH")), sizeof(char));
    strlcpy(places, getenv("PATH"), strlen(getenv("PATH")));
    while(places) {
        char * temp = strsep(&places, ":");
        char * cmd = calloc(1 + strlen(temp) + strlen(arg[0]), sizeof(char));
        sprintf(cmd, "%s/%s", temp, arg[0]);

        if(check_env("dbg")) printf("%s\n", cmd);

        if((f = fopen(cmd, "r")) != 0){
            if(check_env("mem")) {char * dummy = arg[0]; free(dummy);}
            if(check_env("logs")) printf("%s\n", cmd);
            arg[0] = cmd;
            fclose(f);
            return 0;
        }
        else{
            if(check_env("logs")) printf("Not in %s\n", temp);
            if(check_env("mem")) free(cmd);
        }
    }

    if(check_env("mem")) free(places);

    // Invalid command:
    return -1;
    
}
