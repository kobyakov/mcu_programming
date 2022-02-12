#include "stm32f411xx.h"
#include "stm32f411discovery.h"

void delay(void)
{
	for (uint32_t i = 0; i < 500000/2; i++);
}

int main(void)
{
	GPIO_Handle_t gpioLed, gpioButton;
	gpioLed.pGPIOx = LED_GPIO_PORT;
	gpioLed.GPIO_PinConfig.GPIO_PinNumber = ORANGE_LED_PIN;
	gpioLed.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUTPUT;
	gpioLed.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	gpioLed.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	gpioLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

	GPIO_PeriphClockControl(LED_GPIO_PORT, ENABLE);
	GPIO_Init(&gpioLed);

	gpioButton.pGPIOx = USER_BUTTON_PORT;
	gpioButton.GPIO_PinConfig.GPIO_PinNumber = USER_BUTTON_PIN;
	gpioButton.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_INPUT;
	gpioButton.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	gpioButton.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

	GPIO_PeriphClockControl(USER_BUTTON_PORT, ENABLE);
	GPIO_Init(&gpioButton);

	while(1)
	{
		if (GPIO_ReadFromInputPin(USER_BUTTON_PORT, USER_BUTTON_PIN) == BUTTON_PRESSED)
		{
			delay();
			GPIO_WriteToOutputPin(LED_GPIO_PORT, ORANGE_LED_PIN, GPIO_PIN_SET);
		}
		else
		{
			GPIO_WriteToOutputPin(LED_GPIO_PORT, ORANGE_LED_PIN, GPIO_PIN_RESET);
		}
		delay();
	}

	return 0;
}


