#include <stdio.h>
#include <string.h>

#ifndef USART_H_
#define USART_H_

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
uint8_t *string_inOp;
int flag = 1;
static int current_Rx_buffer_position = 0;
static char check_end_of_string = 's';

//string send and responses//

	uint8_t string_Plus[] = "+\r";
	uint8_t string_Reboot[] = "R,1\r";
	uint8_t string_LS[] = "LS\r";
	uint8_t string_SF[] = "SF,1\r";
	uint8_t string_SS[] = "SS,C0000000\r";
	uint8_t string_SR[] = "SR,00000000\r";
	uint8_t string_SS_1[] = "SS,C0000001\r";
	uint8_t string_PZ[] = "PZ\r";
	uint8_t string_PS[] = "PS,11223344556677889900AABBCCDDEEFF\r";
	uint8_t string_PC_0[] = "PC,010203040506070809000A0B0C0D0E0F,02,05\r";
	uint8_t string_PC_1[] = "PC,111213141516171819101A1B1C1D1E1F,18,02\r";
	uint8_t string_U[] = "U\r";
	uint8_t string_SUW[] = "SUW,0102030405060708090A0B0C0D0E0F,1234\r";
	uint8_t string_A[] = "A\r";
	uint8_t string_SSReponse[] = "AOK\r\n";
	uint8_t string_RebootResponse[] = "Reboot\r\nCMD\r\n";

volatile uint8_t rx_buffer[MAX_RX_BUFFER_LENGTH];
volatile uint8_t rx_buffer_read[1];
//! [rx_buffer_var]

bool compareResponse(uint8_t *currentResponse, uint8_t *expectedResponse)
{
	return true;
}
	
void parseResponse()
{
	bool response;
	if(string_inOp == string_Reboot)
	{
		response = compareResponse(rx_buffer, string_RebootResponse);
	}
		
	else if(string_inOp == string_SF || string_inOp == string_SS
	|| string_inOp == string_SR || string_inOp == string_SS_1
	|| string_inOp == string_PZ || string_inOp == string_PS
	|| string_inOp == string_PC_0 || string_inOp == string_PC_1
	|| string_inOp == string_A || string_inOp == string_PS)
	{
		response = compareResponse(rx_buffer, string_SSReponse);
	}
		
	else if(string_inOp == string_SUW)
	{
			
	}
		
}
	
void read_char()
{
	flag = 1;
	while(flag == 1)
	{	
		while(usart_read_buffer_job(&usart_instance, rx_buffer_read, 1) != STATUS_OK);
	}
}

//! [callback_funcs]
void usart_read_callback(struct usart_module *const usart_module)
{
	rx_buffer[current_Rx_buffer_position] = rx_buffer_read[0];
	check_end_of_string = rx_buffer_read[0];
	current_Rx_buffer_position++;
		
	if(check_end_of_string == '\n')
	{
		check_end_of_string = '\0';
		uint8_t *valuecrap = rx_buffer;
		flag = 0;
		valuecrap[0] = 's';
		valuecrap[0] = 'e';

		for(int i = 0; i < current_Rx_buffer_position; ++i)
		{
			rx_buffer[i] = 0;
		}
		current_Rx_buffer_position = 0;
	}	
/*	int i = 0;
	if(check_end_of_string == '\n')
	{
		//comparing one less since we send string with a \r but we receive uint8_t terminated with a \r\n
		int length_to_compare = (sizeof(rx_buffer)/sizeof(uint8_t)) - 1;
		//compare rxbuffer and stringinop values
		if(memcmp(rx_buffer, string_inOp, length_to_compare) != 0)
		{
			flag = 0;
			parseResponse();
		}
	}*/
}

void usart_write_callback(struct usart_module *const usart_module)
{
	port_pin_toggle_output_level(LED_0_PIN);
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
	config_usart.pinmux_pad2 = PINMUX_PB14C_SERCOM4_PAD2;//EDBG_CDC_SERCOM_PINMUX_PAD2;
	config_usart.pinmux_pad3 = PINMUX_PB15C_SERCOM4_PAD3;//EDBG_CDC_SERCOM_PINMUX_PAD3;
	//! [setup_change_config]

	//! [setup_set_config]
	while (usart_init(&usart_instance,
	SERCOM4/*EDBG_CDC_MODULE*/ , &config_usart) != STATUS_OK) {
	}
	
	port_pin_set_output_level(EXT2_PIN_9/*PINMUX_PB15C_SERCOM4_PAD3*/, true);
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

	//! [main_send_string]
	//uint8_t string[] = "Sucks!\r\n";
	//usart_write_buffer_wait(&usart_instance, string, sizeof(string));	
	

	

	
	uint8_t string_PlusResponse[] = "Echo On\r\n";
	
	//uint8_t string_RebootResponse[] = "Reboot\r\n";
	//uint8_t string_LSResponse[] = "LS\r";
	//uint8_t string_SFResponse[] = "AOK\r\n";

	//uint8_t string_SRResponse[] = "AOK\r\n";//
	//uint8_t string_SS_1Response[] = "AOK\r\n";
	//uint8_t string_PZResponse[] = "AOK\r\n";
	//uint8_t string_PSResponse[] = "AOK\r\n";
	//uint8_t string_PC_0Response[] = "AOK\r\n";
	//uint8_t string_PC_1Response[] = "AOK\r\n";
	uint8_t string_UResponse[] = "ERR\r\n";
	uint8_t string_SUWResponse[] = "SUW,0102030405060708090A0B0C0D0E0F,1234\rERR\r\n";
	//uint8_t string_AResponse[] = "AOK\r\n";
	

	//usart_write_buffer_job(&usart_instance,
	//(uint8_t *)rx_buffer, MAX_RX_BUFFER_LENGTH);
	string_inOp = string_Reboot;
	usart_write_buffer_wait(&usart_instance, string_inOp, sizeof(string_Reboot) - 1);
	read_char();
	read_char();
	delay_ms(100000);
	string_inOp = string_Plus;
	usart_write_buffer_wait(&usart_instance, string_Plus, sizeof(string_Plus) - 1);
	read_char();
	delay_ms(1000);
	string_inOp = string_SF;
	usart_write_buffer_wait(&usart_instance, string_SF, sizeof(string_SF) - 1);
	read_char();
	delay_ms(1000);
	string_inOp = string_SS;
	usart_write_buffer_wait(&usart_instance, string_SS, sizeof(string_SS) - 1);
	read_char();
	delay_ms(1000);
	string_inOp = string_SR;
	usart_write_buffer_wait(&usart_instance, string_SR, sizeof(string_SR) - 1);
	read_char();
	delay_ms(1000);
	string_inOp = string_Reboot;
	usart_write_buffer_wait(&usart_instance, string_Reboot, sizeof(string_Reboot) - 1);
	read_char();
	delay_ms(100000);
	string_inOp = string_SS_1;
	usart_write_buffer_wait(&usart_instance, string_SS_1, sizeof(string_SS_1) - 1);
	read_char();
	delay_ms(1000);
	string_inOp = string_PZ;
	usart_write_buffer_wait(&usart_instance, string_PZ, sizeof(string_PZ) - 1);
	read_char();
	delay_ms(1000);
	string_inOp = string_PS;
	usart_write_buffer_wait(&usart_instance, string_PS, sizeof(string_PS) - 1);
	read_char();
	delay_ms(1000);
	string_inOp = string_PC_0;
	usart_write_buffer_wait(&usart_instance, string_PC_0, sizeof(string_PC_0) - 1);
	read_char();
	delay_ms(1000);
	string_inOp = string_PC_1;
	usart_write_buffer_wait(&usart_instance, string_PC_1, sizeof(string_PC_1) - 1);
	read_char();
	delay_ms(1000);
	string_inOp = string_U;
	usart_write_buffer_wait(&usart_instance, string_U, sizeof(string_U) - 1);
	read_char();
	delay_ms(1000);
	string_inOp = string_Plus;
	usart_write_buffer_wait(&usart_instance, string_Plus, sizeof(string_Plus) - 1);
	read_char();
	delay_ms(1000);
	string_inOp = string_Reboot;
	usart_write_buffer_wait(&usart_instance, string_Reboot, sizeof(string_Reboot) - 1);
	read_char();
	delay_ms(100000);
	//usart_write_buffer_wait(&usart_instance, string_LS, sizeof(string_LS));	
	//delay_ms(1000);
	string_inOp = string_A;
	usart_write_buffer_wait(&usart_instance, string_A, sizeof(string_A) - 1);
	read_char();
	delay_ms(1000);
	string_inOp = string_Plus;
	usart_write_buffer_wait(&usart_instance, string_Plus, sizeof(string_Plus) - 1);
	read_char();
	delay_ms(1000);
	string_inOp = string_SUW;
	usart_write_buffer_wait(&usart_instance, string_SUW, sizeof(string_SUW) - 1);
	read_char();
	delay_ms(1000);
	//usart_write_buffer_wait(&usart_instance, stringLs, sizeof(stringLs));	
}




#endif /* USART_H_ */