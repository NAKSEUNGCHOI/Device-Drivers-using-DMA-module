/**
 *	main.c
 *	@brief Learn how to debug using SWV and the on-board button PC13.
 *  @author Nakseung Choi
 *  @date 08-04-2022
 */

#include "stm32f4xx.h"
#include <stdio.h>
#include "uart.h"

uint8_t button_state;

int main(void){

	/*1. Enable clock access to GPIOC for button PC13 */
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;

	/*2. Set PC13 as input pin*/
	GPIOC->MODER &= ~(GPIO_MODER_MODE13_0);
	GPIOC->MODER &= ~(GPIO_MODER_MODE13_1);

	while(1){
		/*Check if BTN is pressed or not */
		/*Push button is active low*/
		if(GPIOC->IDR & GPIO_IDR_ID13){
			button_state = 0;
		}else{
			button_state = 1;
		}
		for(int i = 0; i < 90000; i++){}
	}


}



