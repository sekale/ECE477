/*
 * BLE.h
 *
 * Created: 12/1/2016 7:42:43 PM
 *  Author: pcadmin
 */ 

/*
 * BLE.h
 *
 * Created: 12/1/2016 7:37:21 PM
 *  Author: pcadmin
 */ 

#include <stdio.h>
#include <string.h>
#include "DataDisplayStruct.h"

#ifndef BLE_H_
#define BLE_H_

#define WAKE_HW PIN_PA27
#define WAKE_SW PIN_PA23
#define MICRO_BLE_TX PINMUX_PA24C_SERCOM3_PAD2//PIN_PA24
#define MICRO_BLE_RX PINMUX_PA25C_SERCOM3_PAD3//PIN_PA25

void usart_read_callback(struct usart_module *const usart_module);
void usart_write_callback(struct usart_module *const usart_module);

void configure_usart(void);
void configure_usart_callbacks(void);

//! [module_inst]
struct usart_module usart_instance;
//! [module_inst]

//! [rx_buffer_var]
#define MAX_RX_BUFFER_LENGTH   1000

//BLE Initializations
uint8_t		*string_inOp;
int			readCharLock = 0;
int			current_Rx_buffer_position = 0;
char		check_end_of_string = 's';
uint8_t		*actualResponseValue;
Weather		weatherObject[3];
News		newsObject[3];
TimeNow     timeObject;
bool		timeChangeFlag = false;
bool		weatherChangeFlag = false;
bool		newsChangeFlag = false;
int			screenTrackCounter = 0;
bool		isScreenOn = false;
bool		doneFlag = true;

//string send and responses//
uint8_t string_Plus[] = "+\r\n";
uint8_t string_Reboot[] = "R,1\r\n";
uint8_t string_LS[] = "LS\r\n";
uint8_t string_SF[] = "SF,1\r\n";
uint8_t string_SS[] = "SS,C0000000\r\n";
uint8_t string_SR[] = "SR,00000000\r\n";
uint8_t string_SS_1[] = "SS,C0000001\r\n";
uint8_t string_PZ[] = "PZ\r\n";
uint8_t string_PS[] = "PS,11223344556677889900AABBCCDDEEFF\r\n";
uint8_t string_PC_0[] = "PC,010203040506070809000A0B0C0D0E0F,02,05\r\n";
uint8_t string_PC_1[] = "PC,111213141516171819101A1B1C1D1E1F,18,20\r\n";
uint8_t string_U[] = "U\r\n";
uint8_t string_SUW[] = "SUW,0102030405060708090A0B0C0D0E0F,1234\r\n";
uint8_t string_A[] = "A\r\n";
uint8_t string_SSReponse[] = "AOK\r\n";
uint8_t string_RebootResponse[] = "Reboot\r\nCMD\r\n";
uint8_t string_SN[] = "SN,Glass\r\n";

char rx_buffer[MAX_RX_BUFFER_LENGTH];
volatile uint8_t rx_buffer_read[1];

bool compareResponse(uint8_t *currentResponse, uint8_t *expectedResponse)
{
	return true;
}

void clearBuffer()
{
	for(int i = 0; i < 1000; ++i)
	{
		rx_buffer[i] = ' ';
	}
}

void read_char()
{
	current_Rx_buffer_position = 0;
	doneFlag = false;
	int READ_CHAR_TIMEOUT =  100000;
	//clearBuffer();
	int read_char_timeout = 0;
	readCharLock = 1;
	while(readCharLock == 1)
	{
		read_char_timeout = 0;
		while(usart_read_buffer_job(&usart_instance, rx_buffer_read, 1) != STATUS_OK)
		{
			if(READ_CHAR_TIMEOUT == read_char_timeout++)
			{
				readCharLock = 0;
				break;
			}
		}
	}
}
/*
* Function : parseWeather()
* Author : Kanishk Yadav
*/
void parseWeather()
{
	//WV,XXXX,W 1 1-7 # temp # windspeed # sunny!
	weatherObject[rx_buffer[10] - '0' - 1].typeDay = rx_buffer[11] - '0';
	
	uint16_t i = 0;
	uint8_t arr[3];
	uint8_t count = 0;
	int j = 0;
	while( rx_buffer[i] != '!' )
	{
		if(rx_buffer[i] == '#')
		{
			arr[count++] = i;
		}
		i++;
	}
	
	uint8_t temperatureCounter = arr[0] + 1;
	char temperature[arr[1] - arr[0] - 1];
	uint8_t temp = 0;
	while(temperatureCounter != arr[1])
	{
		temperature[temp++] = rx_buffer[temperatureCounter++];
	}
	weatherObject[rx_buffer[10] - '0' - 1].temperature = atoi(temperature);

	uint8_t windSpeedCounter = arr[1] + 1;
	char windSpeed[arr[2] - arr[1] - 1];
	temp = 0;
	while(windSpeedCounter != arr[2])
	{
		windSpeed[temp++] = rx_buffer[windSpeedCounter++];
	}
	
	weatherObject[rx_buffer[10] - '0' - 1].windSpeed = atoi(windSpeed);
	
	for(j = arr[2] + 1 ; j < i; j++)
	{
		weatherObject[rx_buffer[10] - '0' - 1].typeWeather[j - arr[2] - 1] = rx_buffer[j];
	}
	
	weatherObject[rx_buffer[10] - '0' - 1].typeWeather[j - arr[2] - 1] = '\0';
	//	weatherObject[rx_buffer[10] - '0' - 1].typeWeather = atoi(rx_buffer[i-1]);	//considering the type of weather is integer, SEKALE needs to decide the correct format of sending weather type.
}

void parseNews()
{
	int i = 11;
	int newsObjIndex = actualResponseValue[10] - '0' - 1;
	
	if(actualResponseValue[8] == 'N')
	{
		newsObject[newsObjIndex].currentIndex	= 0;
		//newsObject[newsObjIndex].headline[0]	= '\0';
	}
	while(actualResponseValue[i] != '!')
	{
		if(newsObject[newsObjIndex].currentIndex > 49)
		{
			break;
		}
		newsObject[newsObjIndex].headline[newsObject[newsObjIndex].currentIndex] = actualResponseValue[i];
		newsObject[newsObjIndex].currentIndex++;
		i++;
	}
	newsObject[newsObjIndex].headline[newsObject[newsObjIndex].currentIndex] = '\0';
}

void parseTime()
{
	char hour[2];
	char min[2];
	hour[0] = actualResponseValue[10];
	hour[1] = actualResponseValue[11];
	min[0] = actualResponseValue[12];
	min[1] = actualResponseValue[13];
	timeObject.hour = atoi(hour);
	timeObject.min = atoi(min);
	
}

void unHex(uint8_t *buf)
{
	int i = 8, j = 8;
	do
	{
		if(buf[j] >= 'A' && buf[j] <= 'F')
		{
			buf[j] = buf[j] - 'A' + 10;
		}
		else
		{
			buf[j] = buf[j] - '0';
		}
		
		if(buf[j + 1] >= 'A' && buf[j + 1] <= 'F')
		{
			buf[j + 1] = buf[j + 1] - 'A' + 10;
		}
		else
		{
			buf[j + 1] = buf[j + 1] - '0';
		}
		
		buf[i] = 16 * buf[j] + buf[j+1];
		j= j + 2;
	}while(buf[i++] != '!');
}

void parseGesture()
{
	
	if(actualResponseValue[10] == 'L')
	{
		if(screenTrackCounter != 0)
		{
			screenTrackCounter--;
			if(screenTrackCounter == 0)
			{
				timeChangeFlag = true;
			}
			else if(screenTrackCounter == 1)
			{
				weatherChangeFlag = true;
			}
			else if(screenTrackCounter == 2)
			{
				newsChangeFlag = true;
			}
		}
	}
	else if(actualResponseValue[10] == 'R')
	{
		if(screenTrackCounter != 2)
		{
			screenTrackCounter++;
			if(screenTrackCounter == 0)
			{
				timeChangeFlag = true;
			}
			else if(screenTrackCounter == 1)
			{
				weatherChangeFlag = true;
			}
			else if(screenTrackCounter == 2)
			{
				newsChangeFlag = true;
			}
		}
		
	}
	else if(actualResponseValue[10] == 'T')
	{
		isScreenOn = !isScreenOn;
		if(!isScreenOn)
		{
			fill_color(BLACK);
		}
		else
		{
			if(screenTrackCounter == 0)
			{
				timeChangeFlag = true;
			}
			else if(screenTrackCounter == 1)
			{
				weatherChangeFlag = true;
			}
			else if(screenTrackCounter == 2)
			{
				newsChangeFlag = true;
			}
			
		}
		
	}
}

void parseResponse()
{
	unHex(actualResponseValue);
	if(actualResponseValue[8] == 'W')
	{
		parseWeather();
		weatherChangeFlag = true;
	}
	
	else if(actualResponseValue[8] == 'N' || actualResponseValue[8] == 'n')
	{
		parseNews();
		newsChangeFlag = true;
	}
	
	else if(actualResponseValue[8] == 'T')
	{
		parseTime();
		timeChangeFlag = true;
	}
	else if(actualResponseValue[8] == 'G')
	{
		parseGesture();
	}
	else if(actualResponseValue[8] == 'D')
	{
		doneFlag = true;
		if(screenTrackCounter == 0)
		{
			timeChangeFlag = true;
		}
		else if(screenTrackCounter == 1)
		{
			weatherChangeFlag = true;
		}
		else if(screenTrackCounter == 2)
		{
			newsChangeFlag = true;
		}
	}
	
}


void BLUE_Process_Command()
{
	// We are only interested in the WV,001C,W command
	if (rx_buffer[0] == 'W' && rx_buffer[1] == 'V')
	{
		actualResponseValue = rx_buffer;
		parseResponse();
	}
}

//! [callback_funcs]
void usart_read_callback(struct usart_module *const usart_module)
{
	check_end_of_string = rx_buffer_read[0];
	if(rx_buffer_read[0] != '\n')
	{
		rx_buffer[current_Rx_buffer_position++] = rx_buffer_read[0];
	}
	
	else if(check_end_of_string == '\n') //&& rx_buffer[current_Rx_buffer_position-2] == '\r')
	{
		check_end_of_string = ' ';
		readCharLock = 0;
		BLUE_Process_Command();
		clearBuffer();
	}
}

void usart_write_callback(struct usart_module *const usart_module)
{
}
//! [callback_funcs]

//! [setup]
void configure_usart(void)
{
	//! [setup_config]
	struct usart_config config_usart;
	//! [setup_config]
	//! [setup_config_defaults]
	usart_get_config_defaults(&config_usart);
	//! [setup_config_defaults]

	//! [setup_change_config]
	config_usart.baudrate    = 115200;
	config_usart.mux_setting = USART_RX_3_TX_2_XCK_3/*EDBG_CDC_SERCOM_MUX_SETTING*/;
	config_usart.pinmux_pad2 = MICRO_BLE_TX;//EDBG_CDC_SERCOM_PINMUX_PAD2; //24
	config_usart.pinmux_pad3 = MICRO_BLE_RX;//EDBG_CDC_SERCOM_PINMUX_PAD3; //25
	//! [setup_change_config]

	//! [setup_set_config]
	while (usart_init(&usart_instance,
	SERCOM3/*EDBG_CDC_MODULE*/ , &config_usart) != STATUS_OK) {
	}
	
	struct port_config pin_conf_wake_sw;
	port_get_config_defaults(&pin_conf_wake_sw);
	pin_conf_wake_sw.direction  = PORT_PIN_DIR_OUTPUT;
	port_pin_set_config(WAKE_SW, &pin_conf_wake_sw); 
	
	struct port_config pin_conf_wake_hw;
	port_get_config_defaults(&pin_conf_wake_hw);
	pin_conf_wake_hw.direction  = PORT_PIN_DIR_OUTPUT;
	port_pin_set_config(WAKE_HW, &pin_conf_wake_hw);
	
	port_pin_set_output_level(WAKE_HW, false);
	port_pin_set_output_level(WAKE_SW, true);
	delay_ms(1000);
	
	//! [setup_set_config]

	//! [setup_enable]
	usart_enable(&usart_instance);
	//! [setup_enable]
}

void configure_usart_callbacks(void)
{
	//! [setup_register_callbacks]
	usart_register_callback(&usart_instance,
	usart_write_callback, USART_CALLBACK_BUFFER_TRANSMITTED);
	usart_register_callback(&usart_instance,
	usart_read_callback, USART_CALLBACK_BUFFER_RECEIVED);
	//! [setup_register_callbacks]

	//! [setup_enable_callbacks]
	usart_enable_callback(&usart_instance, USART_CALLBACK_BUFFER_TRANSMITTED);
	usart_enable_callback(&usart_instance, USART_CALLBACK_BUFFER_RECEIVED);
	//! [setup_enable_callbacks]
}
//! [setup]
void initializeUSART()
{
	//! [setup_init]
	configure_usart();
	configure_usart_callbacks();
	//! [setup_init]

	//! [main]
	//! [enable_global_interrupts]
	system_interrupt_enable_global();
	//! [enable_global_interrupts]
	
	//news and weather initializations
	for(int i = 0; i < 3; i++)
	{
		newsObject[i].currentIndex = 0;
		newsObject[i].headline[0] = '\0';
		weatherObject[i].typeWeather[0] = '\0';
	}

	//! [main_send_string]
	string_inOp = string_Reboot;

	usart_write_buffer_wait(&usart_instance, string_inOp, sizeof(string_Reboot) - 1);
	read_char();
	delay_ms(100000);
	string_inOp = string_Plus;
	usart_write_buffer_wait(&usart_instance, string_Plus, sizeof(string_Plus) - 1);
	read_char();	

	//read_char();
	//delay_ms(100000);
	/*while(1)
	{
		string_inOp = string_Plus;
		usart_write_buffer_wait(&usart_instance, string_Plus, sizeof(string_Plus) - 1);
		read_char();
		delay_ms(1000);
	}*/
	delay_ms(1000);
	string_inOp = string_SF;
	usart_write_buffer_wait(&usart_instance, string_SF, sizeof(string_SF) - 1);
	//read_char();
	delay_ms(1000);
	string_inOp = string_SS;
	usart_write_buffer_wait(&usart_instance, string_SS, sizeof(string_SS) - 1);
	//read_char();
	delay_ms(1000);
	string_inOp = string_SR;
	usart_write_buffer_wait(&usart_instance, string_SR, sizeof(string_SR) - 1);
	//read_char();
	delay_ms(1000);
	string_inOp = string_Reboot;
	usart_write_buffer_wait(&usart_instance, string_Reboot, sizeof(string_Reboot) - 1);
	//read_char();
	delay_ms(100000);
	string_inOp = string_SS_1;
	usart_write_buffer_wait(&usart_instance, string_SS_1, sizeof(string_SS_1) - 1);
	//read_char();
	delay_ms(1000);
	string_inOp = string_PZ;
	usart_write_buffer_wait(&usart_instance, string_PZ, sizeof(string_PZ) - 1);
	//read_char();
	delay_ms(1000);
	string_inOp = string_PS;
	usart_write_buffer_wait(&usart_instance, string_PS, sizeof(string_PS) - 1);
	//read_char();
	delay_ms(1000);
	string_inOp = string_PC_0;
	usart_write_buffer_wait(&usart_instance, string_PC_0, sizeof(string_PC_0) - 1);
	//read_char();
	delay_ms(1000);
	string_inOp = string_PC_1;
	usart_write_buffer_wait(&usart_instance, string_PC_1, sizeof(string_PC_1) - 1);
	//read_char();
	delay_ms(1000);
	string_inOp = string_U;
	usart_write_buffer_wait(&usart_instance, string_U, sizeof(string_U) - 1);
	//read_char();
	delay_ms(1000);
	string_inOp = string_Plus;
	usart_write_buffer_wait(&usart_instance, string_Plus, sizeof(string_Plus) - 1);
	//read_char();
	delay_ms(1000);
	string_inOp = string_Reboot;
	usart_write_buffer_wait(&usart_instance, string_Reboot, sizeof(string_Reboot) - 1);
	//read_char();
	//read_char();
	delay_ms(100000);
	//usart_write_buffer_wait(&usart_instance, string_LS, sizeof(string_LS));
	//delay_ms(1000);
	string_inOp = string_A;
	usart_write_buffer_wait(&usart_instance, string_A, sizeof(string_A) - 1);
	//read_char();
	delay_ms(1000);
	string_inOp = string_Plus;
	usart_write_buffer_wait(&usart_instance, string_Plus, sizeof(string_Plus) - 1);
	//read_char();
	delay_ms(10000);
	string_inOp = string_SUW;
	usart_write_buffer_wait(&usart_instance, string_SUW, sizeof(string_SUW) - 1);
	delay_ms(10000);
	//read_char();
	string_inOp = string_SN;
	usart_write_buffer_wait(&usart_instance, string_SN, sizeof(string_SN) - 1);
	delay_ms(1000);
	//read_char();
	//usart_write_buffer_wait(&usart_instance, stringLs, sizeof(stringLs));
	screenTrackCounter = 0;
	isScreenOn = true;
}

#endif /* BLE_H_ */