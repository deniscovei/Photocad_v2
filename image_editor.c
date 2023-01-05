// Covei Denis - 312CA
#include <stdio.h>
#include <stdbool.h>
#include "memory.h"
#include "commands_read.h"
#include "commands.h"

int main(void)
{
	bool loaded = false;
	img image;
	rct slct;

	while (1) {
		char command[BUFF_SIZE];
		char arg[STRING_SIZE];
		get_command(command, arg);

		switch (hash(command)) {
		case LOAD:
			load(&image, &slct, &loaded);
			break;
		case SELECT:
			img_select(image.width, image.height, &slct, false, loaded, true);
			break;
		case SELECT_ALL:
			img_select(image.width, image.height, &slct, true, loaded, true);
			break;
		case HISTOGRAM:
			histogram(image, loaded);
			break;
		case EQUALIZE:
			equalize(image, loaded);
			break;
		case ROTATE:
			rotate(&image, &slct, loaded);
			break;
		case CROP:
			crop(&image, &slct, loaded);
			break;
		case APPLY:
			apply(image, slct, arg, loaded);
			break;
		case SAVE:
			save(image, loaded);
			break;
		case EXIT:
			quit(&image, loaded);
			return 0;
		default:
			printf("Invalid command\n");
		}

		if (check_memory_allocation((void *)1))
		    quit(&image, loaded);
	}

	return 0;
}
