/****
 * DIY shell
*/

#include "osh.h"
History * his;

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
    // static char ** bang;
    prompt(cmd);
    if(!strlen(*cmd)) return 1; // newline only
    if(!strcmp(*cmd, "exit")) return -1;
    if(!strcmp(*cmd, "lore")) {
        lore(his);
        printf("\n");
        return 1;
    }

    int bg = 0;

    Exec * the = build_exec(cmd, bg);
    if(!the) return 1;
    
    what_exec(the);

    spawn(the);
    if(!check_env("caret")) push(his, the);
    // what_exec(the);

    
    return 1;
} 

int main(int argc, char * argv[]) {
    char * cmd;
    make_env();
    v_env(++argv); // environment variables

    his = init();

    while(go(&cmd) != -1) if(check_env("mem")) free(cmd);
    
    if(check_env("mem")) free(cmd);
    if(check_env("dbg")) printf("Out.\n");

    return 0;
}
