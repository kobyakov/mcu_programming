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
#define AHB1_RCC_CFG_REG_OFFSET		0x08UL
#define AHB1_RCC_CFG_REG_ADDR		((AHB1_RCC_BASE_ADDR) + (AHB1_RCC_CFG_REG_OFFSET))
#define GPIOA_BASE_ADDR				0x40020000UL



int main(void)
{
	// 1. Configure RCC_CFGR MCO1 bit fields to select HSI as clock source

	uint32_t* pRccGfgReg = (uint32_t*)AHB1_RCC_CFG_REG_ADDR;
	*pRccGfgReg &= ~(0x3<<21); // clear 21 and 22 bit

	// Configure MCO1 prescaler
	*pRccCfgRef |= (1<<25);
	*pRccCfgRef |= (1<<26);

	// 2. Configura PA8 to AF0 mode to behave as MCO1 signal
	// enable the peripherial clock for GPIOA peripheral
	uint32_t *pRccAhb1Enr = (uint32_t*)((AHB1_RCC_BASE_ADDR) + 0x30);
	*pRccAhb1Enr |= (1<<0); // enable GPIOA peripheral clock

	// configure PA8 as an alternation function
	uint32_t *pGpioAModeReg = (uint32_t*)((GPIOA_BASE_ADDR) + 00);
	*pGpioAModeReg &= ~(0x3<<16); // clear
	*pGpioAModeReg |= (0x2<<16); // set

	// configure the alternation function to set the mode 0 for PA8
	uint32_t* pGpioAAltFunHighReg = (uint32_t*)((GPIOA_BASE_ADDR)+0x24);
	*pGpioAAltFunHighReg &= ~(0xF<<0);



    /* Loop forever */
	for(;;);
}