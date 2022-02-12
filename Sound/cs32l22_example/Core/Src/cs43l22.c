/*
 * cs43l22.c
 *
 *  Created on: Jan 29, 2022
 *      Author: dkobyako
 */

#include "stm32f4xx_hal.h"
#include "cs43l22.h"

static uint8_t iData[2];
static I2C_HandleTypeDef i2cx;
extern I2S_HandleTypeDef hi2s3;

static void write_register(uint8_t reg, uint8_t *data)
{
	iData[0] = reg;
	iData[1] = data[0];
	HAL_I2C_Master_Transmit(&i2cx, DAC_I2C_ADDR, iData, 2, 100);
	//HAL_I2C_Master_Transmit(&i2cx, DAC_I2C_ADDR, data, size, 100);
}
static void read_register(uint8_t reg, uint8_t *data)
{
	iData[0] = reg;
	HAL_I2C_Master_Transmit(&i2cx, DAC_I2C_ADDR, iData, 1, 100);
	HAL_I2C_Master_Receive(&i2cx, DAC_I2C_ADDR, data, 1, 100);
}
/*
static void I2C1WriteByte(uint16_t address, uint8_t reg, uint8_t data)
{
	HAL_I2C_Mem_Write(&hi2s3, address, reg, I2C_MEMADD_SIZE_8BIT, &data, 1, HAL_MAX_DELAY);
}

static uint8_t I2C1ReadByte(uint16_t address, uint8_t reg)
{
	uint8_t response = 0;
	HAL_I2C_Mem_Read(&hi2c1, address, reg, I2C_MEMADD_SIZE_8BIT, &response, 1, HAL_MAX_DELAY);
	return response;
}

void CS43_InitDefault(I2C_HandleTypeDef i2c_handle)
{
	__HAL_UNLOCK(&hi2s3);
	__HAL_I2S_ENABLE(&hi2s3);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, GPIO_PIN_SET);
	i2cx = i2c_handle;

	iData[1] = 0x99;

	uint8_t data;

	data = 0x99;
	write_register(0x00, &data);

	data = 0x80;
	write_register(0x47, &data);


	read_register(0x32, &data);
	data |= 0x80;
	write_register(0x32, &data);

	read_register(0x32, &data);
	data &= (~0x80);
	write_register(0x32, &data);

	data = 0x00;
	write_register(0x00, &data);

	data = 0xd0;
	write_register(0x0d, &data);

	data = 0x81;
	write_register(0x05, &data);

	data = 0x07;
	write_register(0x06, &data);

}
void CS43_StartAudioPlayback()
{
	uint8_t data = 0x9e;
	write_register(0x02, &data);
}
*/

void CS43_Init(I2C_HandleTypeDef i2c_handle, CS43_MODE outputMode)
{
	__HAL_UNLOCK(&hi2s3);     // THIS IS EXTREMELY IMPORTANT FOR I2S3 TO WORK!!
	__HAL_I2S_ENABLE(&hi2s3); // THIS IS EXTREMELY IMPORTANT FOR I2S3 TO WORK!!
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, GPIO_PIN_SET);
	//(1): Get the I2C handle
	i2cx = i2c_handle;
	//(2): Power down
	iData[1] = 0x01;
	write_register(POWER_CONTROL1,iData);
	//(3): Enable Right and Left headphones
	iData[1] =  (2 << 6);  // PDN_HPB[0:1]  = 10 (HP-B always onCon)
	iData[1] |= (2 << 4);  // PDN_HPA[0:1]  = 10 (HP-A always on)
	iData[1] |= (3 << 2);  // PDN_SPKB[0:1] = 11 (Speaker B always off)
	iData[1] |= (3 << 0);  // PDN_SPKA[0:1] = 11 (Speaker A always off)
	write_register(POWER_CONTROL2,&iData[1]);
	//(4): Automatic clock detection
	iData[1] = (1 << 7);
	write_register(CLOCKING_CONTROL,&iData[1]);
	//(5): Interface control 1
	read_register(INTERFACE_CONTROL1, iData);
	iData[1] &= (1 << 5); // Clear all bits except bit 5 which is reserved
	iData[1] &= ~(1 << 7);  // Slave
	iData[1] &= ~(1 << 6);  // Clock polarity: Not inverted
	iData[1] &= ~(1 << 4);  // No DSP mode
	iData[1] &= ~(1 << 2);  // Left justified, up to 24 bit (default)
	iData[1] |= (1 << 2);

	iData[1] |=  (3 << 0);  // 16-bit audio word length for I2S interface
	write_register(INTERFACE_CONTROL1,&iData[1]);
	//(6): Passthrough A settings
	read_register(PASSTHROUGH_A, &iData[1]);
	iData[1] &= 0xF0;      // Bits [4-7] are reserved
	iData[1] |=  (1 << 0); // Use AIN1A as source for passthrough
	write_register(PASSTHROUGH_A,&iData[1]);
	//(7): Passthrough B settings
	read_register(PASSTHROUGH_B, &iData[1]);
	iData[1] &= 0xF0;      // Bits [4-7] are reserved
	iData[1] |=  (1 << 0); // Use AIN1B as source for passthrough
	write_register(PASSTHROUGH_B,&iData[1]);
	//(8): Miscellaneous register settings
	read_register(MISCELLANEOUS_CONTRLS, &iData[1]);
	if(outputMode == CS43_MODE_ANALOG)
	{
		iData[1] |=  (1 << 7);   // Enable passthrough for AIN-A
		iData[1] |=  (1 << 6);   // Enable passthrough for AIN-B
		iData[1] &= ~(1 << 5);   // Unmute passthrough on AIN-A
		iData[1] &= ~(1 << 4);   // Unmute passthrough on AIN-B
		iData[1] &= ~(1 << 3);   // Changed settings take affect immediately
	}
	else if(outputMode == CS43_MODE_I2S)
	{
		iData[1] = 0x02;
	}
	write_register(MISCELLANEOUS_CONTRLS,&iData[1]);
	//(9): Unmute headphone and speaker
	read_register(PLAYBACK_CONTROL, &iData[1]);
	iData[1] = 0x00;
	write_register(PLAYBACK_CONTROL,&iData[1]);
	//(10): Set volume to default (0dB)
	iData[1] = 0x00;
	write_register(PASSTHROUGH_VOLUME_A,&iData[1]);
	write_register(PASSTHROUGH_VOLUME_B,&iData[1]);
	write_register(PCM_VOLUME_A,&iData[1]);
	write_register(PCM_VOLUME_B,&iData[1]);
}

uint8_t CS43_GetId()
{
	uint8_t id;
	read_register(0x01, &id);
	return id;
}

// Function(2): Enable Right and Left headphones
void CS43_Enable_RightLeft(uint8_t side)
{
	switch (side)
	{
		case 0:
			iData[1] =  (3 << 6);  // PDN_HPB[0:1]  = 10 (HP-B always onCon)
			iData[1] |= (3 << 4);  // PDN_HPA[0:1]  = 10 (HP-A always on)
			break;
		case 1:
			iData[1] =  (2 << 6);  // PDN_HPB[0:1]  = 10 (HP-B always onCon)
			iData[1] |= (3 << 4);  // PDN_HPA[0:1]  = 10 (HP-A always on)
			break;
		case 2:
			iData[1] =  (3 << 6);  // PDN_HPB[0:1]  = 10 (HP-B always onCon)
			iData[1] |= (2 << 4);  // PDN_HPA[0:1]  = 10 (HP-A always on)
			break;
		case 3:
			iData[1] =  (2 << 6);  // PDN_HPB[0:1]  = 10 (HP-B always onCon)
			iData[1] |= (2 << 4);  // PDN_HPA[0:1]  = 10 (HP-A always on)
			break;
		default:
			break;
	}
	iData[1] |= (3 << 2);  // PDN_SPKB[0:1] = 11 (Speaker B always off)
	iData[1] |= (3 << 0);  // PDN_SPKA[0:1] = 11 (Speaker A always off)
	write_register(POWER_CONTROL2,&iData[1]);
}

// Function(3): Set Volume Level
void CS43_SetVolume(uint8_t volume)
{
	int8_t tempVol = volume - 50;
	tempVol = tempVol*(127/50);
	uint8_t myVolume =  (uint8_t )tempVol;
	iData[1] = myVolume;
	write_register(PASSTHROUGH_VOLUME_A,&iData[1]);
	write_register(PASSTHROUGH_VOLUME_B,&iData[1]);

	iData[1] = VOLUME_CONVERT_D(volume);

	/* Set the Master volume */
	write_register(CS43L22_REG_MASTER_A_VOL,&iData[1]);
	write_register(CS43L22_REG_MASTER_B_VOL,&iData[1]);
}

// Function(4): Start the Audio DAC
void CS43_Start(void)
{
	// Write 0x99 to register 0x00.
	iData[1] = 0x99;
	write_register(CONFIG_00,&iData[1]);
	// Write 0x80 to register 0x47.
	iData[1] = 0x80;
	write_register(CONFIG_47,&iData[1]);
	// Write '1'b to bit 7 in register 0x32.
	read_register(CONFIG_32, &iData[1]);
	iData[1] |= 0x80;
	write_register(CONFIG_32,&iData[1]);
	// Write '0'b to bit 7 in register 0x32.
	read_register(CONFIG_32, &iData[1]);
	iData[1] &= ~(0x80);
	write_register(CONFIG_32,&iData[1]);
	// Write 0x00 to register 0x00.
	iData[1] = 0x00;
	write_register(CONFIG_00,&iData[1]);
	//Set the "Power Ctl 1" register (0x02) to 0x9E
	iData[1] = 0x9E;
	write_register(POWER_CONTROL1,&iData[1]);
}

void CS43_Stop(void)
{
	iData[1] = 0x01;
	write_register(POWER_CONTROL1,&iData[1]);
}

