/*
 * adc_dmc.c
 *
 *  Created on: Aug 6, 2022
 *      Author: chlsk
 */

#include "adc_dma.h"

uint16_t adc_raw_data[NUM_OF_SAMPLES];

void adc_tim2_dma_init(void){

	/***********GPIO configuration***********/
	/*1. Enable clock access to ADC GPIO pin's port */
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

	/*2. Set PA0 as analog mode */
	GPIOA->MODER |= GPIO_MODER_MODE0;

	/***********ADC configuration***********/
	/*1. Enable clock access to ADC */
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;

	/*2. Set sequence length (Note: can skip this) */
	//ADC1->SQR1 |= ADC_SQR1_L_0;

	/*3. Set sequence (Note: can skip this b/c one channel is used )*/
	//ADC1->SQR3 = (0U << 0) | (1U << 5);

	/*4. Enable scan mode */
	//ADC1->CR1 = ADC_CR1_SCAN;

	/*5. Select to use DMA, DMA disable selection, and external trigger for regular channels (rising edge)*/
	ADC1->CR2 |= ADC_CR2_DMA | ADC_CR2_DDS | ADC_CR2_EXTEN_0;

	/*6. Set external event select for regular group */
	ADC1->CR2 |= ADC_CR2_EXTSEL_1 | ADC_CR2_EXTSEL_2;


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

	/*4.2. Set memory transfer size to half-word (16-bit) */
	DMA2_Stream0->CR |= DMA_SxCR_MSIZE_0;

	/*4.3. Set peripheral transfer size to half-word (16-bit) */
	DMA2_Stream0->CR |= DMA_SxCR_PSIZE_0;

	/*4.4. Enable memory increment */
	DMA2_Stream0->CR |= DMA_SxCR_MINC;

	/*4.5. Enable transfer complete interrupt */
	DMA2_Stream0->CR |= DMA_SxCR_TCIE;

	/*4.6. Set peripheral address*/
	DMA2_Stream0->PAR = (uint32_t)(&ADC1->DR);

	/*4.7. Set memory address */
	DMA2_Stream0->M0AR = (uint32_t)(&adc_raw_data);

	/*4.8. Set number of transfer */
	DMA2_Stream0->NDTR |= NUM_OF_SAMPLES;

	/*5. Enable DMA interrupt in NVIC (nested vector interrupt controller) */
	NVIC_EnableIRQ(DMA2_Stream0_IRQn);


	/***********Enable Modules***********/

	/***********Timer2 Configuration***********/

	/* Configure timer period to 100Hz
	   i.e, ADC is going to be sampling at a 100Hz rate, every 10ms */

	/*1. Enable clock access to TIM2 */
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

	/*2. Set timer pre-scale value*/
	TIM2->PSC = 16000 - 1; // 16 000 000 / 16 000 = 1 kHz

	/*3. Set timer auto-reload value*/
	TIM2->ARR = 10 - 1; // 1000 / 10 = 100 Hz

	/*4. Configure master mode selection bits */
	TIM2->CR2 |= TIM_CR2_MMS_1;

	/*5. launch ADC */
	ADC1->CR2 |= ADC_CR2_ADON;

	/*5. Enable DMA stream. */
	DMA2_Stream0->CR |= DMA_SxCR_EN;

	/*6. Enable TIM2 */
	TIM2->CR1 |= TIM_CR1_CEN;

}

void adc_dma_init(void){

	/***********GPIO configuration***********/
	/*1. Enable clock access to ADC GPIO pin's port */
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

	/*2. Set PA0 and PA1 as analog mode */
	GPIOA->MODER |= GPIO_MODER_MODE0 | GPIO_MODER_MODE1;

	/***********ADC configuration***********/
	/*1. Enable clock access to ADC */
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;

	/*2. Set sequence length */
	ADC1->SQR1 |= ADC_SQR1_L_0;

	/*3. Set sequence */
	ADC1->SQR3 = (0U << 0) | (1U << 5);

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

