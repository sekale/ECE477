/**
 * ECE477 Project Glass
 * Team 10
 */

#include <asf.h>
#include "OLED.h"
#include "BLE.h"
#include "DataDisplayStruct.h"

Weather weather;

int main (void)
{
	delay_init();
	system_init();
	spi_init();
	/* Insert application code here, after the board has been initialized. */	
	initializeOLED();
	//drawTimeMenu(12,0);
	fill_color(BLACK);
	drawWeatherInfo();
	//read_char();
	Weather weatherData[3];
	initializeUSART();
	//draw_line(YELLOW);
	//drawTimeMenu(9, 45, 53);
	//drawCharacter('a', int sRow, int sCol, int eRow, int eCol);
	//drawCharacter('z', 22,  5, 40, 15);
	//drawCharacter('y', 22, 18, 40, 28);
	//drawCharacter('x', 22, 35, 40, 45);
	//drawCharacter('v', 22, 48, 40, 58);
	//drawCharacter('w', 22, 65, 40, 75);
	int counterResetAdvertise = 0;
	while (1) 
	{
		++counterResetAdvertise;
		//Re-advertise the device if it stops showing after a while
		if(counterResetAdvertise == 30)
		{
			usart_write_buffer_wait(&usart_instance, string_A, sizeof(string_A) - 1);
			read_char();
			delay_ms(1000);
			counterResetAdvertise = 0;
		}
		//start an async read of data
		//Check if device is bound SUW will return an ERR\r\n if device is unbound
		read_char(); //read whatever's coming in through the RN4020 BLE
		if(timeChangeFlag && screenTrackCounter == 0 && isScreenOn)
		{
			//Call display function for time
			fill_color(BLACK);
			drawTimeMenu(timeObject.hour, timeObject.min);
			timeChangeFlag = false;
		}
		
		if(weatherChangeFlag && screenTrackCounter == 1 && isScreenOn)
		{
			//call display for weather
			fill_color(YELLOW);
			drawWeatherInfo();
			weatherChangeFlag = false;
		}
		
		if(newsChangeFlag && screenTrackCounter == 2 && isScreenOn)
		{
			//call display for news
			fill_color(GREEN);
			newsChangeFlag = false;
		}
				
		delay_ms(1000);
		
		//fill_color(BLUE);
	}
}