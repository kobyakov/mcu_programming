#include "stm32f411xx.h"
#include "stm32f411discovery.h"

void delay(void)
{
	for (uint32_t i = 0; i < 500000; i++);
}

int main(void)
{
	GPIO_Handle_t gpioLed;
	gpioLed.pGPIOx = LED_GPIO_PORT;
	gpioLed.GPIO_PinConfig.GPIO_PinNumber = ORANGE_LED_PIN;
	gpioLed.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUTPUT;
	gpioLed.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	gpioLed.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	gpioLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

	GPIO_PeriphClockControl(LED_GPIO_PORT, ENABLE);
	GPIO_Init(&gpioLed);

	while(1)
	{
		GPIO_ToggleOutputPin(LED_GPIO_PORT, ORANGE_LED_PIN);
		delay();
	}

	return 0;
}
