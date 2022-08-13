/**
 *	spi_dma.c
 *	@brief spi dma configuration
 *  @date Aug 12, 2022
 *  @author Nakseung Choi
 */
#include "spi_dma.h"

/**
 *
 * PA5	:	SPI1_SCK
 * PA6	:	SPI1_MISO
 * PA7	:   SPI1_MOSI
 *
 */

void spi1_dma_init(void){

	/***************** GPIO Configuration *****************/
	/*1. Enable clock access to PORT of SPI pins */
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

	/*2. Set SPI pins mode to alternate function mode */
	GPIOA->MODER |= GPIO_MODER_MODE5_1 | GPIO_MODER_MODE6_1 | GPIO_MODER_MODE7_1;

	/*3. Set alternate type to AF5(SPI1) */
	GPIOA->AFR[0] |= GPIO_AFRL_AFRL5_0 | GPIO_AFRL_AFRL5_2;
	GPIOA->AFR[0] |= GPIO_AFRL_AFRL6_0 | GPIO_AFRL_AFRL6_2;
	GPIOA->AFR[0] |= GPIO_AFRL_AFRL7_0 | GPIO_AFRL_AFRL7_2;

	/***************** SPI Configuration *****************/
	/*4. Enable clock access to SPI1 module. */
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

	/*5. Set software slave management */
	SPI1->CR1 |= SPI_CR1_SSM | SPI_CR1_SSI;

	/*6. Set SPI to master mode */
	SPI1->CR1 |= SPI_CR1_MSTR;

	/*7. Set CPHA and CPOL */
	SPI1->CR1 |= SPI_CR1_CPHA | SPI_CR1_CPOL;

	/*8. Set clock divider ( fPLCK/4 ) */
	SPI1->CR1 |= SPI_CR1_BR_0;

	/*9. Select to use DMA */
	SPI1->CR2 |= SPI_CR2_RXDMAEN | SPI_CR2_TXDMAEN;

	/*10. Enable SPI. */
	SPI1->CR1 |= SPI_CR1_SPE;

}
void dma2_stream3_spi_tx_init(void){
	/***********DMA configuration***********/
	/*1. Enable clock access to DMA*/
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;

	/*2. Disable DMA stream */
	DMA2_Stream3->CR &= ~DMA_SxCR_EN;

	/*3. wait until DMA stream is disabled */
	while(DMA2_Stream3->CR & DMA_SxCR_EN){}

	/***********configure DMA parameters***********/

	/*4. Enable memory increment */
	DMA2_Stream3->CR |= DMA_SxCR_MINC;

	/*5. Set transfer direction; (memory to peripheral for transmitter) */
	DMA2_Stream3->CR |= DMA_SxCR_DIR_0;
	DMA2_Stream3->CR &= ~DMA_SxCR_DIR_1;

	/*6. Enable transfer complete interrupt */
	DMA2_Stream3->CR |= DMA_SxCR_TCIE;

	/*7. Enable transfer error interrupt */
	DMA2_Stream3->CR |= DMA_SxCR_TEIE;

	/*8. Disable direct mode */
	DMA2_Stream3->FCR |= DMA_SxFCR_DMDIS;

	/*9. Set DMA FIFO threshold; Full */
	DMA2_Stream3->FCR |= DMA_SxFCR_FTH;

	/*10. Enable DMA interrupt in the NVIC*/
	NVIC_EnableIRQ(DMA2_Stream3_IRQn);

}
void dma2_stream2_spi_rx_init(void){
	/*0. Enable clock access to DMA*/
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;

	/*1. Disable DMA */
	DMA2_Stream2->CR &= ~DMA_SxCR_EN;

	/*2. wait until DMA stream is disabled */
	while(DMA2_Stream2->CR & DMA_SxCR_EN){}

	/***********configure DMA parameters***********/
	/*3. Enable memory increment */
	DMA2_Stream2->CR |= DMA_SxCR_MINC;

	/*4. Set transfer direction; (peripheral to memory for receiver) */
	DMA2_Stream2->CR &= ~DMA_SxCR_DIR_0;
	DMA2_Stream2->CR &= ~DMA_SxCR_DIR_1;

	/*5. Set transfer error interrupt flag*/
	DMA2_Stream2->CR |= DMA_SxCR_TCIE;

	/*6. Set transfer complete interrupt flag*/
	DMA2_Stream2->CR |= DMA_SxCR_TEIE;

	/*7. Disable direct mode */
	DMA2_Stream2->FCR |= DMA_SxFCR_DMDIS;

	/*8. Set DMA FIFO threshold; Full */
	DMA2_Stream2->FCR |= DMA_SxFCR_FTH;

	/*9. Enable DMA stream2 interrupt in the NVIC*/
	NVIC_EnableIRQ(DMA2_Stream2_IRQn);

}

void dma2_stream3_spi_transfer(uint32_t msg_to_snd, uint32_t length){

	/*1. Clear interrupt flags*/
	DMA2->LIFCR |= DMA_LIFCR_CTCIF3;
	DMA2->LIFCR |= DMA_LIFCR_CTEIF3;

	/*2. Set peripheral address */
	DMA2_Stream3->PAR = (uint32_t)(&SPI1->DR);

	/*3. Set memory address*/
	DMA2_Stream3->M0AR = msg_to_snd;

	/*4. Set transfer length */
	DMA2_Stream3->NDTR |= length;

	/*5. Enable DMA stream */
	DMA2_Stream3->CR |= DMA_SxCR_EN;

}
void dma2_stream2_spi_receive(uint32_t received_msg, uint32_t msg_length){

	/*1. Clear interrupt flags*/
	DMA2->LIFCR |= DMA_LIFCR_CTCIF2;
	DMA2->LIFCR |= DMA_LIFCR_CTEIF2;

	/*2. Set peripheral address */
	DMA2_Stream2->PAR = (uint32_t)(&SPI1->DR);

	/*3. Set memory address*/
	DMA2_Stream2->M0AR = received_msg;

	/*4. Set transfer length */
	DMA2_Stream2->NDTR |= msg_length;

	/*5. Enable DMA stream */
	DMA2_Stream2->CR |= DMA_SxCR_EN;

}
