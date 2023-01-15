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

// runs in each iteration of the main loop - interactive section.
int go(char ** cmd) {
    prompt(cmd); // gets line. Unbuffered, so it ignores multiple spaces and stuff.
    if(!strlen(*cmd)) return 1; // newline only
    if(!strcmp(*cmd, "exit")) return -1; // classic exit command.
    if(!strcmp(*cmd, "lore")) { // prints all command in history. Pretty useful, imo.
        lore(his); // what I said above, but also resets the envs.
        printf("\n");
        return 1;
    }

    int bg = 0;

    // obviously, it takes a line of input and decomposes it into pipes and indirections.
    Exec * the = build_exec(cmd, bg); 
    if(!the) return 1; // boo
    
    what_exec(the);

    spawn(the); // real stuff
    strip_descriptors(the); // closes the newly opened files / pipes.
    if(!check_env("caret")) push(his, the); // pushes new commands, not ones that had been fetched from history.
    
    return 1;
} 

int main(int argc, char * argv[]) {
    char * cmd;
    make_env(); // initialises environment variables.
    v_env(++argv); // sets environment variables

    his = init(); // duh

    while(go(&cmd) != -1) if(check_env("mem")) free(cmd); // shell
    
    if(check_env("mem")) free(cmd); 
    if(check_env("dbg")) printf("Out.\n");

    return 0;
}
