/**
 * ECE477 Project Glass
 * Team 10
 */

#include <asf.h>
#include "OLED.h"
#include "BLE.h"
#include "DataDisplayStruct.h"

Weather weather;
void getDay(char *day , int dayNum)
{
	
	if(dayNum == 1)
	{
		day[0] = 'M';
		day[1] = 'O';
		day[2] = 'N';
		day[3] = '\0';
	}
	else if(dayNum == 2)
	{
		day[0] = 'T';
		day[1] = 'U';
		day[2] = 'E';
		day[3] = '\0';
	}
	else if(dayNum == 3)
	{
		day[0] = 'W';
		day[1] = 'E';
		day[2] = 'D';
		day[3] = '\0';
	}
	else if(dayNum == 4)
	{
		day[0] = 'T';
		day[1] = 'H';
		day[2] = 'U';
		day[3] = '\0';
	}
	else if(dayNum == 5)
	{
		day[0] = 'F';
		day[1] = 'R';
		day[2] = 'I';
		day[3] = '\0';
	}
	else if(dayNum == 6)
	{
		day[0] = 'S';
		day[1] = 'A';
		day[2] = 'T';
		day[3] = '\0';
	}
	else if(dayNum == 7)
	{
		day[0] = 'S';
		day[1] = 'U';
		day[2] = 'N';
		day[3] = '\0';
	}
	
}

void drawWeatherInfo()
{
	int i = 0;
	char day[4];
	//day[0] = 'W';
	//day[1] = 'E';
	//day[2] = 'D';
	//day[3] = '\0';
	int increment = 0;
	for(i = 0; i < 2 ; i++)
	{
		getDay(day,weatherObject[i].typeDay);
		//prints day
		drawCharacter(day[0],0 + increment,2 ,7 + increment,10,RED);
		drawCharacter(day[1],9 + increment,2 ,16 + increment,10,RED );
		drawCharacter(day[2],18 + increment,2 ,25 + increment,10,RED );
		
		//prints temp
		drawCharacter('O',0 + increment,37,3 + increment,40,RED);
		//drawCharacter('E',0 + increment,36,6 + increment,40,RED);
		drawCharacter('C',0 + increment,42,6 + increment,48,RED);
		//drawCharacter('P',0 + increment,48,6 + increment,52,RED);
		
		int temperature = weatherObject[i].temperature;
		if(temperature < 0)
		{
			draw_line(15 + increment,31,15 + increment,34,CYAN);
			temperature = temperature*(-1);
		}		
		int tempOnes = temperature % 10;
		int tempTens = (temperature/10) % 10;
		//draw_line(0,0,63,95,GREEN);	// The perfect diagonal

		drawDigit(tempTens,10 + increment,36,20 + increment,39,CYAN);
		drawDigit(tempOnes,10 + increment,41,20 + increment,44,CYAN);

		
		drawCharacter('K',0 + increment,70,6 + increment,74,RED);
		drawCharacter('M',0 + increment,76,6 + increment,80,RED);
		drawCharacter('P',0 + increment,82,6 + increment,86,RED);
		drawCharacter('H',0 + increment,88,6 + increment,92,RED);
		
		int windSpeed = weatherObject[i].windSpeed;
		if(windSpeed < 0)
		{
			draw_line(15 + increment,71,15 + increment,74,CYAN);
			windSpeed = windSpeed*(-1);
		}
		int windOnes = windSpeed % 10;
		int windTens = (windSpeed / 10) % 10;

		drawDigit(windTens,10 + increment,76,20 + increment,79,CYAN);
		drawDigit(windOnes,10 + increment,81,20 + increment,84,CYAN);
			
		increment += 36;
	}	
}

void drawNewsArticle(int newsNo, uint16_t color)
{
	int sRow = newsNo*21;
	int START_COL = 1;
	int sCol = START_COL;
	int verticalOffset = 0;
	
	int CHAR_PER_LINE = 13;
	
	int i;
	for(i = 0; i < CHAR_PER_LINE * 3 - 1; ++i)
	{
		if(i != 0 && i % CHAR_PER_LINE == 0)
		{
			sCol = START_COL;
			verticalOffset += 7;
		}
		if(newsObject[newsNo].headline[i] == "\0")
			break;
		drawCharacter(newsObject[newsNo].headline[i],
				sRow + verticalOffset + 1, 
				sCol + 1 + (i%CHAR_PER_LINE)*7,
				sRow + verticalOffset + 1 + 5,
				sCol + 1 + 5 + (i%CHAR_PER_LINE)*7,
				color);
	}
	draw_line(	sRow + verticalOffset + 1 + 5,
				sCol + 1 + (i%CHAR_PER_LINE)*7 + 0,
				sRow + verticalOffset + 1 + 5,
				sCol + 1 + (i%CHAR_PER_LINE)*7 + 1,
				color);
	
	draw_line(	sRow + verticalOffset + 1 + 5,
				sCol + 1 + (i%CHAR_PER_LINE)*7 + 3,
				sRow + verticalOffset + 1 + 5,
				sCol + 1 + (i%CHAR_PER_LINE)*7 + 4,
				color);

	draw_line(	sRow + verticalOffset + 1 + 5,
				sCol + 1 + (i%CHAR_PER_LINE)*7 + 6,
				sRow + verticalOffset + 1 + 5,
				sCol + 1 + (i%CHAR_PER_LINE)*7 + 7,
				color);
}

void drawNewsInfo()
{
	// newsObject[3] struct defined
	//	char headline[51]; //source
	//	int currentIndex;
	fill_color(WHITE);
	drawNewsArticle(0, RED);
	drawNewsArticle(1, BLUE);
	drawNewsArticle(2, GREEN);
}

int main (void)
{
	delay_init();
	system_init();
	spi_init();
	/* Insert application code here, after the board has been initialized. */	
	initializeOLED();
	//drawTimeMenu(12,0);
	//fill_color(CYAN);
	//drawNewsInfo();
	//drawWeatherInfo();
	//read_char();
	//drawCharacter('z', 22,  5, 40, 15,RED);
	//drawCharacter('y', 22, 18, 40, 28,RED);
	//fill_color(YELLOW);
	initializeUSART();
	//draw_line(YELLOW);
	//drawTimeMenu(9, 45, 53);
	//drawCharacter('a', int sRow, int sCol, int eRow, int eCol);

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
		if(timeChangeFlag && screenTrackCounter == 0 && isScreenOn && doneFlag)
		{
			//Call display function for time
			fill_color(BLACK);
			drawTimeMenu(timeObject.hour, timeObject.min);
			timeChangeFlag = false;
		}
		
		if(weatherChangeFlag && screenTrackCounter == 1 && isScreenOn && doneFlag)
		{
			//call display for weather
			fill_color(BLACK);
			drawWeatherInfo();
			weatherChangeFlag = false;
		}
		
		if(newsChangeFlag && screenTrackCounter == 2 && isScreenOn && doneFlag)
		{
			//call display for news
			fill_color(MAGENTA);
			drawNewsInfo();
			newsChangeFlag = false;
		}
				
		delay_ms(1000);
		
		//fill_color(BLUE);
	}
}