/*
 * stm32f411discovery.h
 *
 *  Created on: Jan 24, 2022
 *      Author: dkobyako
 */

#ifndef STM32F411DISCOVERY_H_
#define STM32F411DISCOVERY_H_

#include <stm32f411xx_gpio_driver.h>
#include <stm32f411xx_spi_driver.h>

/*
 * LEDs
 */
#define LED_GPIO_PORT		GPIOD
#define ORANGE_LED_PIN 		GPIO_PIN_NO_13 		// LD3
#define GREEN_LED_PIN		GPIO_PIN_NO_12 		// LD4
#define RED_LED_PIN			GPIO_PIN_NO_14 		// LD5
#define BLUE_LED_PIN		GPIO_PIN_NO_15 		// LD6

/*
 * Button
 */
#define USER_BUTTON_PORT	GPIOA
#define USER_BUTTON_PIN		GPIO_PIN_NO_0
#define USER_BUTTON_IRQ_NO	IRQ_NO_EXTI0

#define HIGH 1
#define BUTTON_PRESSED		HIGH

/*
 * L3GD20 MEMS
 */
#define L3GD20_SPI_GPIO_PORT	GPIOA
#define L3GD20_SPI_SCK_PIN 		GPIO_PIN_NO_5
#define L3GD20_SPI_MOSI_PIN 	GPIO_PIN_NO_7
#define L3GD20_SPI_MISO_PIN 	GPIO_PIN_NO_6

#define L3GD20_CS_PORT			GPIOE
#define L3GD20_CS_PIN			GPIO_PIN_NO_3

#define L3GD20_SPI SPI1


#endif /* STM32F411DISCOVERY_H_ */
