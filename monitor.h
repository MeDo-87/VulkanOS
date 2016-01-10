#ifndef MONITOR_H_
#define MONITOR_H_
#include "common.h"

/* Hardware text mode color constants. */
enum DisplayColour {
	BLACK = 0,
	BLUE = 1,
	GREEN = 2,
	CYAN = 3,
	RED = 4,
	MAGENTA = 5,
	BROWN = 6,
	LIGHT_GREY = 7,
	DARK_GREY = 8,
	LIGHT_BLUE = 9,
	LIGHT_GREEN = 10,
	LIGHT_CYAN = 11,
	LIGHT_RED = 12,
	LIGHT_MAGENTA = 13,
	LIGHT_BROWN = 14,
	WHITE = 15,
};

void SetColour(enum DisplayColour foreground, enum DisplayColour backGround);

//Write a single character out to the screen
void putc(char c);

//Clear the screen
void clear();

//output a null-terminated ASCII string
void putString(char *c);

void WriteHex(UInt32 n);

void WriteDec(UInt32 n);

#endif
