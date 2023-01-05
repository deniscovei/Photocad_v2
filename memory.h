// Covei Denis - 312CA
#pragma once

#include "types_and_defs.h"

/* funtion to check if memory allocation failed
		returns status 0 if there are no errors
		returns status 1 if there were errors found

   Avoid calls like check_memory_allocation(*value) where value is a pointer!
   Dereferencing a NULL pointer can lead to segfault!*/
int check_memory_allocation(void *ptr);

void free_pgm_raster(byte **raster, int n_rows);
void free_ppm_raster(rgb **raster, int n_rows);
byte **malloc_pgm_raster(int width, int height);
rgb **malloc_ppm_raster(int width, int height);
byte **copy_pgm_raster(byte **raster, rct slct);
rgb **copy_ppm_raster(rgb **raster, rct slct);
