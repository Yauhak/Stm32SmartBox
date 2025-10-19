#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LispLikeCalc.h"

char rt[MAX_TOKEN_LEN] = {0};
char tokens[MAX_DEPTH][MAX_TOKENS][MAX_TOKEN_LEN] = {0};
short token_quantity[MAX_DEPTH] = {0};

void func(char param[][MAX_TOKEN_LEN], int i) {
	char func_name = param[0][0];
	double calc_value = 0;
	switch (func_name) {
		case '+':
			for (int param_index = 1; param_index < i; param_index++)
				calc_value += atof(param[param_index]);
			sprintf(rt, "%.6g", calc_value);
			break;

		case '-':
			calc_value = atof(param[1]);
			for (int param_index = 2; param_index < i; param_index++)
				calc_value -= atof(param[param_index]);
			sprintf(rt, "%.6g", calc_value);
			break;

		case '*':
			calc_value = atof(param[1]);
			for (int param_index = 2; param_index < i; param_index++)
				calc_value *= atof(param[param_index]);
			sprintf(rt, "%.6g", calc_value);
			break;

		case '/':
			calc_value = atof(param[1]);
			for (int param_index = 2; param_index < i; param_index++) {
				if (atof(param[param_index]) != 0) {
					calc_value /= atof(param[param_index]);
				} else {
					strcpy(rt, "DIV0");
					return;
				}
			}
			sprintf(rt, "%.6g", calc_value);
			break;

		case '%':
			if (i >= 3) {
				int result = atoi(param[1]) % atoi(param[2]);
				sprintf(rt, "%d", result);
			} else {
				strcpy(rt, "ND_ARGS");
			}
			break;

		case '=':
			if (i >= 3) {
				strcpy(rt, (!strcmp(param[1], param[2])) ? "1" : "0");
			} else {
				strcpy(rt, "ND_ARGS");
			}
			break;

		case '>':
			if (i >= 3) {
				strcpy(rt, (atof(param[1]) > atof(param[2])) ? "1" : "0");
			} else {
				strcpy(rt, "ND_ARGS");
			}
			break;

		case '<':
			if (i >= 3) {
				strcpy(rt, (atof(param[1]) < atof(param[2])) ? "1" : "0");
			} else {
				strcpy(rt, "ND_ARGS");
			}
			break;

		case 'g':  // >=
			if (i >= 3) {
				strcpy(rt, (atof(param[1]) >= atof(param[2])) ? "1" : "0");
			} else {
				strcpy(rt, "ND_ARGS");
			}
			break;

		case 'l':  // <=
			if (i >= 3) {
				strcpy(rt, (atof(param[1]) <= atof(param[2])) ? "1" : "0");
			} else {
				strcpy(rt, "ND_ARGS");
			}
			break;

		case 'n':
			if (i >= 3) {
				strcpy(rt, (!strcmp(param[1], param[2])) ? "0" : "1");
			} else {
				strcpy(rt, "ND_ARGS");
			}
			break;

		case '&':  // and
			if (i >= 2) {
				calc_value = atof(param[1]);
				for (int param_index = 2; param_index < i; param_index++)
					calc_value = calc_value && atof(param[param_index]);
				sprintf(rt, "%.6g", calc_value);
			} else {
				strcpy(rt, "ND_ARGS");
			}
			break;

		case '|':  // or
			if (i >= 2) {
				calc_value = atof(param[1]);
				for (int param_index = 2; param_index < i; param_index++)
					calc_value = calc_value || atof(param[param_index]);
				sprintf(rt, "%.6g", calc_value);
			} else {
				strcpy(rt, "ND_ARGS");
			}
			break;
		default:
			strcpy(rt, param[0]);
			break;
	}
}

short check_exp_valid(char *expression){
	short brackets=0;
	char *ptr=expression;
	while(*ptr){
		if(*ptr=='(')brackets++;
		if(*ptr==')')brackets--;
		ptr++;
	}
	return brackets;
}

int parse(char *expression) {
	if(check_exp_valid(expression)){
		strcpy(rt, "UMB_BRK");
		return -1;
	}
	char *ptr = expression;
	int depth = 0;
	rt[0] = '\0';
	while (*ptr != '\0' && token_quantity[depth] < MAX_TOKENS) {
		while (*ptr == ' ' || *ptr == '\t') ptr++;
		if (*ptr == '\0') break;
		if (*ptr == '(') {
			depth++;
			ptr++;
			continue;
		} else if (*ptr == ')' || *ptr == 0) {
			if (token_quantity[depth] > 0) {
				func(tokens[depth], token_quantity[depth]);
				if (depth > 0) {
					strcpy(tokens[depth - 1][token_quantity[depth - 1]], rt);
					token_quantity[depth - 1]++;
				} else {
					strcpy(rt, tokens[depth - 1][token_quantity[depth - 1]]);
				}
			}
			if (ptr)ptr++;
			if (depth > 0) {
				token_quantity[depth] = 0;
				depth--;
			}
			continue;
		} else {
			int len = 0;
			while (*ptr != ' ' && *ptr != '\t' && *ptr != '(' &&
			       *ptr != ')' && *ptr != '\0' && len < MAX_TOKEN_LEN - 1) {
				tokens[depth][token_quantity[depth]][len++] = *ptr++;
			}

			if (len > 0) {
				tokens[depth][token_quantity[depth]][len] = '\0';
				token_quantity[depth]++;
			}

			if (token_quantity[depth] >= MAX_TOKENS) {
				strcpy(rt, "TK_OUT");
				return -1;
			}
		}
	}
	return 0;
}
