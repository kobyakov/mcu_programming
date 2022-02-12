/*
 * stm32_f411_spi_driver.h
 *
 *  Created on: Jan 25, 2022
 *      Author: dkobyako
 */

#ifndef STM32F411XX_SPI_DRIVER_H_
#define STM32F411XX_SPI_DRIVER_H_

#include <stm32f411xx.h>

typedef struct {
	uint8_t SPI_DeviceMode;
	uint8_t SPI_BusConfig;
	uint8_t SPI_ScklSpeed;
	uint8_t SPI_DFF;
	uint8_t SPI_CPOL;
	uint8_t SPI_CPHA;
	uint8_t SPI_SSM;
} SPI_Config_t;

typedef struct {
	SPI_RegDef_t *pSPIx;
	SPI_Config_t SPIConfig;
	uint8_t* pTxBuffer;
	uint8_t *pRxBuffer;
	uint32_t txLen;
	uint32_t rxLen;
	uint8_t txState;
	uint8_t rxState;
} SPI_Handle_t;

/*
 * SPI Device mode
 */
#define SPI_DEVICE_MODE_MASTER	1
#define SPI_DEVICE_MODE_SLAVE	0

/*
 * SPI Bus config
 */
#define SPI_BUS_CONFIG_FULL_DUPLEX		1
#define SPI_BUS_CONFIG_HALF_DUPLEX		2
#define SPI_BUS_CONFIG_SIMPLEX_RXONLY	3


/*
 * SPI Clock speed
 */
#define SPI_SCKL_SPEED_DIV2		0
#define SPI_SCKL_SPEED_DIV4		1
#define SPI_SCKL_SPEED_DIV8		2
#define SPI_SCKL_SPEED_DIV16	3
#define SPI_SCKL_SPEED_DIV32	4
#define SPI_SCKL_SPEED_DIV64	5
#define SPI_SCKL_SPEED_DIV128	6
#define SPI_SCKL_SPEED_DIV256	7

/*
 * SPI DFF
 */
#define SPI_DFF_8_BITS		0
#define SPI_DFF_16_BITS		1

/*
 * SPI CPOL
 */
#define SPI_CPOL_HIGH 1
#define SPI_CPOL_LOW 0

/*
 * SPI CPHA
 */
#define SPI_CPHA_HIGH		0
#define SPI_CPHA_LOW		1

/*
 * SPI SSM
 */
#define SPI_SSM_EN		1
#define SPI_SSM_DI		0

/*
 * SPI related status flags definitions
 */
#define SPI_TXE_FLAG    ( 1 << SPI_SR_TXE)
#define SPI_RXNE_FLAG   ( 1 << SPI_SR_RXNE)
#define SPI_BUSY_FLAG   ( 1 << SPI_SR_BSY)


/*
 * Possible SPI states
 */
#define SPI_READY		0
#define SPI_BUSY_IN_RX	1
#define SPI_BUSY_IN_TX	2

/*
 * Possible SPI app events
 */
#define SPI_EVENT_TX_CMPLT	1
#define SPI_EVENT_RX_CMPLT	2
#define SPI_EVENT_OVR_ERR	3
#define SPI_EVENT_CRC_ERR	4

/*
 * Driver API
 */

/*
 * Peripheral clock initialization
 */
void SPI_PeriphClockControl(SPI_RegDef_t *pSPIx, uint8_t enOrDis);

/*
 * Init and deinit
 */
void SPI_Init(SPI_Handle_t* pSPIHandle);
void SPI_Deinit(SPI_RegDef_t *pSPIx);

/*
 * Data sending and receiving
 */
void SPI_SendData(SPI_RegDef_t* pSPIx, uint8_t* pTxBuffer, uint32_t len);
void SPI_ReceiveData(SPI_RegDef_t* pSPIx, uint8_t* pRxBuffer, uint32_t len);

uint8_t SPI_SendDataIT(SPI_Handle_t* pSPIHandle, uint8_t* pTxBuffer, uint32_t len);
uint8_t SPI_ReceiveDataIT(SPI_Handle_t* pSPIHandle, uint8_t* pRxBuffer, uint32_t len);

/*
 * IRQ configuration and ISR handling
 */
void SPI_IRQInterruptConfig(uint8_t irqNumber, uint8_t enOrDis);
void SPI_IRQPriorityConfig(uint8_t irqNumber, uint32_t irqPriority);
void SPI_IRQHandling(SPI_Handle_t* pHandle);

/*
 *
 */
void SPI_PeripheralControl(SPI_RegDef_t* pSPIx, uint8_t enOrDi);
void SPI_SSIConfig(SPI_RegDef_t* pSPIx, uint8_t enOrDi);

/*
 *
 */
void SPI_Clear_OVRFlag(SPI_Handle_t* pSPIHandle);
void SPI_CloseTransmission(SPI_Handle_t* pSPIHandle);
void SPI_CloseReception(SPI_Handle_t* pSPIHandle);

/*
 * Application callback
 */
void SPI_ApplicationEventCallback(SPI_Handle_t* pSPIHandle, uint8_t appEvent);

#define SPI1  				((SPI_RegDef_t*)SPI1_BASEADDR)
#define SPI2  				((SPI_RegDef_t*)SPI2_BASEADDR)
#define SPI3  				((SPI_RegDef_t*)SPI3_BASEADDR)


#endif /* STM32F411XX_SPI_DRIVER_H_ */
