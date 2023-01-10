/****
 * DIY shell
*/

#include "osh.h"

// memory management - breaking. Big issue. Will fix later. For now, do not use.
void clean(char ** arg) {
    int i = 0;
    for (int i = 0; arg[i] && *arg[i]; i++){
        if(check_env("dbg")) printf("Freed %d: <%s>\n", i + 1, arg[i]);
        free(arg[i]);
    }
    
    free(arg);
}

void bangbang(char ** arg, char *** bang) {
    if(check_env("mem") && *bang) clean(*bang);
    *bang = malloc(N_ARGS * sizeof(char *));
    for(int i = 0; arg[i] && *arg[i]; i++){
        (*bang)[i] = malloc(ARG_LEN * sizeof(char));
        strlcpy((*bang)[i], arg[i], 20);
    }
}

int go(char ** cmd) {
    static char ** bang;
    prompt(cmd);
    if(!strlen(*cmd)) return 1;

    int bg = 0;

    char ** arg = parse(*cmd, &bg);
    if(!arg) {arg = bang; if(check_env("logs")) printf("Bang.\n");}
    if(!arg) {printf("No prior commands received.\n"); return 1;}
    
    what_args(arg);

    if(check_env("logs")) printf("Parsed.\n");

    int e;
    if((e = env(arg))) {
        if(check_env("logs")) printf("Ecall.\n");
        if(check_env("mem")) clean(arg);
        return e;
    }

    if(set_path(arg)) {
        printf("OSH: Unknown command: %s\n", arg[0]);
        if(check_env("mem")) clean(arg);
        return 1;
    };
    
    what_args(arg);
    bangbang(arg, &bang);

    spawn(arg, bg);
    what_args(arg);

    if(check_env("mem")) clean(arg);
    arg = 0;

    // int fd;
    // if((fd = check_env("file")) != -1) {
    //     // struct stat buf;
    //     // fstat(STDOUT_FILENO, &buf);
    //     // if(!buf.) dup2(STDOUT_FILENO, fd); 
    //     // else dup2(STDIN_FILENO, fd); 
    //     close(fd);
    //     open(stdin, O_RDONLY);
    //     open(stdout, O_WRONLY);
    //     setenv("file", "-1", 1);
    // }

    return 1;
} 

int main(int argc, char * argv[]) {
    char * cmd;
    make_env();
    v_env(++argv); // environment variables

    while(go(&cmd) != -1) if(check_env("mem")) free(cmd);
    
    if(check_env("mem")) free(cmd);
    if(check_env("dbg")) printf("Out.\n");

    return 0;
}
