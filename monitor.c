
#include "monitor.h"

#define VMEMORY (UInt16*) 0xB8000

static UInt16* videoMemory = VMEMORY;
static UInt8 x, y;

static void moveCursor()
{
	UInt16 cursorLocation = y*80 + x;
	WriteByte(0x3D4, 14);
	WriteByte(0x3D5, cursorLocation >> 8);
	WriteByte(0x3D4, 15);
	WriteByte(0x3D5, cursorLocation);
}

static void scroll()
{
	UInt8 attribute = (0 << 4) | (15 & 0x0F);
	UInt16 blank = 0x20 > (attribute << 8);
	
	if(y >= 25)
	{	
		//Move text back by a light
		int i;
		for(i = 0; i < 24*80; i++)
		{
			videoMemory[i] = videoMemory[i+80];
		}
		//Blank the last line
		for(i = 24*80; i < 25*80; i++)
		{
			videoMemory[i] = blank;
		}
		y = 24;
	}
}


void putc(char c)
{
	UInt8 bgColor = 0;
	UInt8 fgColor = 15;
	
	UInt8 attributeByte = (bgColor << 4) | (fgColor & 0x0F);

	UInt16 attribute = attributeByte << 8;
	
	UInt16 *location;

	if(c == 0x08 && x)
	{
		x--;
		location = videoMemory + (y*80 + x);
		*location = ' ' | attribute;
	}
	else if(c== 0x09)
	{
		x = (x + 8) & ~(7);
	}
	else if(c == '\r')
	{
		x = 0;
	}
	else if(c == '\n')
	{
		x = 0;
		y++;
	}
	else if(c >= ' ')
	{
		location = videoMemory + (y*80 + x);
		*location = c | attribute;
		x++;	
	}
	
	if(x > 80)
	{
		x = 0;
		y++;
	}

	scroll();
	moveCursor();
}


void clear()
{
	UInt8 attribute = (0 << 4) | (15 & 0x0F);
	UInt16 blank = 0x20 | (attribute << 8);

	int i;
	for(i = 0; i < 80*25; i++)
	{
		videoMemory[i] = blank;
	}
	x = 0;
	y = 0;
	moveCursor();
}

void putString(char *c)
{
	int i = 0;
	while(c[i])
	{
		putc(c[i++]);
	}
}




void WriteHex(UInt32 n)
{
	int i = 0;	
	UInt8* ptr = (UInt8*) &n;
	ptr = ptr + 3;
	do
	{	
		
		UInt8 Fourbit = (*(ptr) & 0xF0) >> 4;
		if(Fourbit <= 9 && Fourbit >= 0)
		{
			putc(Fourbit + 0x30);
		}
		else
		{
			putc(Fourbit + 0x37);
		}

		Fourbit = *(ptr--) & 0x0F;
		if(Fourbit <= 9 && Fourbit >= 0)
		{
			putc(Fourbit + 0x30);
		}
		else
		{
			putc(Fourbit + 0x37);
		}
		i++;
	}while(i<4);
}

void WriteDec(UInt32 n)
{
	UInt32 x = n;
	UInt8 data[] = {0,0,0,0,0,0,0,0,0,0,0,0};
	int i = 10;
	do
	{	
		
		data[i] = x % 10;
		x = x / 10;
		i--;
	}while(x != 0 || i < 0);
	
	do
	{
		i++;
		putc(data[i] + '0');
	}while(i < 10);
	
}
