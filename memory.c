// Covei Denis - 312CA
#include <stdlib.h>
#include <stdio.h>
#include "types_and_defs.h"

int check_memory_allocation(void *ptr)
{
	// status is initially 0 (by default), so we do not have errors
	static int status;
	if (!ptr) {
		printf("Could not allocate memory\n");
		status = 1;
	}
	return status;
}

void free_pgm_raster(byte **raster, int n_rows)
{
	for (int i = 0; i < n_rows; i++)
		free(raster[i]);

	free(raster);
}

void free_ppm_raster(rgb **raster, int n_rows)
{
	for (int i = 0; i < n_rows; i++)
		free(raster[i]);

	free(raster);
}

byte **malloc_pgm_raster(int width, int height)
{
	byte **raster = malloc(height * sizeof(byte *));
	if (check_memory_allocation(raster))
		return NULL;

	for (int i = 0; i < height; i++) {
		raster[i] = (byte *)malloc(width * sizeof(byte));

		if (check_memory_allocation(raster[i])) {
			free_pgm_raster(raster, i - 1);
			return NULL;
		}
	}

	return raster;
}

rgb **malloc_ppm_raster(int width, int height)
{
	rgb **raster = malloc(height * sizeof(rgb *));
	if (check_memory_allocation(raster))
		return NULL;

	for (int i = 0; i < height; i++) {
		raster[i] = (rgb *)malloc(width * sizeof(rgb));

		if (check_memory_allocation(raster[i])) {
			free_ppm_raster(raster, i - 1);
			return NULL;
		}
	}

	return raster;
}

byte **copy_pgm_raster(byte **raster, rct slct)
{
	int copy_width = slct.lwr.x - slct.upr.x;
	int copy_height = slct.lwr.y - slct.upr.y;

	byte **copy_raster = malloc_pgm_raster(copy_width, copy_height);

	int i_slct = slct.upr.y;
	int j_slct = slct.upr.x;

	for (int i = 0; i < copy_height; i++) {
		for (int j = 0; j < copy_width; j++)
			copy_raster[i][j] = raster[i + i_slct][j + j_slct];
	}

	return copy_raster;
}

rgb **copy_ppm_raster(rgb **raster, rct slct)
{
	int copy_width = slct.lwr.x - slct.upr.x;
	int copy_height = slct.lwr.y - slct.upr.y;

	rgb **copy_raster = malloc_ppm_raster(copy_width, copy_height);

	int i_slct = slct.upr.y;
	int j_slct = slct.upr.x;

	for (int i = 0; i < copy_height; i++) {
		for (int j = 0; j < copy_width; j++)
			copy_raster[i][j] = raster[i + i_slct][j + j_slct];
	}

	return copy_raster;
}
