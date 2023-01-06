#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#define N_ARGS 10
#define ARG_LEN 20

void set_path(char ** arg);

int env(char * arg);
void v_env(char ** argv);
void what_args(char ** arg);

void trim(char ** cmd);
void prompt(char ** cmd);

char ** parse(char * cmd, int * bg);

void bb_wait();
void spawn(char ** arg, int bg);