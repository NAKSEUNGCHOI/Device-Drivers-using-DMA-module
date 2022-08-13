/*
 * adc_dmc.h
 *
 *  Created on: Aug 6, 2022
 *      Author: chlsk
 */

#ifndef INC_ADC_DMA_H_
#define INC_ADC_DMA_H_

#include <stdint.h>
#include "stm32f4xx.h"
#define NUM_OF_CHANNELS 2
#define NUM_OF_SAMPLES				10

void adc_dma_init(void);
void adc_tim2_dma_init(void);

#endif /* INC_ADC_DMA_H_ */
