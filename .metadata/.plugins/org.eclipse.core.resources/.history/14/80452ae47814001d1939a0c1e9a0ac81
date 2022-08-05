/**
 *	main.c
 *	@brief running MPU6050 i2c bare-metal
 *  @author Nakseung Choi
 *  @date 07-28-2022
 */

#include "stm32f4xx.h"
#include <stdio.h>
#include "uart.h"

int main(void){

	uart2_tx_init();
	static unsigned int count = 0;
	while(1){
		printf("%d. Hello from from main function! \n\r", count++);
		for(int i = 0; i < 1000000; i++){}
	}


}



