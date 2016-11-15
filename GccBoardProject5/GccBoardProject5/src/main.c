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
	initializeUSART();
	
	while (1) 
	{
		//Reading MAX_RX_BUFFER_LENGTH characters from SERCOM port
		//usart_read_buffer_job(&usart_instance,
			//(uint8_t *)rx_buffer, MAX_RX_BUFFER_LENGTH);	

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