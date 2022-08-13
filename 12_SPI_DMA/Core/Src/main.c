/**
 *	main.c
 *	@brief UART with DMA
 *  @author Nakseung Choi
 *  @date 08-11-2022
 */

#include "stm32f4xx.h"
#include <stdio.h>
#include "dma.h"
#include "adc_dma.h"
#include "uart_dma.h"
#include "string.h"

extern uint8_t g_tx_complete;
extern uint8_t g_rx_complete;
extern uint8_t g_uart_complete;

extern char uart_data_buffer[USART_DATA_BUFF_SIZE];

char msg_buff[100] = {'\0'};

int main(void){
	uart2_rx_tx_init();
	dma1_init();
	dma1_stream5_uart_rx_config();
	sprintf(msg_buff, "Initialization Done..\n\r");
	dma1_stream6_uart_tx_config((uint32_t)msg_buff, strlen(msg_buff));

	/*
	 * Semaphore; this sets to 1 when tx is complete in uart_dma.c function
	 * If it is not equal to 1, get stuck in the while loop.
	 */
	while(!g_tx_complete){}

	while(1){
		/*
		 * Similar semaphore for rx.
		 * If rx is complete, start printing after 5 chars are typed.
		 */
		if(g_rx_complete){
			sprintf(msg_buff, "Message received : %s \r\n", uart_data_buffer);
			g_rx_complete = 0;
			g_tx_complete = 0;
			g_uart_complete = 0;
			dma1_stream6_uart_tx_config((uint32_t)msg_buff, strlen(msg_buff));
			while(!g_tx_complete){}
		}
	}
}



