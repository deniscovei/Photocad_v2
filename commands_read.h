// Covei Denis - 312CA
#pragma once

// hash funciton for storing commands' names
int hash(char *key);

void get_command(char command[], char arg[]);

// checks if there were more arguments given to a command
bool ignore_buff(void);
