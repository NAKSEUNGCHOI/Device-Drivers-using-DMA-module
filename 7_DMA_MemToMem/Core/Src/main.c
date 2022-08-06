/**
 *	main.c
 *	@brief Learn how to transfer data from memory to memory with DMA.
 *  @author Nakseung Choi
 *  @date 08-06-2022
 */

#include "stm32f4xx.h"
#include <stdio.h>
#include "dma.h"
#include "uart.h"

#define BUFFER_SIZE					5

uint16_t sensor_data_arr[BUFFER_SIZE] = {892,731,1234,90,23};
uint16_t temp_data_arr[BUFFER_SIZE];

volatile uint8_t g_transfer_complete;

int main(void){

	uart2_tx_init();
	DMA2_mem2mem_init();
	DMA2_transfer_start((uint32_t)sensor_data_arr, (uint32_t)temp_data_arr, BUFFER_SIZE);
	while(!(g_transfer_complete)){}

	for(int i = 0; i < BUFFER_SIZE; i++){
		printf("Temp buffer[%d]: %d\r\n", i, temp_data_arr[i]);
	}
	g_transfer_complete = 0;


	while(1){

	}


}

void DMA2_Stream0_IRQHandler(void){

	/*1. Check if transfer complete interrupt occurred */
	if(DMA2->LISR & DMA_LISR_TCIF0){
		g_transfer_complete = 1;
		DMA2->LIFCR |= DMA_LIFCR_CTCIF0;
	}

	/*2. Check if transfer error interrupt occurred. */
	if(DMA2->LISR & DMA_LISR_TEIF0){

		/*3. Do something */

		/*4. Clear flag */
		DMA2->LIFCR |= DMA_LIFCR_CTEIF0;

	}
}


