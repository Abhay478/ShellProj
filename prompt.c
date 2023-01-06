#include "osh.h"

// extern int sarg;
int spaces() {
    int c;
    while((c = getchar()) == ' ');
    if (c == '\n') return -1;
    else return c;
}

// responsible for printing PS1 and retrieveing command. 
void prompt(char ** cmd) {
    fflush(stdin);
    fflush(stdout);
    printf("osh > ");
    fflush(stdout); // cleanliness

    *cmd = malloc(1);

    int c = getchar(), x = -1;
    for(int i = 0; c != '\n'; i++) {
        if(c == ' ')
            if((x = spaces()) == -1) break; 

        // Potential site of tab-completion.

        if(i % (N_ARGS * ARG_LEN) == 0) *cmd = realloc(*cmd, (i + N_ARGS * ARG_LEN) * sizeof(char));
        (*cmd)[i] = (char)c;

        if(x != -1) {c = x; x = -1;}
        else c = getchar();
    }

    fflush(stdin);
    fflush(stdout);
}
