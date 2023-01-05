// Covei Denis - 312CA
#pragma once

#include <stdbool.h>

bool valid(int coord, int range_min, int range_max);
void swap(int *a, int *b);
int clamp(int val, int min_val, int max_val);

// decimal to ascii string
char *dtoascii(int val);
