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
#include "adc_dma.h"


extern uint16_t adc_raw_data[NUM_OF_CHANNELS];

int main(void){

	uart2_tx_init();
	adc_dma_init();

	while(1){
		printf("Value from sensor one: %d \n\r", adc_raw_data[0]);
		printf("Value from sensor two: %d \n\r", adc_raw_data[1]);
		printf("Value from sensor three: %d \n\r", adc_raw_data[2]);
		printf("Value from sensor four: %d \n\r", adc_raw_data[3]);
		printf("Value from sensor five: %d \n\r", adc_raw_data[4]);

		for(int i = 0; i < 1000000; i++){}
	}


}


