/**
 *	main.c
 *	@brief running MPU6050 i2c bare-metal
 *  @author Nakseung Choi
 *  @date 07-28-2022
 */
#include <stdio.h>
#include <stdint.h>
#include "stm32f4xx.h"
#include "MPU6050.h"
#include "i2c.h"

#define GPIOAEN (1U << 0)
#define PIN5	(1U << 5)
#define LED_PIN (PIN5)

void blinky_init(void);

int main(void){

	/*1. Set clock access to GPIOA*/
	blinky_init();

	while(1){
		GPIOA->ODR ^= LED_PIN;
		for(int i = 0; i < 1000000; i++){}
	}


}
void blinky_init(void){
	RCC->AHB1ENR |= GPIOAEN;
	GPIOA->MODER |= (1U << 10);
	GPIOA->MODER &= ~(1U << 11);
}


