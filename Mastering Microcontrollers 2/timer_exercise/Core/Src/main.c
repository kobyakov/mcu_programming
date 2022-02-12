#include "main.h"

void SystemClock_Config(void);

void GPIOInit(void);

void Timer9Init(void);
TIM_HandleTypeDef hTimer9;



int main(void)
{
  HAL_Init();
  SystemClock_Config();
  GPIOInit();
  Timer9Init();

  HAL_TIM_Base_Start_IT(&hTimer9);

  while(1);

/*
 * Polling
  HAL_TIM_Base_Start(&hTimer9);

  while (1)
  {
	  while(!TIM9->SR & TIM_SR_UIF);
	  TIM9->SR = 0;
	  HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
  }
*/
}

void SystemClock_Config(void)
{
}

void GPIOInit(void)
{
	__HAL_RCC_GPIOD_CLK_ENABLE();
	GPIO_InitTypeDef gpioLed;
	gpioLed.Mode = GPIO_MODE_OUTPUT_PP;
	gpioLed.Pin = GPIO_PIN_12;
	gpioLed.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOD, &gpioLed);
}

void Timer9Init(void)
{
	hTimer9.Instance = TIM9;
	hTimer9.Init.Prescaler = 25-1;
	hTimer9.Init.Period = 32000-1;
	HAL_TIM_Base_Init(&hTimer9);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* hTimer)
{
	HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
}

void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
}
