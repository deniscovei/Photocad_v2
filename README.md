# Image editor CLI - *Covei Denis - 312CA*

In order to run the program, use the **make build** rule and execute it with
**./image_editor** in terminal.

## Commands

**LOAD \<file\>**\
Upload a **ppm** or **pgm** image from **\<file\>**.

\
**SELECT \<x<sub>1</sub>\> \<y<sub>1</sub>\> \<x<sub>2</sub>\> 
\<y<sub>2</sub>\>**\
Select pixels from width range [x<sub>1</sub>,
x<sub>2</sub>) and  height range [y<sub>1</sub>, y<sub>2</sub>).
> **Note:** It is not mandatory that **x<sub>1</sub> < x<sub>2</sub>**
or **y<sub>1</sub> < y<sub>2</sub>**. Coordinates may be
replaced within the program execution.

\
**SELECT ALL**\
Select the whole image.

\
**HISTOGRAM \<max_height\> \<number_of_bins\>**\
Display the image histogram using **max_height** stars and exactly
**number_of_bins** bins, i.e. intervals.

***Operation description:** A histogram is a graph showing the number of pixels in
an image at each different intensity value found in that image.*

> **Note:** The image must be grayscale.

\
**EQUALIZE**\
Equalize the image.

***Operation description:** Histogram Equalization is a computer image processing
technique used to improve contrast in images . It accomplishes this by effectively
spreading out the most frequent intensity values, i.e. stretching out the intensity
range of the image.*

> **Note:** The image must be grayscale.

\
**ROTATE \<angle\>**\
If the whole image is selected, rotate the image. Otherwise rotate the
current selection.
> **Note:** The selection must be square in order to rotate it.

\
**CROP**\
Crop the current selection.
The whole size of the image is automatically selected after a succesful crop.

\
**APPLY \<parameter\>**\
Apply an effect to current selection.
The possible effects given as **\<parameter\>** are:
* **EDGE**
* **SHARPEN**
* **BLUR**
* **GAUSSIAN_BLUR**

***Operation description:** This operation is done by using kernles, which are
small predefined matrices, each one being particularly made for a specific effect.
This is accomplished by doing a convolution between the kernel and the image.*

> **Note:** The image must be color.

\
**SAVE \<file_name\> [ascii]**\
Save the current image with the name **\<file_name\>**.\
If the optional argument **[ascii]** is present, then the image is saved in
**ascii** format.\
The default saving format is **binary**.

\
**EXIT**\
Unload the image and exit the program.

\
**Invalid command**\
If a typed command is not found, an error message will be displayed.

