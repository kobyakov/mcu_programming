#include "stm32f4xx_hal.h"

void HAL_MspInit(void)
{
  /*
   * Here will do low level processor specific inits
   */

	__HAL_RCC_SYSCFG_CLK_ENABLE();
	__HAL_RCC_PWR_CLK_ENABLE();
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_0);

	// 1. Set up the priority grouping of the ARM Cortex Mx processor
	// HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

	// 2. Enable the required system exceptions of the ARM Cortex Mx processor
	// SCB->SHCSR |= 0x7 << 15; // usage fault, memory fault, bus fault are enabled

	// 3. Configure the priority for the system exceptions
	// HAL_NVIC_SetPriority(MemoryManagement_IRQn, 0, 0);
	// HAL_NVIC_SetPriority(BusFault_IRQn, 0, 0);
	// HAL_NVIC_SetPriority(UsageFault_IRQn, 0, 0);
}

void HAL_USART_MspInit(USART_HandleTypeDef *husart)
{
	GPIO_InitTypeDef gpioUart;

	// 1. Enable the clock for the USART and for the GPIOA
	__HAL_RCC_USART2_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	// 2. Do the pin muxing configuration
	gpioUart.Pin = GPIO_PIN_2;
	gpioUart.Mode = GPIO_MODE_AF_PP;
	gpioUart.Pull = GPIO_PULLUP;
	gpioUart.Speed = GPIO_SPEED_FREQ_LOW;
	gpioUart.Alternate = GPIO_AF7_USART2;
	HAL_GPIO_Init(GPIOA, &gpioUart);

	gpioUart.Pin = GPIO_PIN_3;
	HAL_GPIO_Init(GPIOA, &gpioUart);

	// 3. Enable the IRQ and set up the priority
	HAL_NVIC_EnableIRQ(USART2_IRQn);
	HAL_NVIC_SetPriority(USART2_IRQn, 15, 0);

}
