// Covei Denis - 312CA
#pragma once

#include <stdio.h>
#include <stdbool.h>
#include "types_and_defs.h"

void ignore_comments(byte * cif, FILE * file);
void get_int_ascii(int *number, FILE *file);
void get_int_binary(int *number, FILE *file);
void get_byte_ascii(byte *number, FILE *file);
void get_byte_binary(byte *number, FILE *file);
void read_pgm(char *format, img *image, FILE *file);
void read_ppm(char *format, img *image, FILE *file);
void fread_buff(char buff[], int lenght, FILE *file);
void img_select(int width, int height, rct *slct, bool whl_img, bool loaded,
				bool msg);
void unload(img *image);
void load(img *image, rct *slct, bool *loaded);
int *get_frq(img image, int n_bins, int *max_frq);
void histogram(img image, bool loaded);
void equalize(img image, bool loaded);
void rotate_pgm_selection(img *image, rct *slct, int angle, bool selected_all);
void rotate_ppm_selection(img *image, rct *slct, int angle, bool selected_all);
void rotate(img *image, rct *slct, bool loaded);
void crop(img *image, rct *slct, bool loaded);
void write_kernel(double kernel[KERNEL_SIZE][KERNEL_SIZE], ...);
void get_kernel(double kernel[KERNEL_SIZE][KERNEL_SIZE], char *arg);
void apply(img image, rct slct, char arg[], bool loaded);

// write magic number, width, height, gray / color value
void write_metadata(char *magic_number, img image, FILE *file);

void write_grayscale(char *format, img image, FILE *file);
void write_color(char *format, img image, FILE *file);
void save(img image, bool loaded);
void quit(img *image, bool loaded);
