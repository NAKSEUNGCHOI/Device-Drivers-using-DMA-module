/*
 * swo.c
 *
 *  Created on: Aug 4, 2022
 *      Author: chlsk
 */

#include "stm32f4xx.h"
#include <stdio.h>
#include "swo.h"

int __io_putchar(int ch){
	ITM_SendChar(ch);
	return ch;
}

void log_error(char *p){
	printf("Log Error: ");
	printf((char*)p);
	printf("\r\n");
}
void log_info(char *p){
	printf("Log Info: ");
	printf((char*)p);
	printf("\r\n");
}
void log_debug(char *p){
	printf("Log Debug: ");
	printf((char*)p);
	printf("\r\n");
}
