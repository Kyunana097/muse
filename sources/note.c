#include "Headers/oledfont.h"
#include "Headers/note.h"
#include <REGX52.H>
#include "Headers/oled.h"

unsigned char track_position = 0;
unsigned char speed = 4;	

void note_spawn(track)
{
	x_position = 0;//Òô·ûË®Æ½Î»ÖÃ 0-127
	for (x_position = 128; x_position - speed; x_position > 0)
	{
		OLED_ShowString(x_position, track, "Q", 16);
	}
}