#ifndef LISPLIKECALC_H
#define LISPLIKECALC_H

#define MAX_DEPTH 10
#define MAX_TOKEN_LEN 10
#define MAX_TOKENS 10

extern char rt[MAX_TOKEN_LEN];

int parse(char *expression);
void func(char param[][MAX_TOKEN_LEN], int i);

#endif