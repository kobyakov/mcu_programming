/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Auto-generated by STM32CubeIDE
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2022 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#include <stdint.h>

#define AHB1_RCC_BASE_ADDR			0x40023800UL
#define AHB1_RCC_ABP2_ENR_OFFSET	0x44UL
#define AHB1_RCC_ABP2_ENR_ADDR		((AHB1_RCC_BASE_ADDR)+(AHB1_RCC_ABP2_ENR_OFFSET))

#define ADC_BASE_ADDR				0x40012000UL
#define ADC_CR1_REG_OFFSET			0x04UL
#define ADC_CR1_REG_ADDR			((ADC_BASE_ADDR)+(ADC_CR1_REG_OFFSET))


#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

int main(void)
{
	uint32_t pRccApb2Enr  = (uint32_t*)AHB1_RCC_ABP2_ENR_ADDR;
	uint32_t *pAddrCr1Reg = (uint32_t*)ADC_CR1_REG_ADDR;

	// 1. Enable the peripheral clock for ADC1. It must be done before configuring the ADC.
	*pRccApb2Enr |= (1 << 8);

	// 2. Modift the ADC CR1 register
	*pAddrConfig |= (1 << 8);
    /* Loop forever */
	for(;;);
}