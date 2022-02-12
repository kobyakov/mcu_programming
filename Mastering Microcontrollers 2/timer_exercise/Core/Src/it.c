#include "stm32f4xx_hal.h"

extern TIM_HandleTypeDef hTimer9;

void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}

void TIM1_BRK_TIM9_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&hTimer9);
}
