// Covei Denis - 312CA
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>
#include <stdarg.h>
#include "type_funcs.h"
#include "types_and_defs.h"
#include "memory.h"
#include "commands_read.h"

void ignore_comments(byte *cif, FILE *file)
{
	while (*cif == '#') {
		while (*cif != '\n')
			fread(cif, sizeof(byte), 1, file);

		fread(cif, sizeof(byte), 1, file);
	}
}

void get_int_ascii(int *number, FILE *file)
{
	byte cif;
	*number = 0;

	fread(&cif, sizeof(byte), 1, file);
	ignore_comments(&cif, file);
	while (!isdigit(cif))
		fread(&cif, sizeof(byte), 1, file);

	while (isdigit(cif)) {
		if (isdigit(cif))
			*number = (*number) * 10 + cif - '0';

		fread(&cif, sizeof(byte), 1, file);
		ignore_comments(&cif, file);
	}
}

void get_int_binary(int *number, FILE *file)
{
	byte cif = 0;
	fread(&cif, sizeof(byte), 1, file);
	*number = (int)cif;
}

void get_byte_ascii(byte *number, FILE *file)
{
	byte cif;
	*number = 0;

	fread(&cif, sizeof(byte), 1, file);
	ignore_comments(&cif, file);
	while (!isdigit(cif))
		fread(&cif, sizeof(byte), 1, file);

	while (isdigit(cif)) {
		if (isdigit(cif))
			*number = (*number) * 10 + cif - '0';

		fread(&cif, sizeof(byte), 1, file);
		ignore_comments(&cif, file);
	}
}

void get_byte_binary(byte *number, FILE *file)
{
	byte cif = 0;
	fread(&cif, sizeof(byte), 1, file);
	*number = cif;
}

// FORMAT_TYPE - ascii or binary
#define GET_RGB_TMP(FORMAT_TYPE) \
void get_rgb_##FORMAT_TYPE(rgb * pixel, FILE *file) \
{ \
	get_byte_##FORMAT_TYPE(&pixel->red, file); \
	get_byte_##FORMAT_TYPE(&pixel->green, file); \
	get_byte_##FORMAT_TYPE(&pixel->blue, file); \
}

GET_RGB_TMP(ascii)
GET_RGB_TMP(binary)

void read_pgm(char *format, img * image, FILE *file)
{
	int width = image->width;
	int height = image->height;

	image->pgm.raster = malloc_pgm_raster(width, height);
	byte **raster = image->pgm.raster;

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (!strcmp(format, "ascii"))
				get_byte_ascii(&raster[i][j], file);
			else if (!strcmp(format, "binary"))
				get_byte_binary(&raster[i][j], file);
		}
	}
}

void read_ppm(char *format, img *image, FILE *file)
{
	int width = image->width;
	int height = image->height;

	image->ppm.raster = malloc_ppm_raster(width, height);
	rgb **raster = image->ppm.raster;

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (!strcmp(format, "ascii"))
				get_rgb_ascii(&raster[i][j], file);
			else if (!strcmp(format, "binary"))
				get_rgb_binary(&raster[i][j], file);
		}
	}
}

void fread_buff(char buff[], int lenght, FILE *file)
{
	byte cif;
	fread(&cif, sizeof(byte), 1, file);
	ignore_comments(&cif, file);
	fseek(file, -1, SEEK_CUR);
	fread(buff, sizeof(char), lenght, file);
}

void img_select(int width, int height, rct *slct, bool whl_img, bool loaded,
				bool msg)
{
	int x1, x2, y1, y2;

	// if the whole image is selected
	if (whl_img) {
		if (!loaded && msg) {
			if (msg)
				printf("No image loaded\n");
			return;
		}

		// upper left corner
		x1 = 0;
		y1 = 0;

		// lower right corner
		x2 = width;
		y2 = height;

		// succesful selection
		*slct = (rct){ {x1, y1}, {x2, y2} };
		if (msg)
			printf("Selected ALL\n");

	// if selection is given by coordinates
	} else {
		// read coordinates
		int argument_count = scanf("%d%d%d%d", &x1, &y1, &x2, &y2);

		// if there were not enough coordinates given
		if (ignore_buff() || argument_count != SELECT_ARGS_COUNT) {
			printf("Invalid command\n");
			return;
		}

		if (!loaded && msg) {
			if (msg)
				printf("No image loaded\n");
			return;
		}

		if (x1 > x2)
			swap(&x1, &x2);

		if (y1 > y2)
			swap(&y1, &y2);

		// if coordinates are within image dimensions
		if (valid(x1, 0, width) && valid(x2, 0, width) &&
			valid(y1, 0, height) && valid(y2, 0, height) &&
			x1 != x2 && y1 != y2) {
			*slct = (rct){ {x1, y1}, {x2, y2} };

			// succesful selection
			if (msg)
				printf("Selected %d %d %d %d\n", x1, y1, x2, y2);
		} else {
			// unsuccesful selection
			if (msg)
				printf("Invalid set of coordinates\n");
		}
	}
}

void unload(img *image)
{
	int height = image->height;
	char *type = image->type;

	if (!strcmp(type, "pgm")) {
		for (int i = 0; i < height; i++)
			free(image->pgm.raster[i]);

		free(image->pgm.raster);
	}

	if (!strcmp(type, "ppm")) {
		for (int i = 0; i < height; i++)
			free(image->ppm.raster[i]);

		free(image->ppm.raster);
	}
}

void load(img *image, rct *slct, bool *loaded)
{
	char buff[BUFF_SIZE];

	char filename[BUFF_SIZE];
	scanf("%s", filename);

	// if there is already a loaded image, unload it in order to load a new one
	if (*loaded) {
		unload(image);
		*loaded = false;
	}

	// open image file
	FILE *image_file = fopen(filename, "rb");
	if (!image_file) {
		printf("Failed to load %s\n", filename);
		return;
	}

	*loaded = true;

	// read magic number
	fread_buff(buff, 3, image_file);

	char *magic_number = strndup(buff, 2);
	if (check_memory_allocation(magic_number))
		return;

	// get width
	get_int_ascii(&image->width, image_file);

	// get height
	get_int_ascii(&image->height, image_file);

	// get gray intensity or color intensity
	val_size tmp_value;
	get_byte_ascii(&tmp_value, image_file);

	switch (hash(magic_number)) {
		// grayscale ascii
	case P2:
		image->type = "pgm";
		image->pgm.gray_value = tmp_value;
		read_pgm("ascii", image, image_file);
		break;

		// color ascii
	case P3:
		image->type = "ppm";
		image->ppm.color_value = tmp_value;
		read_ppm("ascii", image, image_file);
		break;

		// grayscale binary
	case P5:
		image->type = "pgm";
		image->pgm.gray_value = tmp_value;
		read_pgm("binary", image, image_file);
		break;

		// color binary
	case P6:
		image->type = "ppm";
		image->ppm.color_value = tmp_value;
		read_ppm("binary", image, image_file);
		break;
	}

	// close image file
	fclose(image_file);

	// release resources
	free(magic_number);

	// automatically select the whole image if loaded
	img_select(image->width, image->height, slct, true, loaded, false);

	// succesful load
	printf("Loaded %s\n", filename);
}

int *get_frq(img image, int n_bins, int *max_frq)
{
	byte **raster = image.pgm.raster;
	int width = image.width;
	int height = image.height;

	// frequency vector to store pixels' frequencies in bins
	int *frq = (int *)calloc(n_bins, sizeof(int));
	if (check_memory_allocation(frq))
		return NULL;

	// lenght of a bin interval
	int len = (image.pgm.gray_value + 1) / n_bins;
	*max_frq = 0;

	// count frquencies of each pixel value
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++)
			frq[raster[i][j] / len]++;
	}

	for (int i = 0; i < n_bins; i++) {
		if (frq[i] > (*max_frq))
			*max_frq = frq[i];
	}

	return frq;
}

void histogram(img image, bool loaded)
{
	int max_val, n_bins;
	int argument_count = scanf("%d%d", &max_val, &n_bins);

	// if there is no image loaded
	if (!loaded) {
		printf("No image loaded\n");
		return;
	}

	// if there were not enough arguments given
	if (argument_count != HISTOGRAM_ARGS_COUNT || ignore_buff()) {
		printf("Invalid command\n");
		return;
	}

	// if the image is color
	if (!strcmp(image.type, "ppm")) {
		printf("Black and white image needed\n");
		return;
	}

	// histogram
	int max_frq;
	int *frq = get_frq(image, n_bins, &max_frq);

	// display hsitogram
	for (int i = 0; i < n_bins; i++) {
		// star count
		int count = 1.0 * frq[i] / max_frq * max_val;

		// print number of stars
		printf("%d\t|\t", count);

		// print stars
		for (int j = 0; j < count; j++)
			printf("*");

		printf("\n");
	}

	free(frq);
}

void equalize(img image, bool loaded)
{
	// if there is no image loaded
	if (!loaded) {
		printf("No image loaded\n");
		return;
	}

	// if the image is color
	if (!strcmp(image.type, "ppm")) {
		printf("Black and white image needed\n");
		return;
	}

	byte **raster = image.pgm.raster;

	int width = image.width;
	int height = image.height;
	const int area = width * height;

	int not_needed; // used as argument for calling get_frq
	int grval = image.pgm.gray_value;
	int *frq = get_frq(image, grval + 1, &not_needed);
	int *sum = (int *)calloc(grval + 1, sizeof(int));

	// partial sums
	sum[0] = frq[0];
	for (int i = 1; i <= grval; i++)
		sum[i] = sum[i - 1] + frq[i];

	// equalize image
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			raster[i][j] =
				clamp((int)round(255.0 * sum[raster[i][j]] / area), 0, 255);
		}
	}

	// if image was succesfully equalized
	printf("Equalize done\n");

	free(frq);
	free(sum);
}

void rotate_pgm_selection(img *image, rct *slct, int angle, bool selected_all)
{
	byte **slct_raster =
		copy_pgm_raster(image->pgm.raster, *slct);
	byte **curr_raster = image->pgm.raster;
	byte **new_raster;

	int curr_height = image->height;

	int i_begin, i_end, j_begin, j_end;

	// mark rotation area
	if (selected_all && (angle == 90 || angle == 270)) {
		new_raster = (byte **)
			malloc_pgm_raster(image->height, image->width);

		i_begin = 0;
		i_end = image->width;

		j_begin = 0;
		j_end = image->height;
	} else {
		new_raster = image->pgm.raster;

		i_begin = slct->upr.y;
		i_end = slct->lwr.y;

		j_begin = slct->upr.x;
		j_end = slct->lwr.x;
	}

	// rotate selection
	switch (angle) {
	case 0:
		break;
	case 90:
		for (int i_slct = 0, j = j_end - 1; j >= j_begin; i_slct++, j--) {
			for (int j_slct = 0, i = i_begin; i < i_end; j_slct++, i++)
				new_raster[i][j] = slct_raster[i_slct][j_slct];
		}
		break;
	case 180:
		for (int i_slct = 0, i = i_end - 1; i >= i_begin; i_slct++, i--) {
			for (int j_slct = 0, j = j_end - 1; j >= j_begin; j_slct++, j--)
				new_raster[i][j] = slct_raster[i_slct][j_slct];
		}
		break;
	case 270:
		for (int i_slct = 0, j = j_begin; j < j_end; i_slct++, j++) {
			for (int j_slct = 0, i = i_end - 1; i >= i_begin; j_slct++, i--)
				new_raster[i][j] = slct_raster[i_slct][j_slct];
		}
		break;
	}

	free_pgm_raster(slct_raster, slct->lwr.y - slct->upr.y);

	// if the whole image was selected swap its witdh and height
	if (selected_all && (angle == 90 || angle == 270)) {
		free_pgm_raster(curr_raster, curr_height);
		swap(&image->width, &image->height);
		image->pgm.raster = new_raster;
		img_select(image->width, image->height, slct, true, true, false);
	}
}

void rotate_ppm_selection(img *image, rct *slct, int angle, bool selected_all)
{
	rgb **slct_raster =
		copy_ppm_raster(image->ppm.raster, *slct);
	rgb **curr_raster = image->ppm.raster;
	rgb **new_raster;

	int curr_height = image->height;

	int i_begin, i_end, j_begin, j_end;

	// mark rotation area
	if (selected_all && (angle == 90 || angle == 270)) {
		new_raster = (rgb **)
			malloc_ppm_raster(image->height, image->width);

		i_begin = 0;
		i_end = image->width;

		j_begin = 0;
		j_end = image->height;
	} else {
		new_raster = image->ppm.raster;

		i_begin = slct->upr.y;
		i_end = slct->lwr.y;

		j_begin = slct->upr.x;
		j_end = slct->lwr.x;
	}

	// rotate selection
	switch (angle) {
	case 0:
		break;
	case 90:
		for (int i_slct = 0, j = j_end - 1; j >= j_begin; i_slct++, j--) {
			for (int j_slct = 0, i = i_begin; i < i_end; j_slct++, i++)
				new_raster[i][j] = slct_raster[i_slct][j_slct];
		}
		break;
	case 180:
		for (int i_slct = 0, i = i_end - 1; i >= i_begin; i_slct++, i--) {
			for (int j_slct = 0, j = j_end - 1; j >= j_begin; j_slct++, j--)
				new_raster[i][j] = slct_raster[i_slct][j_slct];
		}
		break;
	case 270:
		for (int i_slct = 0, j = j_begin; j < j_end; i_slct++, j++) {
			for (int j_slct = 0, i = i_end - 1; i >= i_begin; j_slct++, i--)
				new_raster[i][j] = slct_raster[i_slct][j_slct];
		}
		break;
	}

	free_ppm_raster(slct_raster, slct->lwr.y - slct->upr.y);

	// if the whole image was selected swap its witdh and height
	if (selected_all && (angle == 90 || angle == 270)) {
		free_ppm_raster(curr_raster, curr_height);
		swap(&image->width, &image->height);
		image->ppm.raster = new_raster;
		img_select(image->width, image->height, slct, true, true, false);
	}
}

void rotate(img *image, rct *slct, bool loaded)
{
	int angle;
	scanf("%d", &angle);

	// if there is no image loaded
	if (!loaded) {
		printf("No image loaded\n");
		return;
	}

	// if the selection cannot be rotated
	if (angle % RIGHT_ANGLE != 0) {
		printf("Unsupported rotation angle\n");
		return;
	}

	bool selected_all = (slct->lwr.y - slct->upr.y == image->height &&
		slct->lwr.x - slct->upr.y == image->width) ? true : false;

	// if the image is not selected all and the selection is not sqare
	if (slct->lwr.y - slct->upr.y != slct->lwr.x - slct->upr.x &&
		!selected_all) {
		printf("The selection must be square\n");
		return;
	}

	// succesful rotation
	printf("Rotated %d\n", angle);

	// restrict the angle within the [0, 360) degrees interval
	angle %= COMPLETE_ANGLE;
	if (angle < 0)
		angle += COMPLETE_ANGLE;

	if (!strcmp(image->type, "ppm"))
		rotate_ppm_selection(image, slct, angle, selected_all);

	if (!strcmp(image->type, "pgm"))
		rotate_pgm_selection(image, slct, angle, selected_all);
}

void crop(img *image, rct *slct, bool loaded)
{
	// if there is no image loaded
	if (!loaded) {
		printf("No image loaded\n");
		return;
	}

	int curr_height = image->height;

	int new_width = slct->lwr.x - slct->upr.x;
	int new_height = slct->lwr.y - slct->upr.y;

	int i_slct = slct->upr.y;
	int j_slct = slct->upr.x;

	// grayscale image
	if (!strcmp(image->type, "pgm")) {
		byte **curr_raster = image->pgm.raster;
		byte **new_raster = malloc_pgm_raster(new_width, new_height);

		// get cropped matrix's pixels
		for (int i = 0; i < new_height; i++) {
			for (int j = 0; j < new_width; j++)
				new_raster[i][j] = curr_raster[i + i_slct][j + j_slct];
		}

		// release resources for old raster
		free_pgm_raster(curr_raster, curr_height);

		// replace current raster with the cropped one
		image->pgm.raster = new_raster;
	}

	// color image
	if (!strcmp(image->type, "ppm")) {
		rgb **curr_raster = image->ppm.raster;
		rgb **new_raster = malloc_ppm_raster(new_width, new_height);

		// get cropped matrix's pixels
		for (int i = 0; i < new_height; i++) {
			for (int j = 0; j < new_width; j++)
				new_raster[i][j] = curr_raster[i + i_slct][j + j_slct];
		}

		// release resources for old raster
		free_ppm_raster(curr_raster, curr_height);

		// replace current raster with the cropped one
		image->ppm.raster = new_raster;
	}

	// replace current metadata with the cropped image's metadata
	image->width = new_width;
	image->height = new_height;

	// automatically select the whole image if cropped
	img_select(image->width, image->height, slct, true, loaded, false);

	// succesful crop
	printf("Image cropped\n");
}

void write_kernel(double kernel[KERNEL_SIZE][KERNEL_SIZE], ...)
{
	va_list ptr;
	va_start(ptr, kernel);

	for (int i = 0; i < KERNEL_SIZE; i++) {
		for (int j = 0; j < KERNEL_SIZE; j++)
			kernel[i][j] = va_arg(ptr, double);
	}

	va_end(ptr);
}

void get_kernel(double kernel[KERNEL_SIZE][KERNEL_SIZE], char *arg)
{
	double coef = 1.0;
	switch (hash(arg)) {
	case EDGE:
		write_kernel(kernel,
					 coef * -1, coef * -1, coef * -1,
					 coef * -1, coef * 8, coef * -1,
					 coef * -1, coef * -1, coef * -1);
		break;
	case SHARPEN:
		write_kernel(kernel,
					 coef * 0, coef * -1, coef * 0,
					 coef * -1, coef * 5, coef * -1,
					 coef * 0, coef * -1, coef * 0);
		break;
	case BLUR:
		coef = 1.0 * 1 / 9;
		write_kernel(kernel,
					 coef * 1, coef * 1, coef * 1,
					 coef * 1, coef * 1, coef * 1,
					 coef * 1, coef * 1, coef * 1);
		break;
	case GAUSSIAN_BLUR:
		coef = 1.0 * 1 / 16;
		write_kernel(kernel,
					 coef * 1, coef * 2, coef * 1,
					 coef * 2, coef * 4, coef * 2,
					 coef * 1, coef * 2, coef * 1);
		break;
	}
}

void apply(img image, rct slct, char arg[], bool loaded)
{
	// if there is no image loaded
	if (!loaded) {
		printf("No image loaded\n");
		return;
	}

	// if there was no parameter given
	if (!strcmp(arg, "NO_ARG")) {
		printf("Invalid command\n");
		return;
	}

	// if the given image is grayscale
	if (!strcmp(image.type, "pgm")) {
		printf("Easy, Charlie Chaplin\n");
		return;
	}

	double kernel[KERNEL_SIZE][KERNEL_SIZE];
	switch (hash(arg)) {
	case EDGE:
	case SHARPEN:
	case BLUR:
	case GAUSSIAN_BLUR:
		get_kernel(kernel, arg);
		break;
	case BADKEY:
		printf("APPLY parameter invalid\n");
		return;
	}

	int width = image.width;
	int height = image.height;

	int i_begin = (slct.upr.y == 0) ? 1 : slct.upr.y;
	int i_end = (slct.lwr.y == height) ? height - 1 : slct.lwr.y;

	int j_begin = (slct.upr.x == 0) ? 1 : slct.upr.x;
	int j_end = (slct.lwr.x == width) ? width - 1 : slct.lwr.x;

	// succesful apply
	printf("APPLY %s done\n", arg);

	// if selection to apply is void
	if (i_begin >= i_end - 1 || j_begin >= j_end - 1)
		return;

	rct cp_slct = (rct){ {j_begin - 1, i_begin - 1}, {j_end + 1, i_end + 1} };
	rgb **slct_raster = copy_ppm_raster(image.ppm.raster, cp_slct);

	for (int i = i_begin, i_slct = 1; i < i_end; i++, i_slct++) {
		for (int j = j_begin, j_slct = 1; j < j_end; j++, j_slct++) {
			// declaring new pixel's channels
			double red = 0, green = 0, blue = 0;

			// calculate the new pixel using its neighbours
			for (int i_ngh = -1, i_krn = 0; i_ngh <= 1; i_ngh++, i_krn++) {
				for (int j_ngh = -1, j_krn = 0; j_ngh <= 1; j_ngh++, j_krn++) {
					// red channel
					red += kernel[i_krn][j_krn] *
						slct_raster[i_slct + i_ngh][j_slct + j_ngh].red;

					// green channel
					green += kernel[i_krn][j_krn] *
						slct_raster[i_slct + i_ngh][j_slct + j_ngh].green;

					// blue channel
					blue += kernel[i_krn][j_krn] *
						slct_raster[i_slct + i_ngh][j_slct + j_ngh].blue;
				}
			}

			// replace the new pixel
			image.ppm.raster[i][j].red = (byte)clamp(round(red), 0, 255);
			image.ppm.raster[i][j].green = (byte)clamp(round(green), 0, 255);
			image.ppm.raster[i][j].blue = (byte)clamp(round(blue), 0, 255);
		}
	}
	free_ppm_raster(slct_raster, i_end - i_begin + 2);
}

void write_metadata(char *magic_number, img image, FILE *file)
{
	// write magic number
	fwrite(magic_number, sizeof(char), strlen(magic_number), file);
	fwrite("\n", sizeof(char), 1, file);

	// write width
	char *width = dtoascii(image.width);
	fwrite(width, sizeof(char), strlen(width), file);
	fwrite(" ", sizeof(char), 1, file);
	free(width);

	// write height
	char *height = dtoascii(image.height);
	fwrite(height, sizeof(char), strlen(height), file);
	fwrite("\n", sizeof(char), 1, file);
	free(height);

	// write gray value
	char *grval = dtoascii(image.pgm.gray_value);
	fwrite(grval, sizeof(char), strlen(grval), file);
	fwrite("\n", sizeof(char), 1, file);
	free(grval);
}

void write_grayscale(char *format, img image, FILE *file)
{
	if (!strcmp(format, "ascii"))
		write_metadata("P2", image, file);

	if (!strcmp(format, "binary"))
		write_metadata("P5", image, file);

	int width = image.width;
	int height = image.height;
	byte **raster = image.pgm.raster;

	// ascii format
	if (!strcmp(format, "ascii")) {
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				char *pxl = dtoascii((int)raster[i][j]);
				fwrite(pxl, sizeof(char), strlen(pxl), file);
				free(pxl);
				fwrite(" ", sizeof(char), 1, file);
			}
			fwrite("\n", sizeof(char), 1, file);
		}
	}

	// binary format
	if (!strcmp(format, "binary")) {
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++)
				fwrite((byte *)&raster[i][j], sizeof(byte), 1, file);
		}
	}
}

void write_color(char *format, img image, FILE *file)
{
	if (!strcmp(format, "ascii"))
		write_metadata("P3", image, file);

	if (!strcmp(format, "binary"))
		write_metadata("P6", image, file);

	int width = image.width;
	int height = image.height;
	rgb **raster = image.ppm.raster;

	// ascii format
	if (!strcmp(format, "ascii")) {
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				char *red = dtoascii((int)raster[i][j].red);
				fwrite(red, sizeof(char), strlen(red), file);
				fwrite(" ", sizeof(char), 1, file);
				free(red);

				char *green = dtoascii((int)raster[i][j].green);
				fwrite(green, sizeof(char), strlen(green), file);
				fwrite(" ", sizeof(char), 1, file);
				free(green);

				char *blue = dtoascii((int)raster[i][j].blue);
				fwrite(blue, sizeof(char), strlen(blue), file);
				free(blue);

				//if (j != height - 1) {
				fwrite(" ", sizeof(char), 1, file);
				//}
			}
			fwrite("\n", sizeof(char), 1, file);
		}
	}

	// binary format
	if (!strcmp(format, "binary")) {
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				char red = (char)raster[i][j].red;
				fwrite(&red, sizeof(char), 1, file);

				char green = (char)raster[i][j].green;
				fwrite(&green, sizeof(char), 1, file);

				char blue = (char)raster[i][j].blue;
				fwrite(&blue, sizeof(char), 1, file);
			}
		}
	}
}

void save(img image, bool loaded)
{
	char filename[BUFF_SIZE];
	scanf("%s", filename);

	char save_as[STRING_SIZE] = "binary";
	char next = getchar();
	if (next == ' ')
		next = getchar();

	if (next != '\n') {
		ungetc(next, stdin);
		scanf("%s", save_as);
	}

	// if there is no image loaded
	if (!loaded) {
		printf("No image loaded\n");
		return;
	}

	FILE *image_file = fopen(filename, "wb");
	if (!image_file) {
		printf("Cannot open file!\n");
		return;
	}

	char *type = image.type;

	if (!strcmp(save_as, "ascii")) {
		// grayscale ascii P2
		if (!strcmp(type, "pgm"))
			write_grayscale("ascii", image, image_file);

		// color ascii P3
		if (!strcmp(type, "ppm"))
			write_color("ascii", image, image_file);
	}

	if (!strcmp(save_as, "binary")) {
		// grayscale binary P5
		if (!strcmp(type, "pgm"))
			write_grayscale("binary", image, image_file);

		// color binary P6
		if (!strcmp(type, "ppm"))
			write_color("binary", image, image_file);
	}

	fclose(image_file);

	// succesful save
	printf("Saved %s\n", filename);
}

void quit(img *image, bool loaded)
{
	// if there is no image loaded
	if (!loaded) {
		printf("No image loaded\n");
		return;
	}

	unload(image);
}
