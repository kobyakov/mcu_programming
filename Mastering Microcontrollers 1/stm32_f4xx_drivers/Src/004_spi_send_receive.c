#include "stm32f411xx.h"
#include "stm32f411discovery.h"
#include "string.h"

void L3GD20_GPIOInit(void);
void L3GD20_SPI_Init(void);

void L3GD20WriteData(uint8_t address, uint8_t data);
uint8_t L3GD20ReadData(uint8_t address);

int main()
{
	L3GD20_GPIOInit();
	L3GD20_SPI_Init();
	SPI_SSIConfig(L3GD20_SPI, ENABLE);

	SPI_PeripheralControl(L3GD20_SPI, ENABLE);

	L3GD20WriteData(0x20, 0x0F);
	uint8_t whoAmI = L3GD20ReadData(0x8F);
	uint8_t ctrlReg1 = L3GD20ReadData(0xA0);

	void(whoAmI);
	void(ctrlReg1);

	while(1)
	{
		whoAmI = 0;
	}
}

void L3GD20_GPIOInit(void)
{
	GPIO_Handle_t SPIPins;
	SPIPins.pGPIOx = L3GD20_SPI_GPIO_PORT;
	SPIPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	SPIPins.GPIO_PinConfig.GPIO_PinAltFunMode = 5;
	SPIPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	SPIPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	SPIPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	// SCLK
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = L3GD20_SPI_SCK_PIN;
	GPIO_Init(&SPIPins);
	// MOSI
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = L3GD20_SPI_MOSI_PIN;
	GPIO_Init(&SPIPins);
	// MISO
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = L3GD20_SPI_MISO_PIN;
	GPIO_Init(&SPIPins);

	// CS
	GPIO_Handle_t CSPin;
	CSPin.pGPIOx = L3GD20_CS_PORT;
	CSPin.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUTPUT;
	CSPin.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	CSPin.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	CSPin.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	CSPin.GPIO_PinConfig.GPIO_PinNumber = L3GD20_CS_PIN;
	GPIO_Init(&CSPin);
}

void L3GD20_SPI_Init(void)
{
	SPI_Handle_t SPIHandle;
	SPIHandle.pSPIx = L3GD20_SPI;
	SPIHandle.SPIConfig.SPI_BusConfig = SPI_BUS_CONFIG_FULL_DUPLEX;
	SPIHandle.SPIConfig.SPI_DeviceMode = SPI_DEVICE_MODE_MASTER;
	SPIHandle.SPIConfig.SPI_ScklSpeed = SPI_SCKL_SPEED_DIV8;
	SPIHandle.SPIConfig.SPI_DFF = SPI_DFF_8_BITS;
	SPIHandle.SPIConfig.SPI_CPOL = SPI_CPOL_HIGH;
	SPIHandle.SPIConfig.SPI_CPHA = SPI_CPHA_LOW;
	SPIHandle.SPIConfig.SPI_SSM = SPI_SSM_EN;
	SPI_Init(&SPIHandle);
}


static uint8_t SendByte(uint8_t byte)
{
	SPI_SendData(L3GD20_SPI, &byte, 1);
	uint8_t respByte;
	SPI_ReceiveData(L3GD20_SPI, &respByte, 1);
	return respByte;
}

void L3GD20WriteData(uint8_t address, uint8_t data)
{
	GPIO_WriteToOutputPin(L3GD20_CS_PORT, L3GD20_CS_PIN, GPIO_PIN_RESET);
	//SPI_SendData(L3GD20_SPI, &address, 1);
	//SPI_SendData(L3GD20_SPI, &data, 1);
	SendByte(address);
	SendByte(data);
	GPIO_WriteToOutputPin(L3GD20_CS_PORT, L3GD20_CS_PIN, GPIO_PIN_SET);

}

uint8_t L3GD20ReadData(uint8_t address)
{
	uint8_t response = 0xFF;
	GPIO_WriteToOutputPin(L3GD20_CS_PORT, L3GD20_CS_PIN, GPIO_PIN_RESET);
	//SPI_SendData(L3GD20_SPI, &address, 1);
	SendByte(address);
	//SPI_ReceiveData(L3GD20_SPI, &response, 1);
	response = SendByte(0x00);
	GPIO_WriteToOutputPin(L3GD20_CS_PORT, L3GD20_CS_PIN, GPIO_PIN_SET);
	return response;
}

