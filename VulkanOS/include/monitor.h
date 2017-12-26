#ifndef MONITOR_H_
#define MONITOR_H_
#include "common.h"


namespace stdio
{

class Monitor
{
	public:
Monitor();

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
	WHITE = 15
};

static void SetColour(enum DisplayColour foreground, enum DisplayColour backGround);

//Write a single character out to the screen
static void putc(char c);

//Clear the screen
static void clear();

//output a null-terminated ASCII string
static void putString(char *c);
static void putString(const char *c);

static void WriteHex(UInt32 n);

static void WriteDec(UInt32 n);
Monitor operator<< (const char* c)
{
	putString(c);
	return *this;
}

Monitor operator<< (const int c)
{
	WriteDec(c);
	return *this;
}

private:
static void moveCursor();
static void scroll();

};
}
#endif
