/**
 *	main.c
 *	@brief running MPU6050 i2c bare-metal
 *  @author Nakseung Choi
 *  @date 07-28-2022
 */
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "stm32f4xx.h"

char _a[1] = {'A'};
char *pt_a = _a;

char hello[] = "Hello from main";
char *p_hello = hello;

int main(void){

	for(int i = 0; i < strlen(hello); i++){
		ITM_SendChar(p_hello[i]);
	}

	while(1){

	}


}



