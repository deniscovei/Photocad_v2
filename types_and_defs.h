// Covei Denis - 312CA
#pragma once

#define BUFF_SIZE 100000
#define STRING_SIZE 20
#define MAX_CIF 20
#define KERNEL_SIZE 3
#define RIGHT_ANGLE 90
#define COMPLETE_ANGLE 360
#define HISTOGRAM_ARGS_COUNT 2
#define SELECT_ARGS_COUNT 4
typedef unsigned char byte;
typedef unsigned char val_size;

// grayscale image characteristics
typedef struct {
	val_size gray_value;
	byte **raster;
} pgm_t;

// rgb pixel type
typedef struct {
	byte red, green, blue;
} rgb;

// color image characteristics
typedef struct {
	val_size color_value;
	rgb **raster;
} ppm_t;

// image
typedef struct {
	char *type; // pgm or ppm
	int width, height;
	union {
		pgm_t pgm;
		ppm_t ppm;
	};
} img;

/* reactangle defined by two points:
		upr - upper left corner
		lwr - lower right corner */
typedef struct {
	struct point {
		int x, y;
	} upr, lwr;
} rct;

enum keys {
	BADKEY, LOAD, SELECT, SELECT_ALL, ROTATE, EQUALIZE, CROP, APPLY, HISTOGRAM,
	SAVE, EXIT, P2, P3, P5, P6, EDGE, SHARPEN, BLUR, GAUSSIAN_BLUR
};
