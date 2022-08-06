/*
 * dma.c
 *
 *  Created on: Aug 6, 2022
 *      Author: chlsk
 */

#include "dma.h"

void DMA2_mem2mem_init(void){

	/*1. enable clock access to DMA2 */
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;

	/*2. Disable DMA stream */
	DMA2_Stream0->CR &= ~(DMA_SxCR_EN);

	/*3. Wait until DMA stream is disabled. */
	while(DMA2_Stream0->CR & DMA_SxCR_EN){}

	/*4. Configure dma parameters */

	/*4.1. Set Memory size = half-word (16 bit) */
	DMA2_Stream0->CR |= DMA_SxCR_MSIZE_0;

	/*4.2. Set Peripheral data size to half-word */
	DMA2_Stream0->CR |= DMA_SxCR_PSIZE_0;

	/*4.3. Enable memory increment */
	DMA2_Stream0->CR |= DMA_SxCR_MINC;

	/*4.4. Enable peripheral address increment */
	DMA2_Stream0->CR |= DMA_SxCR_PINC;

	/*4.5. Select Memory to memory transfer */
	DMA2_Stream0->CR |= DMA_SxCR_DIR_1;

	/*4.6. Enable transfer interrupt */
	DMA2_Stream0->CR |= DMA_SxCR_TCIE;

	/*4.7. */
	DMA2_Stream0->CR |= DMA_SxCR_TEIE;

	/*5. Disable direct mode */
	DMA2_Stream0->FCR |= DMA_SxFCR_DMDIS;

	/*6. Set DMA FIFO threshold */
	DMA2_Stream0->FCR |= DMA_SxFCR_FTH;

	/*7. Enable DMA interrupt in NVIC (nested vector interrupt controller) */
	NVIC_EnableIRQ(DMA2_Stream0_IRQn);

}

void DMA2_transfer_start(uint32_t src_buff, uint32_t dest_buff, uint32_t len){
	/*1. Set peripheral address */
	DMA2_Stream0->PAR = src_buff;

	/*2. Set memory address */
	DMA2_Stream0->M0AR = dest_buff;

	/*3. Set transfer length */
	DMA2_Stream0->NDTR = len;

	/*4. Enable dma stream. */
	DMA2_Stream0->CR |= DMA_SxCR_EN;

}
