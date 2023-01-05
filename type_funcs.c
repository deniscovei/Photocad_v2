// Covei Denis - 312CA
#include <stdbool.h>
#include "types_and_defs.h"

bool valid(int coord, int range_min, int range_max)
{
	return (range_min <= coord && coord <= range_max) ? true : false;
}

void swap(int *a, int *b)
{
	int aux = *a;
	*a = *b;
	*b = aux;
}

int clamp(int val, int min_val, int max_val)
{
	if (val < min_val)
		return min_val;

	if (val > max_val)
		return max_val;

	return val;
}

char *dtoascii(int val)
{
	int count = 0;
	int cif[MAX_CIF];

	do {
		cif[count++] = val % 10;
		val /= 10;
	} while (val);

	char *num = (char *)malloc((count + 1) * sizeof(char));
	for (int i = 0, j = count - 1; j >= 0; i++, j--)
		num[i] = cif[j] + '0';

	num[count] = '\0';

	return num;
}
