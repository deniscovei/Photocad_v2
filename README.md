# Image editor CLI - *Covei Denis - 312CA*

In order to run the program, use the **make build** rule and execute it with
**./image_editor** in terminal.

## Commands

**LOAD &nbsp;&nbsp; \<file\>**\
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
**number_of_bins** bins.
> **Note:** The image must be color.

\
**EQUALIZE**\
Equalize the image.
> **Note:** The image must be grayscale.

\
**ROTATE \<angle\>**\
If the whole image is selected, rotate the image. Otherwise rotate the
current selection.
> **Note:** The selection must be square in order to rotate it.

\
**CROP**\
Crop the current selection.
The image is automatically selected all after a succesful crop.

\
**APPLY \<parameter\>**\
Apply an effect to current selection.
The possible effects given as **\<parameter\>** are:
* **EDGE**
* **SHARPEN**
* **BLUR**
* **GAUSSIAN_BLUR**
> **Note:** The image must be color.

\
**SAVE \<file_name\> [ascii]**\
Save the current image with the file_name
If optional argument **[ascii]** is present, then the image is saved in
ascii format.
The default format is **binary**.

\
**EXIT**\
Unload the image and exit the program.

\
**Invalid command**\
If a typed command is not found, an error message will be displayed.

