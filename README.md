# Covei Denis - 312CA

    In order to run the program, use the 'make build' rule and execute it with
    ./image_editor

    LOAD <file>
        Upload a ppm or pgm image from a given file.
    
    SELECT <x1> <y1> <x2> <y2>
        Select pixels from width range [x1, x2) and  height range [y1, y2).
        It is not mandatory that x1 < x2 or y1 < y2. Coordinates may be
        replaced within the program.

    SELECT ALL
        Select whole image.

    HISTOGRAM <max_height> <number_of_bins>
        Display the image histogram using max_height stars and exactly
        number_of_bins bins.
        > **Note** The image must be color.

    EQUALIZE
        Equalize the image.
        The image must be grayscale.

    ROTATE <angle>
        If the whole image is selected, rotate the image. Otherwise rotate the
        current selection.
        The selection must be square in order to rotate it.

    CROP
        Crop the current selection.
        The image is automatically selected all after a succesful crop.

    APPLY <parameter>
        Apply parameter to current selection.
        Possible parameters are:
            EDGE
            SHARPEN
            BLUR
            GAUSSIAN_BLUR
        The image must be color.

    SAVE <file_name> [ascii]
        Save the current image with the file_name
        If optional argument [ascii] is present, then the image is saved in
        ascii format.
        The default format is binary.

    EXIT
        Unload the image and exit the program.

    Invalid command
        If a typed command is not found, an error message will be displayed.

