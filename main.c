/****
 * DIY shell
*/

#include "osh.h"

int dbg = 0;
int logs = 0;
int sarg = 0;
int mem = 1;

// memory management - breaking
void clean(char ** arg) {
    int i = 0;
    for (int i = 0; arg[i] && *arg[i]; i++){
        if(dbg) printf("Freed %d: <%s>\n", i + 1, arg[i]);
        free(arg[i]);
    }
    
    free(arg);
}

void bangbang(char ** arg, char *** bang) {
    if(mem && *bang) clean(*bang);
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
    if(!arg) {arg = bang; if(logs) printf("Bang.\n");}
    if(!arg) {printf("No prior commands received.\n"); return 1;}
    if(sarg) what_args(arg);

    if(logs) printf("Parsed.\n");

    int e;
    if((e = env(arg[0]))) {
        if(mem) clean(arg);
        return e;
    }
    set_path(arg);
    if(sarg) what_args(arg);
    bangbang(arg, &bang);

    spawn(arg, bg);
    if(sarg) what_args(arg);

    if(mem) clean(arg);
    arg = 0;
    return 1;
} 

int main(int argc, char * argv[]) {
    char * cmd;
    v_env(++argv); // environment variables

    while(go(&cmd) != -1) if(mem) free(cmd);
    
    if(mem) free(cmd);
    if(dbg) printf("Out.\n");

    return 0;
}