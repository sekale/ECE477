#include <stdio.h>

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
#define MAX_RX_BUFFER_LENGTH   5

volatile uint8_t rx_buffer[MAX_RX_BUFFER_LENGTH];
//! [rx_buffer_var]

//! [callback_funcs]
void usart_read_callback(struct usart_module *const usart_module)
{
	
	//usart_write_buffer_job(&usart_instance,
	//(uint8_t *)rx_buffer, MAX_RX_BUFFER_LENGTH);
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
	
	//BLE Initializations
	usart_write_buffer_wait(&usart_instance, string_Reboot, sizeof(string_Reboot) - 1);
	delay_ms(100000);
	usart_write_buffer_wait(&usart_instance, string_Plus, sizeof(string_Plus) - 1);
	delay_ms(1000);
	usart_write_buffer_wait(&usart_instance, string_SF, sizeof(string_SF) - 1);
	delay_ms(1000);
	usart_write_buffer_wait(&usart_instance, string_SS, sizeof(string_SS) - 1);
	delay_ms(1000);
	usart_write_buffer_wait(&usart_instance, string_SR, sizeof(string_SR) - 1);
	delay_ms(1000);
	usart_write_buffer_wait(&usart_instance, string_Reboot, sizeof(string_Reboot) - 1);
	delay_ms(100000);
	usart_write_buffer_wait(&usart_instance, string_SS_1, sizeof(string_SS_1) - 1);
	delay_ms(1000);
	usart_write_buffer_wait(&usart_instance, string_PZ, sizeof(string_PZ) - 1);
	delay_ms(1000);
	usart_write_buffer_wait(&usart_instance, string_PS, sizeof(string_PS) - 1);
	delay_ms(1000);
	usart_write_buffer_wait(&usart_instance, string_PC_0, sizeof(string_PC_0) - 1);
	delay_ms(1000);
	usart_write_buffer_wait(&usart_instance, string_PC_1, sizeof(string_PC_1) - 1);
	delay_ms(1000);
	usart_write_buffer_wait(&usart_instance, string_U, sizeof(string_U) - 1);
	delay_ms(1000);
	usart_write_buffer_wait(&usart_instance, string_Reboot, sizeof(string_Reboot) - 1);
	delay_ms(100000);
	//usart_write_buffer_wait(&usart_instance, string_LS, sizeof(string_LS));	
	//delay_ms(1000);
	usart_write_buffer_wait(&usart_instance, string_A, sizeof(string_A) - 1);
	delay_ms(1000);
	usart_write_buffer_wait(&usart_instance, string_Plus, sizeof(string_Plus) - 1);
	delay_ms(1000);
	usart_write_buffer_wait(&usart_instance, string_SUW, sizeof(string_SUW) - 1);
	delay_ms(1000);
	//usart_write_buffer_wait(&usart_instance, stringLs, sizeof(stringLs));	
}




#endif /* USART_H_ */