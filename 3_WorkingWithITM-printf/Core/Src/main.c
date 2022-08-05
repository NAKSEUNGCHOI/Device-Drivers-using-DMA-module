/**
 *	main.c
 *	@brief build a printf function using ITM and debug with it.
 *  @author Nakseung Choi
 *  @date 08-04-2022
 */
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "stm32f4xx.h"


char _a[1] = {'A'};
char *pt_a = _a;

char hello[] = "Hello from main";
char *p_hello = hello;

int __io_putchar(int ch){
	ITM_SendChar(ch);
	return ch;
}

int main(void){

	printf("printf is now available.\n");

	while(1){

	}


}



