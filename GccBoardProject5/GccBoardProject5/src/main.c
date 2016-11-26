/**
 * ECE477 Project Glass
 * Team 10
 */

#include <asf.h>
#include "OLED.h"
#include "BLE.h"

int main (void)
{
	delay_init();
	system_init();
	spi_init();
	/* Insert application code here, after the board has been initialized. */	
	initializeOLED();
	initializeUSART();
	
	while (1) 
	{
		//start an async read of data
		//draw_line(YELLOW);
		drawTimeMenu(9, 45, 53);
		//drawCharacter('a', int sRow, int sCol, int eRow, int eCol);
		drawCharacter('z', 22,  5, 40, 15);
		drawCharacter('y', 22, 18, 40, 28);
		drawCharacter('x', 22, 35, 40, 45);
		drawCharacter('v', 22, 48, 40, 58);
		drawCharacter('w', 22, 65, 40, 75);
		
		delay_ms(100000);
		
		//fill_color(BLUE);

	}
}