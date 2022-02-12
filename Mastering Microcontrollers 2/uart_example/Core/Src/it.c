#include "stm32f4xx_hal.h"

extern USART_HandleTypeDef huart2;

void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}

void USART2_IRQHandler(void)
{
	HAL_USART_IRQHandler(&huart2);

	HAL_USART_Receive_IT();


}


