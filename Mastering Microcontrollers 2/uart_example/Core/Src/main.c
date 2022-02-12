#include "stm32f4xx_hal.h"
#include "main.h"

#include <string.h>

void SystemClockConfig(void);
void UART2_Init(void);
void ErrorHandler(void);

#define TRUE 1
#define FALSE 0

uint8_t convertToCapital(uint8_t byte);

USART_HandleTypeDef huart2;

char *helloString = "The application is running";
const size_t bufSize = 100;
size_t count = 0;
uint8_t dataBuffer[bufSize];
uint8_t receptionComplete = FALSE;

int main(void)
{
	HAL_Init();
	SystemClockConfig();
	UART2_Init();

	HAL_USART_Transmit(&huart2, (uint8_t*)helloString, strlen(helloString), HAL_MAX_DELAY);

	while(receptionComplete != TRUE)
		HAL_UART_Receive_IT(&huart2, recvData, 1);

	while(1);

// Polling-based
/*

	uint8_t rcvdData;
	const size_t bufSize = 100;
	uint8_t dataBuffer[bufSize];
	size_t count = 0;


	while(1)
	{
		count = 0;
		while(1)
		{
			HAL_USART_Receive(&huart2, &rcvdData, 1, HAL_MAX_DELAY);
			if (rcvdData == '\r' || count == bufSize-1)
			{
				break;
			}
			dataBuffer[count++] = convertToCapital(rcvdData);
		}
		dataBuffer[count++] = '\r';
		HAL_USART_Transmit(&huart2, (uint8_t*)dataBuffer, count, HAL_MAX_DELAY);
	}
*/

}

void SystemClockConfig(void)
{

}

void UART2_Init(void)
{
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 115200;
	huart2.Init.WordLength = USART_WORDLENGTH_8B;
	huart2.Init.StopBits = USART_STOPBITS_1;
	huart2.Init.Parity = USART_PARITY_NONE;
	huart2.Init.Mode = USART_MODE_TX_RX;
	if (HAL_USART_Init(&huart2) != HAL_OK)
	{
		ErrorHandler();
	}

}

void HAL_USART_RxCpltCallback(USART_HandleTypeDef *husart)
{
	if (recvData == '\r' || count == bufSize -1)
	{
		dataBuffer[count++] = '\r';
		HAL_USART_Transmit(&huart2, dataBuffer, count, HAL_MAX_DELAY);
		return;
	}
	dataBuffer[count++] = convertToCapital(recvData);
}

uint8_t convertToCapital(uint8_t data)
{
	if (data >= 'a' && data <= 'z')
	{
		data = data - ('a' - 'A');
	}
	return data;
}

void ErrorHandler(void)
{
	while(1);
}
