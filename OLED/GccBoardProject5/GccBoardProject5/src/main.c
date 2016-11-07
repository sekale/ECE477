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
#define SPI_SCLK_LOW_TIME 0.01
#define SPI_SCLK_HIGH_TIME 0.01
#define HIGH_VALUE true
#define LOW_VALUE false

// Timing Delays
#define SSD1331_DELAYS_HWFILL		(100)
#define SSD1331_DELAYS_HWLINE       (1)

// SSD1331 Commands
#define SSD1331_CMD_DRAWLINE 		0x21
#define SSD1331_CMD_DRAWRECT 		0x22
#define SSD1331_CMD_FILL 			0x26
#define SSD1331_CMD_SETCOLUMN 		0x15
#define SSD1331_CMD_SETROW    		0x75
#define SSD1331_CMD_CONTRASTA 		0x81
#define SSD1331_CMD_CONTRASTB 		0x82
#define SSD1331_CMD_CONTRASTC		0x83
#define SSD1331_CMD_MASTERCURRENT 	0x87
#define SSD1331_CMD_SETREMAP 		0xA0
#define SSD1331_CMD_STARTLINE 		0xA1
#define SSD1331_CMD_DISPLAYOFFSET 	0xA2
#define SSD1331_CMD_NORMALDISPLAY 	0xA4
#define SSD1331_CMD_DISPLAYALLON  	0xA5
#define SSD1331_CMD_DISPLAYALLOFF 	0xA6
#define SSD1331_CMD_INVERTDISPLAY 	0xA7
#define SSD1331_CMD_SETMULTIPLEX  	0xA8
#define SSD1331_CMD_SETMASTER 		0xAD
#define SSD1331_CMD_DISPLAYOFF 		0xAE
#define SSD1331_CMD_DISPLAYON     	0xAF
#define SSD1331_CMD_POWERMODE 		0xB0
#define SSD1331_CMD_PRECHARGE 		0xB1
#define SSD1331_CMD_CLOCKDIV 		0xB3
#define SSD1331_CMD_PRECHARGEA 		0x8A
#define SSD1331_CMD_PRECHARGEB 		0x8B
#define SSD1331_CMD_PRECHARGEC 		0x8C
#define SSD1331_CMD_PRECHARGELEVEL 	0xBB
#define SSD1331_CMD_VCOMH 			0xBE

void write_MOSI(bool high)
{
	if(high)
	{
		port_pin_set_output_level(EXT1_PIN_16, true);
	}
	else
	{
		port_pin_set_output_level(EXT1_PIN_16, false);
	}
}

void write_SCLK(bool high)
{
	if(high)
	{
		port_pin_set_output_level(EXT1_PIN_15, true);
	}
	
	else
	{
		port_pin_set_output_level(EXT1_PIN_15, false);
	}
}

uint8_t SPI_transfer_byte(uint8_t byte_out)
{
	delay_ms(2);
    uint8_t byte_in = 0;
    uint8_t bit;

    for (bit = 0x80; bit; bit >>= 1) {
        /* Shift-out a bit to the MOSI line */
        write_MOSI((byte_out & bit) ? HIGH : LOW);

        /* Delay for at least the peer's setup time */
        delay_ms(SPI_SCLK_LOW_TIME);

        /* Pull the clock line high */
        write_SCLK(HIGH_VALUE);

        /* Shift-in a bit from the MISO line */
        //if (read_MISO() == HIGH)
            //byte_in |= bit;

        /* Delay for at least the peer's hold time */
        delay_ms(SPI_SCLK_HIGH_TIME);

        /* Pull the clock line low */
        write_SCLK(LOW_VALUE);
    }

    return byte_in;
}

int main (void)
{
	delay_init();
	system_init();
	
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
	
	
	
/*#define EXT1_PIN_15               PIN_PA05
#define EXT1_PIN_16               PIN_PA06
#define EXT1_PIN_17               PIN_PA04
#define EXT1_PIN_18               PIN_PA07*/

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
	int j = 0;
	uint16_t color = 0xCC;
	uint16_t x = 0X02;
	uint16_t y = 0X15;
	uint16_t w = 0X03;
	uint16_t h = 0X12;
	/* This skeleton code simply sets the LED to the state of the button. */
	while (1) {
		/* Is button pressed? */
		//sending data
		port_pin_set_output_level(EXT1_PIN_17, true);
		delay_ms(200);
		port_pin_set_output_level(EXT1_PIN_17, false); //cs low
		//reset
		delay_ms(200);
		port_pin_set_output_level(EXT1_PIN_18, true); //reset high
		delay_ms(200);

		port_pin_set_output_level(EXT1_PIN_18, false); //reset low
		delay_ms(200);
		port_pin_set_output_level(EXT1_PIN_18, true); //reset high
		delay_ms(200);		
		port_pin_set_output_level(EXT1_PIN_14, false); //DC LOW COMMAND MODE
		delay_ms(200);	

		SPI_transfer_byte(0X25);  	// 0xAE
		SPI_transfer_byte(SSD1331_CMD_DISPLAYOFF);  	// 0xAE
		SPI_transfer_byte(SSD1331_CMD_SETREMAP); 	// 0xA0	
		SPI_transfer_byte(0x72); 	// rgb color
		
		SPI_transfer_byte(SSD1331_CMD_STARTLINE); 	// 0xA1
		SPI_transfer_byte(0x0);
		SPI_transfer_byte(SSD1331_CMD_DISPLAYOFFSET); 	// 0xA2
		SPI_transfer_byte(0x0);
		SPI_transfer_byte(SSD1331_CMD_NORMALDISPLAY);  	// 0xA4
		SPI_transfer_byte(SSD1331_CMD_SETMULTIPLEX); 	// 0xA8
		SPI_transfer_byte(0x3F);  			// 0x3F 1/64 duty
		SPI_transfer_byte(SSD1331_CMD_SETMASTER);  	// 0xAD
		SPI_transfer_byte(0x8E);
		SPI_transfer_byte(SSD1331_CMD_POWERMODE);  	// 0xB0
		SPI_transfer_byte(0x0B);
		SPI_transfer_byte(SSD1331_CMD_PRECHARGE);  	// 0xB1
		SPI_transfer_byte(0x31);
		SPI_transfer_byte(SSD1331_CMD_CLOCKDIV);  	// 0xB3
		SPI_transfer_byte(0xF0);  // 7:4 = Oscillator Frequency, 3:0 = CLK Div Ratio (A[3:0]+1 = 1..16)
		SPI_transfer_byte(SSD1331_CMD_PRECHARGEA);  	// 0x8A
		SPI_transfer_byte(0x64);
		SPI_transfer_byte(SSD1331_CMD_PRECHARGEB);  	// 0x8B
		SPI_transfer_byte(0x78);
		SPI_transfer_byte(SSD1331_CMD_PRECHARGEA);  	// 0x8C
		SPI_transfer_byte(0x64);
		SPI_transfer_byte(SSD1331_CMD_PRECHARGELEVEL);  	// 0xBB
		SPI_transfer_byte(0x3A);
		SPI_transfer_byte(SSD1331_CMD_VCOMH);  		// 0xBE
		SPI_transfer_byte(0x3E);
		SPI_transfer_byte(SSD1331_CMD_MASTERCURRENT);  	// 0x87
		SPI_transfer_byte(0x06);
		SPI_transfer_byte(SSD1331_CMD_CONTRASTA);  	// 0x81
		SPI_transfer_byte(0x91);
		SPI_transfer_byte(SSD1331_CMD_CONTRASTB);  	// 0x82
		SPI_transfer_byte(0x50);
		SPI_transfer_byte(SSD1331_CMD_CONTRASTC);  	// 0x83
		SPI_transfer_byte(0x7D);
		SPI_transfer_byte(SSD1331_CMD_DISPLAYON);	//--turn on oled panel  
		
		
		
		SPI_transfer_byte(SSD1331_CMD_SETCOLUMN);
		SPI_transfer_byte(0X02);
		SPI_transfer_byte(3-1);

		SPI_transfer_byte(SSD1331_CMD_SETROW);
		SPI_transfer_byte(0X05);
		SPI_transfer_byte(2-1);
		port_pin_set_output_level(EXT1_PIN_14, true); //DC HIGH DATA MODE
		
		SPI_transfer_byte(color >> 8);    
		SPI_transfer_byte(color);
		
		
		/*SPI_transfer_byte(SSD1331_CMD_FILL);
		SPI_transfer_byte(0x01);
		SPI_transfer_byte(SSD1331_CMD_DRAWRECT);
		SPI_transfer_byte(0x03 & 0xFF);							// Starting column
		SPI_transfer_byte(0x02 & 0xFF);							// Starting row
		SPI_transfer_byte((0x03+w-1) & 0xFF);	// End column
		SPI_transfer_byte((0x02+h-1) & 0xFF);	// End row
  
		// Outline color
		SPI_transfer_byte((uint8_t)((color >> 11) << 1));
		SPI_transfer_byte((uint8_t)((color >> 5) & 0x3F));
		SPI_transfer_byte((uint8_t)((color << 1) & 0x3F));
		// Fill color
		SPI_transfer_byte((uint8_t)((color >> 11) << 1));
		SPI_transfer_byte((uint8_t)((color >> 5) & 0x3F));
		SPI_transfer_byte((uint8_t)((color << 1) & 0x3F));
 
		// Delay while the fill completes
		delay_ms(SSD1331_DELAYS_HWFILL); */
		
		delay_ms(100000);

	}
}

		/*if (port_pin_get_input_level(BUTTON_0_PIN) == BUTTON_0_ACTIVE) {
			
			//delay_ms(100);
			
			port_pin_set_output_level(LED_0_PIN, LED_0_ACTIVE);
			delay_ms(10000);
		} else {
		
			port_pin_set_output_level(LED_0_PIN, !LED_0_ACTIVE);
		}*/