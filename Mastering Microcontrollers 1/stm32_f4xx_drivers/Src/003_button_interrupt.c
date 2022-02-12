#include "stm32f411xx.h"
#include "stm32f411discovery.h"
#include "string.h"

void delay(void)
{
	for (uint32_t i = 0; i < 500000/2; i++);
}

static const uint8_t selectedLED = GREEN_LED_PIN;

int main(void)
{
	GPIO_Handle_t gpioLed, gpioButton;
	memset(&gpioLed, 0, sizeof(gpioLed));
	memset(&gpioButton, 0, sizeof(gpioButton));

 	gpioLed.pGPIOx = LED_GPIO_PORT;
	gpioLed.GPIO_PinConfig.GPIO_PinNumber = selectedLED;
	gpioLed.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUTPUT;
	gpioLed.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	gpioLed.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	gpioLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

	GPIO_PeriphClockControl(LED_GPIO_PORT, ENABLE);
	GPIO_Init(&gpioLed);

	gpioButton.pGPIOx = USER_BUTTON_PORT;
	gpioButton.GPIO_PinConfig.GPIO_PinNumber = USER_BUTTON_PIN;
	gpioButton.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IT_FT;
	gpioButton.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	gpioButton.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

	GPIO_PeriphClockControl(USER_BUTTON_PORT, ENABLE);
	GPIO_Init(&gpioButton);

	// irq config
	GPIO_IRQPriorityConfig(USER_BUTTON_IRQ_NO, NVIC_IRQ_PRI15);
	GPIO_IRQInterruptConfig(USER_BUTTON_IRQ_NO, ENABLE);

	while(1);

	return 0;
}

void EXTI0_IRQHandler(void)
{
	delay();
	GPIO_IRQHandling(USER_BUTTON_PIN);
	GPIO_ToggleOutputPin(LED_GPIO_PORT, selectedLED);
}
