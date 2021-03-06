#define SPI_SCLK_LOW_TIME 0.004
#define SPI_SCLK_HIGH_TIME 0.004
#define HIGH_VALUE true
#define LOW_VALUE false

//OLED PIN DEFINITIONS
#define SCL_OLED	PIN_PA08
#define SDA_OLED	PIN_PA09
#define CS_OLED		PIN_PA10
#define RST_OLED	PIN_PA11
#define DC_OLED		PIN_PA16

// Timing Delays
#define SSD1331_DELAYS_HWFILL		(100)
#define SSD1331_DELAYS_HWLINE       (1)

//colors
#define	BLACK           0x0000
#define	BLUE            0x001F
#define	RED             0xF800
#define	GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0
#define WHITE           0xFFFF

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

void spi_init()
{
	//SPI Pin Definitions
	struct port_config pin_conf_clk;
	port_get_config_defaults(&pin_conf_clk);
	
	struct port_config pin_conf_mosi;
	port_get_config_defaults(&pin_conf_mosi);
	
	struct port_config pin_conf_CS;
	port_get_config_defaults(&pin_conf_CS);	
	
	struct port_config pin_conf_RST;
	port_get_config_defaults(&pin_conf_RST);	
//SN,DEVICENAME
	struct port_config pin_conf_DC;
	port_get_config_defaults(&pin_conf_DC);	

	pin_conf_clk.direction  = PORT_PIN_DIR_OUTPUT;
	port_pin_set_config(SCL_OLED, &pin_conf_clk);
	
	pin_conf_mosi.direction  = PORT_PIN_DIR_OUTPUT;
	port_pin_set_config(SDA_OLED, &pin_conf_mosi);
	
	pin_conf_CS.direction  = PORT_PIN_DIR_OUTPUT;
	port_pin_set_config(CS_OLED, &pin_conf_CS);
	
	pin_conf_RST.direction  = PORT_PIN_DIR_OUTPUT;
	port_pin_set_config(RST_OLED, &pin_conf_RST);	

	pin_conf_DC.direction  = PORT_PIN_DIR_OUTPUT;
	port_pin_set_config(DC_OLED, &pin_conf_DC);	
}
void write_MOSI(bool high)
{
	if(high)
	{
		port_pin_set_output_level(SDA_OLED, true);
	}
	else
	{
		port_pin_set_output_level(SDA_OLED, false);
	}
}

void write_SCLK(bool high)
{
	if(high)
	{
		port_pin_set_output_level(SCL_OLED, true);
	}
	
	else
	{
		port_pin_set_output_level(SCL_OLED, false);
	}
}

uint8_t SPI_transfer_byte(uint8_t byte_out)
{
	delay_ms(2);
	uint8_t byte_in = 0;
	uint8_t bit;
	port_pin_set_output_level(CS_OLED, true);
	delay_ms(10);
	port_pin_set_output_level(CS_OLED, false); //cs low
	for (bit = 0x80; bit; bit >>= 1)
	{
		/* Shift-out a bit to the MOSI line */
		write_MOSI((byte_out & bit) ? HIGH : LOW);

		/* Delay for at least the peer's setup time */
		delay_us(SPI_SCLK_LOW_TIME); //changed to us from ms
 
		/* Pull the clock line high */
		write_SCLK(HIGH_VALUE);

		/* Shift-in a bit from the MISO line */
		//if (read_MISO() == HIGH)
		//byte_in |= bit;

		/* Delay for at least the peer's hold time */
		delay_us(SPI_SCLK_HIGH_TIME); //changed to us from ms

		/* Pull the clock line low */
		write_SCLK(LOW_VALUE);
	}

	return byte_in;
}

void fill_color(uint16_t color)
{
	SPI_transfer_byte(SSD1331_CMD_FILL);
	SPI_transfer_byte(0x01);
	SPI_transfer_byte(SSD1331_CMD_DRAWRECT);
	SPI_transfer_byte(0x02 & 0xFF);							// Starting column
	SPI_transfer_byte(0x01 & 0xFF);							// Starting row
	SPI_transfer_byte((0x02+96-1) & 0xFF);	// End column
	SPI_transfer_byte((0x01+64-1) & 0xFF);	// End row
	
	// Outline color
	SPI_transfer_byte((uint8_t)((color >> 11) << 1));
	SPI_transfer_byte((uint8_t)((color >> 5) & 0x3F));
	SPI_transfer_byte((uint8_t)((color << 1) & 0x3F));
	// Fill color
	SPI_transfer_byte((uint8_t)((color >> 11) << 1));
	SPI_transfer_byte((uint8_t)((color >> 5) & 0x3F));
	SPI_transfer_byte((uint8_t)((color << 1) & 0x3F));
	
	// Delay while the fill completes
	delay_ms(SSD1331_DELAYS_HWFILL);
}

void draw_line(uint8_t sRow, uint8_t sCol, uint8_t eRow, uint8_t eCol, uint16_t color)
{
	SPI_transfer_byte(SSD1331_CMD_DRAWLINE);
	SPI_transfer_byte(sCol);
	SPI_transfer_byte(sRow);
	SPI_transfer_byte(eCol);
	SPI_transfer_byte(eRow);
	// SPI_transfer_byte(0x00);
	// SPI_transfer_byte(0x00);
	// SPI_transfer_byte(0x20);
	// SPI_transfer_byte(0x10);
	delay_ms(SSD1331_DELAYS_HWLINE);
	SPI_transfer_byte((uint8_t)((color >> 11) << 1));
	SPI_transfer_byte((uint8_t)((color >> 5) & 0x3F));
	SPI_transfer_byte((uint8_t)((color << 1) & 0x3F));
	delay_ms(SSD1331_DELAYS_HWLINE);
}

void drawCharacter(char character, int sRow, int sCol, int eRow, int eCol, uint16_t color)
{
	int width = eCol - sCol;
	int height = eRow - sRow;

	switch(character)
	{
		case 'a':
		case 'A':
		draw_line(sRow, sCol, sRow, sCol + width, color);               // draw top line
		draw_line(sRow, sCol, sRow + height, sCol, color);                // draw left line
		draw_line(sRow + height/2, sCol, sRow + height/2, sCol + width, color); // draw middle line
		draw_line(sRow, sCol + width, sRow + height, sCol + width, color); // draw right line
		break;
		case 'b':
		case 'B':
		draw_line(sRow, sCol, sRow + height, sCol, color);                // draw left line
		draw_line(sRow + height/2, sCol, sRow + height/2, sCol + width, color); // draw middle line
		draw_line(sRow + height/2, sCol + width, sRow + height, sCol + width, color); // draw right line
		draw_line(sRow + height, sCol, sRow + height, sCol + width, color); // draw bottom line
		break;
		case 'c':
		case 'C':
		draw_line(sRow + height/2, sCol, sRow + height, sCol, color);                // draw left line
		draw_line(sRow + height/2, sCol, sRow + height/2, sCol + width, color); // draw middle line
		draw_line(sRow + height, sCol, sRow + height, sCol + width, color); // draw bottom line
		break;
		case 'd':
		case 'D':
		draw_line(sRow + height/2, sCol, sRow + height, sCol, color);                // draw left line
		draw_line(sRow + height/2, sCol, sRow + height/2, sCol + width, color); // draw middle line
		draw_line(sRow, sCol + width, sRow + height, sCol + width, color); // draw right line
		draw_line(sRow + height, sCol, sRow + height, sCol + width, color); // draw bottom line
		break;
		case 'e':
		case 'E':
		draw_line(sRow, sCol, sRow, sCol + width, color);               // draw top line
		draw_line(sRow, sCol, sRow + height, sCol, color);                // draw left line
		draw_line(sRow + height/2, sCol, sRow + height/2, sCol + width, color); // draw middle line
		draw_line(sRow + height, sCol, sRow + height, sCol + width, color); // draw bottom line
		break;
		case 'f':
		case 'F':
		draw_line(sRow, sCol, sRow, sCol + width, color);               // draw top line
		draw_line(sRow, sCol, sRow + height, sCol, color);                // draw left line
		draw_line(sRow + height/2, sCol, sRow + height/2, sCol + width, color); // draw middle line
		break;
		case 'g':
		case 'G':
		draw_line(sRow, sCol, sRow, sCol + width, color);               // draw top line
		draw_line(sRow, sCol, sRow + height, sCol, color);                // draw left line
		draw_line(sRow + height/2, sCol + width/2, sRow + height/2, sCol + width, color); // draw middle line
		draw_line(sRow + height/2, sCol + width, sRow + height, sCol + width, color); // draw right line
		draw_line(sRow + height, sCol, sRow + height, sCol + width, color); // draw bottom line
		break;
		case 'h':
		case 'H':
		draw_line(sRow, sCol, sRow + height, sCol, color);                // draw left line
		draw_line(sRow + height/2, sCol, sRow + height/2, sCol + width, color); // draw middle line
		draw_line(sRow + height/2, sCol + width, sRow + height, sCol + width, color); // draw right line
		break;
		case 'i':
		case 'I':
		draw_line(sRow, sCol, sRow, sCol + width, color);               // draw top line
		draw_line(sRow, sCol + width/2, sRow + height, sCol + width/2, color);                // draw left line
		draw_line(sRow + height, sCol, sRow + height, sCol + width, color); // draw bottom line
		break;
		case 'j':
		case 'J':
		draw_line(sRow, sCol, sRow, sCol + width, color);               // draw top line
		draw_line(sRow + 2*height/3, sCol + width/3, sRow + height, sCol + width/3, color);                // draw left line
		draw_line(sRow, sCol + 2*width/3, sRow + height, sCol + 2*width/3, color); // draw right line
		draw_line(sRow + height, sCol + width/3, sRow + height, sCol + 2*width/3, color); // draw bottom line
		break;
		case 'k':
		case 'K':
		draw_line(sRow, sCol, sRow + height, sCol, color);                // draw left line
		draw_line(sRow + height/2, sCol, sRow, sCol + width, color); // draw right line
		draw_line(sRow + height/2, sCol, sRow + height, sCol + width, color); // draw bottom line
		break;
		case 'l':
		case 'L':
		draw_line(sRow, sCol, sRow + height, sCol, color);                // draw left line
		draw_line(sRow + height, sCol, sRow + height, sCol + width, color); // draw bottom line
		break;
		case 'm':
		case 'M':
		draw_line(sRow, sCol, sRow + height, sCol, color);                // draw left line
		draw_line(sRow, sCol + width, sRow + height, sCol + width, color); // draw right line
		draw_line(sRow, sCol, sRow + height/2, sCol + width/2, color);		// draw this(\) line
		draw_line(sRow + height/2, sCol + width/2, sRow, sCol + width, color); // draw this(/) line
		break;
		case 'n':
		case 'N':
		draw_line(sRow, sCol, sRow + height, sCol, color);                // draw left line
		draw_line(sRow, sCol + width, sRow + height, sCol + width, color); // draw right line
		draw_line(sRow, sCol, sRow + height, sCol + width/2, color);		// draw this(\) line
		break;
		case 'o':
		case 'O':
		draw_line(sRow, sCol, sRow, sCol + width, color);               // draw top line
		draw_line(sRow, sCol, sRow + height, sCol, color);                // draw left line
		draw_line(sRow, sCol + width, sRow + height, sCol + width, color); // draw right line
		draw_line(sRow + height, sCol, sRow + height, sCol + width, color); // draw bottom line
		break;
		case 'p':
		case 'P':
		draw_line(sRow, sCol, sRow, sCol + width, color);               // draw top line
		draw_line(sRow, sCol, sRow + height, sCol, color);                // draw left line
		draw_line(sRow + height/2, sCol, sRow + height/2, sCol + width, color); // draw middle line
		draw_line(sRow, sCol + width, sRow + height/2, sCol + width, color); // draw right line
		break;
		case 'q':
		case 'Q':
		draw_line(sRow, sCol, sRow, sCol + width, color);               // draw top line
		draw_line(sRow, sCol, sRow + height, sCol, color);                // draw left line
		draw_line(sRow + height/2, sCol + width/2, sRow + height, sCol + width, color); // draw this(\) line
		draw_line(sRow, sCol + width, sRow + height, sCol + width, color); // draw right line
		draw_line(sRow + height, sCol, sRow + height, sCol + width, color); // draw bottom line
		break;
		case 'r':
		case 'R':
		draw_line(sRow, sCol, sRow, sCol + width, color);               // draw top line
		draw_line(sRow, sCol, sRow + height, sCol, color);                // draw left line
		draw_line(sRow + height/2, sCol, sRow + height/2, sCol + width, color); // draw middle line
		draw_line(sRow, sCol + width, sRow + height/2, sCol + width, color); // draw right line
		draw_line(sRow + height/2, sCol + width/2, sRow + height, sCol + width, color); // draw this(\) line
		break;
		case 's':
		case 'S':
		draw_line(sRow, sCol, sRow, sCol + width, color);               // draw top line
		draw_line(sRow, sCol, sRow + height/2, sCol, color);                // draw left line
		draw_line(sRow + height/2, sCol, sRow + height/2, sCol + width, color); // draw middle line
		draw_line(sRow + height/2, sCol + width, sRow + height, sCol + width, color); // draw right line
		draw_line(sRow + height, sCol, sRow + height, sCol + width, color); // draw bottom line
		break;
		case 't':
		case 'T':
		draw_line(sRow, sCol, sRow, sCol + width, color);               // draw top line
		draw_line(sRow, sCol + width/2, sRow + height, sCol + width/2, color);	// draw left line
		break;
		case 'u':
		case 'U':
		draw_line(sRow, sCol, sRow + height, sCol, color);                // draw left line
		draw_line(sRow + height/2, sCol, sRow + height/2, sCol + width, color); // draw middle line
		draw_line(sRow, sCol + width, sRow + height, sCol + width, color); // draw right line
		draw_line(sRow + height, sCol, sRow + height, sCol + width, color); // draw bottom line
		break;
		case 'v':
		case 'V':
		draw_line(sRow, sCol, sRow + height, sCol + width/2, color);		// draw this(\) line
		draw_line(sRow + height, sCol + width/2, sRow, sCol + width, color); // draw this(/) line
		break;
		case 'w':
		case 'W':
		draw_line(sRow, sCol, sRow + height, sCol, color);                // draw left line
		draw_line(sRow, sCol + width, sRow + height, sCol + width, color); // draw right line
		draw_line(sRow + height/2, sCol + width/2, sRow + height, sCol + width, color); // draw this(\) line
		draw_line(sRow + height, sCol, sRow + height/2, sCol + width/2, color); // draw this(/) line
		break;
		case 'x':
		case 'X':
		draw_line(sRow, sCol, sRow + height, sCol + width, color);		// draw this(\) line
		draw_line(sRow + height, sCol, sRow, sCol + width, color); // draw this(/) line
		break;
		case 'y':
		case 'Y':
		draw_line(sRow, sCol, sRow + height/2, sCol + width/2, color);		// draw this(\) line
		draw_line(sRow + height, sCol, sRow, sCol + width, color); // draw this(/) line
		break;
		case 'z':
		case 'Z':
		draw_line(sRow, sCol, sRow, sCol + width, color);               // draw top line
		draw_line(sRow + height, sCol, sRow, sCol + width, color); // draw this(/) line
		draw_line(sRow + height, sCol, sRow + height, sCol + width, color); // draw bottom line
		break;
	}
}

void drawDigit(int digit, int sRow, int sCol, int eRow, int eCol, uint16_t color)
{
	int width  = eCol - sCol;
	int height = eRow - sRow;

	switch(digit)
	{
		case 0:
		draw_line(sRow, sCol, sRow + height, sCol, color);                  // draw left line
		draw_line(sRow, sCol, sRow, sCol + width, color);                   // draw top line
		draw_line(sRow + height, sCol, sRow + height, sCol + width, color); // draw bottom line
		draw_line(sRow, sCol + width, sRow + height, sCol + width, color);  // draw right line
		break;
		case 1:
		draw_line(sRow, sCol + 2*width/3, sRow + height, sCol + 2*width/3, color);
		break;
		case 2:
		draw_line(sRow, sCol, sRow, sCol + width, color);                   // draw top line
		draw_line(sRow, sCol + width, sRow + height/2, sCol + width, color);  // draw right line
		draw_line(sRow + height/2, sCol, sRow + height/2, sCol + width, color); // draw middle line
		draw_line(sRow + height/2, sCol, sRow + height, sCol, color);         // draw left line
		draw_line(sRow + height, sCol, sRow + height, sCol + width, color); // draw bottom line
		break;
		case 3:
		draw_line(sRow, sCol, sRow, sCol + width, color);                   // draw top line
		draw_line(sRow, sCol + width, sRow + height, sCol + width, color);  // draw right line
		draw_line(sRow + height/2, sCol, sRow + height/2, sCol + width, color); // draw middle line
		draw_line(sRow + height, sCol, sRow + height, sCol + width, color); // draw bottom line
		break;
		case 4:
		draw_line(sRow, sCol, sRow + height/2, sCol, color);         // draw left line
		draw_line(sRow + height/2, sCol, sRow + height/2, sCol + width, color); // draw middle line
		draw_line( sRow + height/3, sCol + 2*width/3, sRow + height, sCol + 2*width/3 , color);
		break;
		case 5:
		draw_line(sRow, sCol, sRow, sCol + width, color);               // draw top line
		draw_line(sRow, sCol, sRow + height/2, sCol, color);                // draw left line
		draw_line(sRow + height/2, sCol, sRow + height/2, sCol + width, color); // draw middle line
		draw_line(sRow + height/2, sCol + width, sRow + height, sCol + width, color); // draw right line
		draw_line(sRow + height, sCol, sRow + height, sCol + width, color); // draw bottom line
		break;
		case 6:
		draw_line(sRow, sCol, sRow, sCol + width, color);               // draw top line
		draw_line(sRow, sCol, sRow + height, sCol, color);                // draw left line
		draw_line(sRow + height/2, sCol, sRow + height/2, sCol + width, color); // draw middle line
		draw_line(sRow + height/2, sCol + width, sRow + height, sCol + width, color); // draw right line
		draw_line(sRow + height, sCol, sRow + height, sCol + width, color); // draw bottom line
		break;
		case 7:
		draw_line(sRow, sCol, sRow, sCol + width, color);               // draw top line
		draw_line(sRow, sCol + width, sRow + height, sCol + 2*width/3, color); // draw right line
		break;
		case 8:
		draw_line(sRow, sCol, sRow, sCol + width, color);               // draw top line
		draw_line(sRow, sCol, sRow + height, sCol, color);                // draw left line
		draw_line(sRow + height/2, sCol, sRow + height/2, sCol + width, color); // draw middle line
		draw_line(sRow, sCol + width, sRow + height, sCol + width, color); // draw right line
		draw_line(sRow + height, sCol, sRow + height, sCol + width, color); // draw bottom line
		break;
		case 9:
		draw_line(sRow, sCol, sRow, sCol + width, color);               // draw top line
		draw_line(sRow, sCol, sRow + height/2, sCol, color);                // draw left line
		draw_line(sRow + height/2, sCol, sRow + height/2, sCol + width, color); // draw middle line
		draw_line(sRow, sCol + width, sRow + height, sCol + width, color); // draw right line
		draw_line(sRow + height, sCol, sRow + height, sCol + width, color); // draw bottom line
		break;
	}
}

void drawTimeMenu(int hour, int mins)
{
	// dimensions: 96 * 64
	//drawDigit(int digit, int sRow, int sCol, int eRow, int eCol)
	drawDigit(hour/10, 5, 5, 59, 24, RED);
	drawDigit(hour%10, 5, 27, 59, 46, RED);
	drawDigit(mins/10, 5, 49, 59, 68, RED);
	drawDigit(mins%10, 5, 71, 59, 90, RED);
}

void initializeOLED()
{
	//reset
	delay_ms(200);
	port_pin_set_output_level(RST_OLED, true); //reset high
	delay_ms(500);

	port_pin_set_output_level(RST_OLED, false); //reset low
	delay_ms(500);
	port_pin_set_output_level(RST_OLED, true); //reset high
	delay_ms(500);
	//port_pin_set_output_level(RST_OLED, false); //reset high
	//delay_ms(500);
	port_pin_set_output_level(DC_OLED, false); //DC LOW COMMAND MODE
	delay_ms(200);
	
	
	
	//SPI_transfer_byte(0X25);  	// 0xAE
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
	//SPI_transfer_byte(0X25);  	// 0xAE
	//SPI_transfer_byte(0X00);  	// 0xAE
	//SPI_transfer_byte(0X00);  	// 0xAE
	//SPI_transfer_byte(0X10);  	// 0xAE
	//SPI_transfer_byte(0X10);  	// 0xAE
	
	fill_color(BLACK);
}
