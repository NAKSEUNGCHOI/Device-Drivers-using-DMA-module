/*
 * uart_dma.c
 *
 *  Created on: Aug 11, 2022
 *      Author: chlsk
 */
#include "uart_dma.h"

#define UART_BAUDRATE 		115200
#define CLK					16000000

char uart_data_buffer[USART_DATA_BUFF_SIZE];

/* Semaphore */
uint8_t g_rx_complete;
uint8_t g_tx_complete;
uint8_t g_uart_complete;

static void uart_set_baudrate(uint32_t periph_clk, uint32_t baudrate);
static uint16_t compute_uart_baudrate(uint32_t periph_clk, uint32_t baudrate);

void uart2_rx_tx_init(void){

	/*1. Set clock access to GPIOA*/
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

	/*2. Set PA2 and PA3 mode to alternate function */
	GPIOA->MODER |= GPIO_MODER_MODE2_1 | GPIO_MODER_MODE3_1;
	GPIOA->MODER &= ~GPIO_MODER_MODE2_0;
	GPIOA->MODER &= ~GPIO_MODER_MODE3_0;

	/*3. Set PA2 and PA 3 alternate function type to AF7 (UART2_TX) */
	GPIOA->AFR[0] |= GPIO_AFRL_AFRL2_0 | GPIO_AFRL_AFRL2_1 | GPIO_AFRL_AFRL2_2;
	GPIOA->AFR[0] &= ~GPIO_AFRL_AFRL2_3;
	GPIOA->AFR[0] |= GPIO_AFRL_AFRL3_0 | GPIO_AFRL_AFRL3_1 | GPIO_AFRL_AFRL3_2;
	GPIOA->AFR[0] &= ~GPIO_AFRL_AFRL3_3;

	/*4. Enable clock access to UART2 */
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

	/*5. Set baudrate */
	uart_set_baudrate(CLK, UART_BAUDRATE);

	/*6. Select to use DMA */
	USART2->CR3 |= USART_CR3_DMAT | USART_CR3_DMAR;

	/*7. Set transfer direction */
	USART2->CR1 = USART_CR1_TE | USART_CR1_RE;

	/*8. Clear TC flag (for UART interrupt clear flag) */
	USART2->SR |= USART_SR_TC;

	/*9. Enable TCIE transfer complete interrupt */
	USART2->CR1 |= USART_CR1_TCIE;

	/*10 Enable UART Module */
	USART2->CR1 |= USART_CR1_UE;

	/*11. Enable USART2 in NVIC */
	NVIC_EnableIRQ(USART2_IRQn);

}

void dma1_init(void){
	/*1. Enable clock access to dma1*/
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;

	/*2. Enable DMA stream6 interrupt in NVIC*/
	NVIC_EnableIRQ(DMA1_Stream6_IRQn);
}
void dma1_stream5_uart_rx_config(void){
	/*1.. Disable DMA */
	DMA1_Stream5->CR &= ~DMA_SxCR_EN;

	/*2. wait until DMA is disabled */
	while(DMA1_Stream5->CR & DMA_SxCR_EN){}

	/*3. Clear interrupt flags for stream 5*/
	/*3.1. direct mode error interrupt flag */
	DMA1->HIFCR |= DMA_HIFCR_CDMEIF5;

	/*3.2. clear transfer error interrupt flag*/
	DMA1->HIFCR |= DMA_HIFCR_CTEIF5;

	/*3.3. transfer complete interrupt flag*/
	DMA1->HIFCR |= DMA_HIFCR_CTCIF5;

	/*4. configure DMA parameters */
	/*4.1. Set peripheral address*/
	DMA1_Stream5->PAR = (uint32_t)(&USART2->DR);

	/*4.2. Set memory address */
	DMA1_Stream5->M0AR = (uint32_t)(&uart_data_buffer);

	/*4.3. Set number of transfer */
	DMA1_Stream5->NDTR |= USART_DATA_BUFF_SIZE;

	/*4.4. Select Channel 4 */
	DMA1_Stream5->CR |= DMA_SxCR_CHSEL_2;

	/*4.5. Enable memory increment */
	DMA1_Stream5->CR |= DMA_SxCR_MINC;

	/*4.6. Set transfer complete interrupt */
	DMA1_Stream5->CR |= DMA_SxCR_TCIE;

	/*5. Enable circular mode */
	DMA1_Stream5->CR |= DMA_SxCR_CIRC;

	/*6. Set transfer direction; (peripheral to memory for receiver) */
	DMA1_Stream5->CR &= ~DMA_SxCR_DIR_0;
	DMA1_Stream5->CR &= ~DMA_SxCR_DIR_1;

	/*7. Enable DMA stream. */
	DMA1_Stream5->CR |= DMA_SxCR_EN;

	/*8. Enable DMA stream5 interrupt in NVIC*/
	NVIC_EnableIRQ(DMA1_Stream5_IRQn);


}
void dma1_stream6_uart_tx_config(uint32_t msg_to_snd, uint32_t msg_len){
	/*1. Disable DMA */
	DMA1_Stream6->CR &= ~DMA_SxCR_EN;

	/*2. wait until DMA is disabled */
	while(DMA1_Stream6->CR & DMA_SxCR_EN){}

	/*3. Clear interrupt flags for stream 5*/
	/*3.1. direct mode error interrupt flag */
	DMA1->HIFCR |= DMA_HIFCR_CDMEIF6;

	/*3.2. clear transfer error interrupt flag*/
	DMA1->HIFCR |= DMA_HIFCR_CTEIF6;

	/*3.3. transfer complete interrupt flag*/
	DMA1->HIFCR |= DMA_HIFCR_CTCIF6;

	/*4.1. Set peripheral address*/
	DMA1_Stream6->PAR = (uint32_t)(&USART2->DR);

	/*4.2. Set memory address */
	DMA1_Stream6->M0AR = (msg_to_snd);

	/*4.3. Set number of transfer */
	DMA1_Stream6->NDTR |= msg_len;

	/*4.4. Select Channel 4 */
	DMA1_Stream6->CR |= DMA_SxCR_CHSEL_2;

	/*4.5. Enable memory increment */
	DMA1_Stream6->CR |= DMA_SxCR_MINC;

	/*5. Set transfer direction; (memory to peripheral for transmitter) */
	DMA1_Stream6->CR |= DMA_SxCR_DIR_0;
	DMA1_Stream6->CR &= ~DMA_SxCR_DIR_1;

	/*6. Set transfer complete interrupt */
	DMA1_Stream6->CR |= DMA_SxCR_TCIE;

	/*6. Enable DMA stream. */
	DMA1_Stream6->CR |= DMA_SxCR_EN;

}
static uint16_t compute_uart_baudrate(uint32_t periph_clk, uint32_t baudrate){
	return ((periph_clk + (baudrate / 2U)) / baudrate);
}
static void uart_set_baudrate(uint32_t periph_clk, uint32_t baudrate){
	USART2->BRR = compute_uart_baudrate(periph_clk, baudrate);
}
void DMA1_Stream5_IRQHandler(void){
	if(DMA1->HISR & DMA_HISR_TCIF5){
		/*1. Do something.... i.g. turn a led on */
		g_rx_complete = 1;

		/*2. Clear the flag */
		DMA1->HIFCR |= DMA_HIFCR_CTCIF5;

	}
}
void DMA1_Stream6_IRQHandler(void){
	if(DMA1->HISR & DMA_HISR_TCIF6){
		/*1. Do something.... i.g. turn a led on */
		g_tx_complete = 1;

		/*2. Clear the flag */
		DMA1->HIFCR |= DMA_HIFCR_CTCIF6;

	}
}
void USART2_IRQHandler(void){
	/*1. Do something.... i.g. turn a led on */
	g_uart_complete = 1;

	/*2. Clear the flag */
	USART2->SR &= ~USART_SR_TC;
}






