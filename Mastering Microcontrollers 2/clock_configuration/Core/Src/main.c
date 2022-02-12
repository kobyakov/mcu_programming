#include "main.h"
#include <string.h>

void SystemClockConfig(void);
void ErrorHandler(void);

int main(void)
{
	HAL_Init();
	SystemClockConfig();

	while(1);
}

void SystemClockConfig(void)
{
	RCC_OscInitTypeDef oscInit;
	RCC_ClkInitTypeDef clkInit;

	memset(&oscInit, 0, sizeof(oscInit));
	oscInit.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	oscInit.HSEState = RCC_HSE_ON;
	if (HAL_OK != HAL_RCC_OscConfig(&oscInit))
		ErrorHandler();

	clkInit.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | \
			RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	clkInit.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
	clkInit.AHBCLKDivider = RCC_SYSCLK_DIV2;
	clkInit.APB1CLKDivider = RCC_HCLK_DIV2;
	clkInit.APB2CLKDivider = RCC_HCLK_DIV2;

	if (HAL_OK != HAL_RCC_ClockConfig(&clkInit, FLASH_ACR_LATENCY_0WS))
		ErrorHandler();

	__HAL_RCC_HSI_DISABLE();

	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
}

void SystemClockConfig1(void)
{
	RCC_OscInitTypeDef oscInit;
	RCC_ClkInitTypeDef clkInit;

	memset(&oscInit, 0, sizeof(oscInit));
	oscInit.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	oscInit.HSIState = RCC_HSI_ON;
	oscInit.PLL.PLLState = RCC_PLL_ON;
	oscInit.PLL.PLLSource = RCC_PLLSOURCE_HSI;

	uint32_t FLatency = 0;

	// for 50 MHz
	{
		oscInit.PLL.PLLM = 16;
		oscInit.PLL.PLLN = 100;
		oscInit.PLL.PLLP = 2;
		oscInit.PLL.PLLQ = 2;
		//oscInit.PLL.PLLR = 2;

		clkInit.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | \
				RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
		clkInit.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
		clkInit.AHBCLKDivider = RCC_SYSCLK_DIV1;
		clkInit.APB1CLKDivider = RCC_HCLK_DIV2;
		clkInit.APB2CLKDivider = RCC_HCLK_DIV2;

		FLatency = FLASH_ACR_LATENCY_1WS;
	}

/*
 * Has to be configured to reach maximum frequency
	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
	__HAL_PWR_OVERDRIVE_ENABLE();
*/

	if (HAL_OK != HAL_RCC_OscConfig(&oscInit))
		ErrorHandler();

	if (HAL_OK != HAL_RCC_ClockConfig(&clkInit, FLatency))
		ErrorHandler();

	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

}

void SystemClockConfig2(void)
{
	RCC_OscInitTypeDef oscInit;
	RCC_ClkInitTypeDef clkInit;

	memset(&oscInit, 0, sizeof(oscInit));
	oscInit.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	oscInit.HSEState = RCC_HSE_ON;
	oscInit.PLL.PLLState = RCC_PLL_ON;
	oscInit.PLL.PLLSource = RCC_PLLSOURCE_HSE;

	uint32_t FLatency = 0;

	// for 50 MHz
	{
		oscInit.PLL.PLLM = 8;
		oscInit.PLL.PLLN = 100;
		oscInit.PLL.PLLP = 2;
		oscInit.PLL.PLLQ = 2;
		//oscInit.PLL.PLLR = 2;

		clkInit.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | \
				RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
		clkInit.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
		clkInit.AHBCLKDivider = RCC_SYSCLK_DIV1;
		clkInit.APB1CLKDivider = RCC_HCLK_DIV2;
		clkInit.APB2CLKDivider = RCC_HCLK_DIV2;

		FLatency = FLASH_ACR_LATENCY_1WS;
	}

	if (HAL_OK != HAL_RCC_OscConfig(&oscInit))
		ErrorHandler();

	if (HAL_OK != HAL_RCC_ClockConfig(&clkInit, FLatency))
		ErrorHandler();

	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

}

void ErrorHandler(void)
{

}
