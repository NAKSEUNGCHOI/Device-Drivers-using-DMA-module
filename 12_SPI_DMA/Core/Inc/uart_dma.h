/*
 * uart_dma.h
 *
 *  Created on: Aug 11, 2022
 *      Author: chlsk
 */

#ifndef INC_UART_DMA_H_
#define INC_UART_DMA_H_
#include <stdint.h>
#include "stm32f4xx.h"

#define USART_DATA_BUFF_SIZE 			5

void uart2_rx_tx_init(void);
void dma1_init(void);
void dma1_stream5_uart_rx_config(void);
void dma1_stream6_uart_tx_config(uint32_t msg_to_snd, uint32_t msg_len);

#endif /* INC_UART_DMA_H_ */
