/**
 * ECE477 Project Glass
 * Team 10
 */

#include <asf.h>
#include "OLED.h"
#include "BLE.h"
#include "DataDisplayStruct.h"

int main (void)
{
	delay_init();
	system_init();
	spi_init();
	/* Insert application code here, after the board has been initialized. */	
	initializeOLED();
	
	Weather weatherData[3];
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
		
		//WV,XXXX,W,SAT#COUDY#25#32!SAT#COUDY#25#32!SAT#COUDY#25#32!
		delay_ms(100000);
		
		//while(usart_read_buffer_job(&usart_instance, rx_buffer_read, 1) != STATUS_OK);
		
		//W,		
		//WV,001E,572C46726923436C65617223372336.
		//WV,001E,21536174235261696E233223322153.
		//WV,001E,756E235261696E2334233221.		
		
		//fill_color(BLUE);

	}
}