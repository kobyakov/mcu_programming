#include <stm32f411xx_spi_driver.h>

static void spi_txe_interrupt_handle(SPI_Handle_t* pSPIHandle);
static void spi_rxne_unterrupt_handle(SPI_Handle_t* pSPIHandle);
static void spi_ovr_err_interrupt_handle(SPI_Handle_t* pSPIHandle);

void SPI_PeriphClockControl(SPI_RegDef_t *pSPIx, uint8_t enOrDis)
{
	if(enOrDis == ENABLE)
	{
		if (pSPIx == SPI1)
			SPI1_PCLK_EN();
		else if(pSPIx == SPI2)
			SPI2_PCLK_EN();
		else if(pSPIx == SPI3)
			SPI3_PCLK_EN();
	}
	else
	{
		/*
		 * @TODO
		 */
	}
}

void SPI_Init(SPI_Handle_t* pSPIHandle)
{
	uint32_t tempreg = 0;

	SPI_PeriphClockControl(pSPIHandle->pSPIx, ENABLE);

	// configure the device mode
	tempreg |= pSPIHandle->SPIConfig.SPI_DeviceMode << SPI_CR1_MSTR;

	// configure the bus config
	if (pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_FULL_DUPLEX)
	{
		// bidi mode cleared
		tempreg &= ~(1<<SPI_CR1_BIDIMODE);
	}
	else if (pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_HALF_DUPLEX)
	{
		// bidi mode set
		tempreg |= (1<<SPI_CR1_BIDIMODE);
	}
	else if (pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_SIMPLEX_RXONLY)
	{
		// bidi mode cleared
		tempreg &= ~(1<<SPI_CR1_BIDIMODE);
		// rxonly bit set
		tempreg |= (1<<SPI_CR1_BIDIOE);
	}

	// configure the serial clock speed
	tempreg |= pSPIHandle->SPIConfig.SPI_ScklSpeed << SPI_CR1_BR;

	// configure the dff
	tempreg |= pSPIHandle->SPIConfig.SPI_DFF << SPI_CR1_DFF;

	// configure the CPOL
	tempreg |= pSPIHandle->SPIConfig.SPI_CPOL << SPI_CR1_CPOL;

	// configure the CPHA
	tempreg |= pSPIHandle->SPIConfig.SPI_CPHA << SPI_CR1_CPHA;

	// configure the SSM
	tempreg |= pSPIHandle->SPIConfig.SPI_SSM << SPI_CR1_SSM;

	pSPIHandle->pSPIx->CR1 = tempreg;

}


void SPI_Deinit(SPI_RegDef_t *pSPIx)
{

}

uint8_t SPI_GetFlagStatus(SPI_RegDef_t* pSPIx, uint32_t flagName)
{
	if (pSPIx->SR & flagName)
	{
		return FLAG_SET;
	}
	return FLAG_RESET;
}

void SPI_SendData(SPI_RegDef_t* pSPIx, uint8_t* pTxBuffer, uint32_t len)
{
	while(len > 0)
	{
		// wait til TXE is set
		while(SPI_GetFlagStatus(pSPIx, SPI_TXE_FLAG) == FLAG_RESET);

		// check the dff
		if (pSPIx->CR1 & (1 << SPI_CR1_DFF))
		{
			// 16bit dff
			pSPIx->DR = *((uint16_t*)pTxBuffer);
			len -=2;
			(uint16_t*)pTxBuffer++;
		}
		else
		{
			// 8 bit dff
			pSPIx->DR = *pTxBuffer;
			len --;
			pTxBuffer++;
		}
	}
}

void SPI_ReceiveData(SPI_RegDef_t* pSPIx, uint8_t* pRxBuffer, uint32_t len)
{
	while(len > 0)
	{
		// wait til RXNE is set
		while(SPI_GetFlagStatus(pSPIx, SPI_RXNE_FLAG) == FLAG_RESET);

		// check the dff
		if (pSPIx->CR1 & (1 << SPI_CR1_DFF))
		{
			// 16bit dff
			*(uint16_t*)pRxBuffer = pSPIx->DR;
			len -=2;
			(uint16_t*)pRxBuffer++;
		}
		else
		{
			// 8 bit dff
			*pRxBuffer = pSPIx->DR;
			len --;
			pRxBuffer++;
		}
	}
}

uint8_t SPI_SendDataIT(SPI_Handle_t* pSPIHandle, uint8_t* pTxBuffer, uint32_t len)
{
	uint8_t state = pSPIHandle->txState;
	if (state != SPI_BUSY_IN_TX)
	{
		pSPIHandle->pTxBuffer = pTxBuffer;
		pSPIHandle->txLen = len;
		pSPIHandle->txState = SPI_BUSY_IN_TX;
		pSPIHandle->pSPIx->CR2 |= (1 << SPI_CR2_TXEIE);
	}

	return state;

}

uint8_t SPI_ReceiveDataIT(SPI_Handle_t* pSPIHandle, uint8_t* pRxBuffer, uint32_t len)
{
	uint8_t state = pSPIHandle->rxState;
	if (state != SPI_BUSY_IN_RX)
	{
		pSPIHandle->pRxBuffer = pRxBuffer;
		pSPIHandle->rxLen = len;
		pSPIHandle->rxState = SPI_BUSY_IN_RX;
		pSPIHandle->pSPIx->CR2 |= (1 << SPI_CR2_RXNEIE);
	}

	return state;
}

void SPI_IRQInterruptConfig(uint8_t irqNumber, uint8_t enOrDis)
{

}

void SPI_IRQPriorityConfig(uint8_t irqNumber, uint32_t irqPriority)
{

}


void SPI_IRQHandling(SPI_Handle_t* pHandle)
{
	uint8_t temp1, temp2;
	// check for txe
	temp1 = pHandle->pSPIx->SR & (1 << SPI_SR_TXE);
	temp2 = pHandle->pSPIx->CR2 & (1 << SPI_CR2_TXEIE);

	if (temp1 && temp2)
	{
		// handle TXE
		spi_txe_interrupt_handle(pHandle);
	}
	// check for rxne
	temp1 = pHandle->pSPIx->SR & (1 << SPI_SR_RXNE);
	temp2 = pHandle->pSPIx->CR2 & (1 << SPI_CR2_RXNEIE);

	if (temp1 && temp2)
	{
		spi_rxne_unterrupt_handle(pHandle);
	}

	temp1 = pHandle->pSPIx->SR & (1 << SPI_SR_OVR);
	temp2 = pHandle->pSPIx->CR2 & (1 << SPI_CR2_ERRIE);

	if (temp1 && temp2)
	{
		spi_ovr_err_interrupt_handle(pHandle);
	}

}

void SPI_PeripheralControl(SPI_RegDef_t* pSPIx, uint8_t enOrDi)
{
	if (enOrDi == ENABLE)
	{
		pSPIx->CR1 |= (1 << SPI_CR1_SPE);
	}
	else
	{
		pSPIx->CR1 &= ~(1 << SPI_CR1_SPE);
	}
}

void SPI_SSIConfig(SPI_RegDef_t* pSPIx, uint8_t enOrDi)
{
	if (enOrDi == ENABLE)
	{
		pSPIx->CR1 |= (1 << SPI_CR1_SSI);
	}
	else
	{
		pSPIx->CR1 &= ~(1 << SPI_CR1_SSI);
	}
}

void SPI_Clear_OVRFlag(SPI_Handle_t* pSPIHandle)
{
	uint8_t temp;
	temp = pSPIHandle->pSPIx->DR;
	temp = pSPIHandle->pSPIx->SR;
	(void)temp;
}

void SPI_CloseTransmission(SPI_Handle_t* pSPIHandle)
{
	pSPIHandle->pSPIx->CR2 &= ~(1 << SPI_CR2_TXEIE);
	pSPIHandle->pTxBuffer = NULL;
	pSPIHandle->txLen = 0;
	pSPIHandle->txState = SPI_READY;
}

void SPI_CloseReception(SPI_Handle_t* pSPIHandle)
{
	pSPIHandle->pSPIx->CR2 &= ~(1 << SPI_CR2_RXNEIE);
	pSPIHandle->pRxBuffer = NULL;
	pSPIHandle->rxLen = 0;
	pSPIHandle->rxState = SPI_READY;
}

__weak void SPI_ApplicationEventCallback(SPI_Handle_t* pSPIHandle, uint8_t appEvent)
{
}

/*
 * Some helper functions
 */
static void spi_txe_interrupt_handle(SPI_Handle_t* pSPIHandle)
{
	if (pSPIHandle->pSPIx->CR1 & (1 << SPI_CR1_DFF))
	{
		// 16bit dff
		pSPIHandle->pSPIx->DR = *((uint16_t*)pSPIHandle->pTxBuffer);
		pSPIHandle->txLen -=2;
		(uint16_t*)pSPIHandle->pTxBuffer++;
	}
	else
	{
		// 8 bit dff
		pSPIHandle->pSPIx->DR = *pSPIHandle->pTxBuffer;
		pSPIHandle->txLen --;
		pSPIHandle->pTxBuffer++;
	}
	if (!pSPIHandle->txLen)
	{
		SPI_CloseTransmission(pSPIHandle);
		SPI_ApplicationEventCallback(pSPIHandle, SPI_EVENT_TX_CMPLT);
	}
}

static void spi_rxne_unterrupt_handle(SPI_Handle_t* pSPIHandle)
{
	if (pSPIHandle->pSPIx->CR1 & (1 << SPI_CR1_DFF))
	{
		// 16bit dff
		*((uint16_t*)pSPIHandle->pRxBuffer) = pSPIHandle->pSPIx->DR;
		pSPIHandle->rxLen -=2;
		(uint16_t*)pSPIHandle->pRxBuffer++;
	}
	else
	{
		// 8 bit dff
		*(pSPIHandle->pRxBuffer) = pSPIHandle->pSPIx->DR;
		pSPIHandle->rxLen --;
		pSPIHandle->pRxBuffer++;
	}
	if (!pSPIHandle->rxLen)
	{
		SPI_CloseReception(pSPIHandle);
		SPI_ApplicationEventCallback(pSPIHandle, SPI_EVENT_RX_CMPLT);
	}
}

static void spi_ovr_err_interrupt_handle(SPI_Handle_t* pSPIHandle)
{
	if (pSPIHandle->txState != SPI_BUSY_IN_TX)
	{
		SPI_Clear_OVRFlag(pSPIHandle);
	}
	SPI_ApplicationEventCallback(pSPIHandle, SPI_EVENT_OVR_ERR);
}
