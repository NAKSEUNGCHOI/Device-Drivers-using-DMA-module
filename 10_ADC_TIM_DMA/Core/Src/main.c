/**
 *	main.c
 *	@brief ADC with DMA using timer
 *  @author Nakseung Choi
 *  @date 08-06-2022
 */

#include "stm32f4xx.h"
#include <stdio.h>
#include "dma.h"
#include "uart.h"
#include "adc_dma.h"


extern uint16_t adc_raw_data[NUM_OF_SAMPLES];

volatile uint8_t g_transfer_complete;

int main(void){

	g_transfer_complete = 0;
	uart2_tx_init();
	adc_tim2_dma_init();

	while(1){
		if(g_transfer_complete){
			g_transfer_complete = 0;
			for(int i = 0; i < NUM_OF_SAMPLES; i++){
				printf("Sample number [%d]: %d\n\r", i, adc_raw_data[i]);
			}
			for(int i = 0; i < 1000000; i++){}
		}
	}
}
void DMA2_Stream0_IRQHandler(void){

	/*1. Check if transfer complete interrupt occurred */
	if(DMA2->LISR & DMA_LISR_TCIF0){
		g_transfer_complete = 1;
	/*2. Clear transfer complete interrupt flag */
		DMA2->LIFCR |= DMA_LIFCR_CTCIF0;
	}
}


