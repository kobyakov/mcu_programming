#include "stm32f4xx_hal.h"

void HAL_MspInit(void)
{
	__HAL_RCC_SYSCFG_CLK_ENABLE();
	__HAL_RCC_PWR_CLK_ENABLE();
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_0);
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* hTimer9)
{
	// Enable the clock for the TIM9
	__HAL_RCC_TIM9_CLK_ENABLE();

	// Enable IRQ of TIM9
	HAL_NVIC_EnableIRQ(TIM1_BRK_TIM9_IRQn);

	// Configure the priority
	HAL_NVIC_SetPriority(TIM1_BRK_TIM9_IRQn, 15, 0);

}
