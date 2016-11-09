/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * This is a bare minimum user application template.
 *
 * For documentation of the board, go \ref group_common_boards "here" for a link
 * to the board-specific documentation.
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# Minimal main function that starts with a call to system_init()
 * -# Basic usage of on-board LED and button
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
#include <asf.h>
#include "OLED.h"

int main (void)
{
	delay_init();
	system_init();
	
	//SPI Pin Definitions
	struct port_config pin_conf_clk;
	port_get_config_defaults(&pin_conf_clk);
	
	struct port_config pin_conf_mosi;
	port_get_config_defaults(&pin_conf_mosi);
	
	struct port_config pin_conf_CS;
	port_get_config_defaults(&pin_conf_CS);	
	
	struct port_config pin_conf_RST;
	port_get_config_defaults(&pin_conf_RST);	

	struct port_config pin_conf_DC;
	port_get_config_defaults(&pin_conf_DC);	

	pin_conf_clk.direction  = PORT_PIN_DIR_OUTPUT;
	port_pin_set_config(EXT1_PIN_15, &pin_conf_clk);
	
	pin_conf_mosi.direction  = PORT_PIN_DIR_OUTPUT;
	port_pin_set_config(EXT1_PIN_16, &pin_conf_mosi);
	
	pin_conf_CS.direction  = PORT_PIN_DIR_OUTPUT;
	port_pin_set_config(EXT1_PIN_17, &pin_conf_CS);
	
	pin_conf_RST.direction  = PORT_PIN_DIR_OUTPUT;
	port_pin_set_config(EXT1_PIN_18, &pin_conf_RST);	

	pin_conf_DC.direction  = PORT_PIN_DIR_OUTPUT;
	port_pin_set_config(EXT1_PIN_14, &pin_conf_DC);

	/* Insert application code here, after the board has been initialized. */	
	initializeOLED();
	/* This skeleton code simply sets the LED to the state of the button. */
	while (1) 
	{
		/* Is button pressed? */
		//sending data
		
		
		/*SPI_transfer_byte(SSD1331_CMD_SETCOLUMN);
		SPI_transfer_byte(0X02);
		SPI_transfer_byte(3-1);

		SPI_transfer_byte(SSD1331_CMD_SETROW);
		SPI_transfer_byte(0X05);
		SPI_transfer_byte(2-1);
		port_pin_set_output_level(EXT1_PIN_14, true); //DC HIGH DATA MODE
		
		SPI_transfer_byte(color >> 8);    
		SPI_transfer_byte(color);*/
		
    //SPI_transfer_byte(CMD_FILL_WINDOW);//fill window
    //SPI_transfer_byte(ENABLE_FILL);
    //SPI_transfer_byte(CMD_DRAW_RECTANGLE);//draw rectangle
    //SPI_transfer_byte(x0);//start column
    //SPI_transfer_byte(y0);//start row
    //SPI_transfer_byte(x1);//end column
    //SPI_transfer_byte(y1);//end row
    //SPI_transfer_byte((uint8_t)((outColor>>11)&0x1F));//R
    //SPI_transfer_byte((uint8_t)((outColor>>5)&0x3F));//G
    //SPI_transfer_byte((uint8_t)(outColor&0x1F));//B
    //SPI_transfer_byte((uint8_t)((fillColor>>11)&0x1F));//R
    //SPI_transfer_byte((uint8_t)((fillColor>>5)&0x3F));//G
    //SPI_transfer_byte((uint8_t)(fillColor&0x1F));//B		
		
		

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

		/*if (port_pin_get_input_level(BUTTON_0_PIN) == BUTTON_0_ACTIVE) {
			
			//delay_ms(100);
			
			port_pin_set_output_level(LED_0_PIN, LED_0_ACTIVE);
			delay_ms(10000);
		} else {
		
			port_pin_set_output_level(LED_0_PIN, !LED_0_ACTIVE);
		}*/