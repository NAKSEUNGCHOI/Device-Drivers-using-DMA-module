/*
 * adc_dmc.c
 *
 *  Created on: Aug 6, 2022
 *      Author: chlsk
 */

#include "adc_dma.h"

uint16_t adc_raw_data[NUM_OF_CHANNELS];

void adc_dma_init(void){

	/***********GPIO configuration***********/
	/*1. Enable clock access to ADC GPIO pin's port */
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOCEN;

	/*2. Set PA0, PA1, PA4, PB0, and PC1 as analog mode */
	GPIOA->MODER |= GPIO_MODER_MODE0 | GPIO_MODER_MODE1 | GPIO_MODER_MODE4;
	GPIOB->MODER |= GPIO_MODER_MODE0;
	GPIOC->MODER |= GPIO_MODER_MODE1;

	/***********ADC configuration***********/
	/*1. Enable clock access to ADC */
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;

	/*2. Set sequence length */
	ADC1->SQR1 |= ADC_SQR1_L_2 | ADC_SQR1_L_1;

	/*3. Set sequence */
	ADC1->SQR3 = (0U << 0) | (1U << 5) | (1U << 12) | (1U << 18) | (1U << 23) | (1U << 21) | (1U << 20);

	/*4. Enable scan mode */
	ADC1->CR1 = ADC_CR1_SCAN;

	/*5. Select to use DMA*/
	ADC1->CR2 |= ADC_CR2_DMA | ADC_CR2_DDS | ADC_CR2_CONT;

	/*6. Enable continuous mode */
	/*CONT above */


	/***********DMA configuration***********/
	/*1. Enable clock access to DMA*/
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;

	/*2. Disable DMA */
	DMA2_Stream0->CR &= ~DMA_SxCR_EN;

	/*3. wait until DMA is disabled */
	while(DMA2_Stream0->CR & DMA_SxCR_EN){}

	/*4. configure DMA parameters */
	/*4.1. Enable circular mode */
	DMA2_Stream0->CR |= DMA_SxCR_CIRC;

	/*4.2. Set memory transfer size */
	DMA2_Stream0->CR |= DMA_SxCR_MSIZE_0;

	/*4.3. Set peripheral transfer size */
	DMA2_Stream0->CR |= DMA_SxCR_PSIZE_0;

	/*4.4. Enable memory increment */
	DMA2_Stream0->CR |= DMA_SxCR_MINC;

	/*4.5. Set peripheral address*/
	DMA2_Stream0->PAR = (uint32_t)&(ADC1->DR);

	/*4.6. Set memory address */
	DMA2_Stream0->M0AR = (uint32_t)&(adc_raw_data);

	/*4.7. Set number of transfer */
	DMA2_Stream0->NDTR |= NUM_OF_CHANNELS;

	/*5. Enable DMA stream. */
	DMA2_Stream0->CR |= DMA_SxCR_EN;

	/***********ADC configuration***********/

	/*6. Enable ADC */
	ADC1->CR2 |= ADC_CR2_ADON;

	/*7. Start ADC */
	ADC1->CR2 |= ADC_CR2_SWSTART;

}

