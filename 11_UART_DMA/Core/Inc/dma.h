/*
 * dma.h
 *
 *  Created on: Aug 6, 2022
 *      Author: chlsk
 */

#ifndef INC_DMA_H_
#define INC_DMA_H_

#include "stm32f4xx.h"
#include <stdint.h>

void DMA2_transfer_start(uint32_t src_buff, uint32_t dest_buff, uint32_t len);
void DMA2_mem2mem_init(void);

#endif /* INC_DMA_H_ */
