// Covei Denis - 312CA
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "types_and_defs.h"

int hash(char *key)
{
	if (!key)
		return BADKEY;

	typedef struct {
		char *key;
		int value;
	} set;

	set hash_table[] = {
		{"LOAD", LOAD}, {"SELECT", SELECT}, {"SELECT ALL", SELECT_ALL},
		{"ROTATE", ROTATE}, {"EQUALIZE", EQUALIZE}, {"CROP", CROP},
		{"APPLY", APPLY}, {"HISTOGRAM", HISTOGRAM}, {"SAVE", SAVE},
		{"EXIT", EXIT}, {"P2", P2}, {"P3", P3}, {"P5", P5}, {"P6", P6},
		{"EDGE", EDGE}, {"SHARPEN", SHARPEN}, {"BLUR", BLUR},
		{"GAUSSIAN_BLUR", GAUSSIAN_BLUR}
	};

	int count = sizeof(hash_table) / sizeof(set);

	for (int i = 0; i < count; i++) {
		set curr = hash_table[i];

		if (strcmp(curr.key, key) == 0)
			return curr.value;
	}

	return BADKEY;
}

void get_command(char command[], char arg[])
{
	scanf("%s", command);

	char next = getchar();
	if (next == ' ') {
		char c = getchar();
		ungetc(c, stdin);

		if (isupper(c) || c == '_') {
			char buff[BUFF_SIZE];
			scanf("%s", buff);
			strcat(command, " ");
			strcat(command, buff);
		}
	} else {
		ungetc(next, stdin);
		strcpy(arg, "NO_ARG");
		return;
	}

	if (strstr(command, "APPLY")) {
		if (strchr(command, ' ')) {
			strcpy(arg, strchr(command, ' ') + 1);
			strcpy(command, "APPLY");
		} else {
			scanf("%s", arg);
		}
	}
}

bool ignore_buff(void)
{
	bool args_found = false;
	char next = getchar();

	if (isupper(next)) {
		ungetc(next, stdin);
		return args_found;
	}

	while (next != '\n') {
		next = getchar();
		args_found = true;
	}

	return args_found;
}
