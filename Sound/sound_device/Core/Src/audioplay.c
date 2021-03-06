#include "audioplay.h"
//------------------------------------------------------
#define I2S_STANDARD                  I2S_STANDARD_PHILIPS

/* Position in the audio play buffer */

/* MUTE commands */
#define AUDIO_MUTE_ON                 1
#define AUDIO_MUTE_OFF                0

#define AUDIO_RESET_PIN                       GPIO_PIN_4
#define AUDIO_RESET_GPIO                      GPIOD
#define VOLUME_CONVERT(Volume)    (((Volume) > 100)? 100:((uint8_t)(((Volume) * 255) / 100)))

#define CODEC_STANDARD                0x04

static uint8_t Is_cs43l22_Stop = 1;

#define   CS43L22_REG_MISC_CTL            0x0E

#define AUDIO_I2C_ADDRESS                     0x94
#define CS43L22_CHIPID_ADDR    0x01
#define  CS43L22_ID            0xE0
#define  CS43L22_ID_MASK       0xF8

#define DMA_MAX_SZE                     0xFFFF
#define DMA_MAX(_X_)                (((_X_) <= DMA_MAX_SZE)? (_X_):DMA_MAX_SZE)
#define AUDIODATA_SIZE                  2   /* 16-bits audio data size */

const uint32_t I2SFreq[8] = {8000, 11025, 16000, 22050, 32000, 44100, 48000, 96000};
const uint32_t I2SPLLN[8] = {256, 429, 213, 429, 426, 271, 258, 344};
const uint32_t I2SPLLR[8] = {5, 4, 4, 4, 4, 6, 3, 1};
volatile uint8_t OutputDev = 0;

extern I2C_HandleTypeDef hi2c1;
extern I2S_HandleTypeDef hi2s3;
//------------------------------------------------
__STATIC_INLINE void DelayMicro(__IO uint32_t micros)
{
   micros *=(SystemCoreClock / 1000000) / 5;
   while (micros--);
}
//------------------------------------------------------
void Error(void)
{
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
}
//------------------------------------------------------
static void I2S3_Init(uint32_t AudioFreq)
{
	hi2s3.Instance=I2S3;
	/*Disable I2S Block*/
	__HAL_I2S_DISABLE(&hi2s3);
	hi2s3.Init.AudioFreq = AudioFreq;
	hi2s3.Init.Standard = I2S_STANDARD;
	HAL_I2S_DeInit(&hi2s3);
	if(HAL_I2S_Init(&hi2s3)!=HAL_OK)
	{
		Error();
	}
}
//------------------------------------------------------
uint8_t CODEC_IO_Write(uint8_t Addr, uint8_t Reg, uint8_t Value)
{
	HAL_StatusTypeDef status = HAL_OK;
	uint8_t result=0;
	status = HAL_I2C_Mem_Write(&hi2c1, Addr,(uint16_t)Reg,
						I2C_MEMADD_SIZE_8BIT,&Value,1,0x1000);
	if(status!=HAL_OK)
	{
		Error();
		return 1;
	}
	return result;
}
//------------------------------------------------------
uint32_t cs43l22_SetVolume(uint16_t DeviceAddr,
										uint8_t Volume)
{

	uint32_t counter=0;
	uint8_t convertedvol=VOLUME_CONVERT(Volume);
	if(Volume>0xE6)
	{
		/*Set the Master volume*/
		counter+=CODEC_IO_Write(DeviceAddr,0x20,convertedvol-0x07);
		counter+=CODEC_IO_Write(DeviceAddr,0x21,convertedvol-0x07);
	}
	else
	{
		/*Set the Master volume*/
		counter+=CODEC_IO_Write(DeviceAddr,0x20,convertedvol+0x19);
		counter+=CODEC_IO_Write(DeviceAddr,0x21,convertedvol+0x19);
	}
	return counter;
}
//------------------------------------------------------
uint32_t cs43l22_SetMute(uint16_t DeviceAddr, uint32_t Cmd)
{
	uint32_t counter=0;
	/*Set the Mute mode*/
	if(Cmd==AUDIO_MUTE_ON)
	{
		counter+=CODEC_IO_Write(DeviceAddr,0x04,0xFF);
	}
	else /*AUDIO_MUTE_OFF Disable the Mute*/
	{
		counter+=CODEC_IO_Write(DeviceAddr,0x04,OutputDev);
	}
	return counter;
}
//------------------------------------------------------
uint32_t cs43l22_Init(uint16_t DeviceAddr,uint16_t OutputDevice,
										uint8_t Volume, uint32_t AudioFreq)
{
	uint32_t counter=0;
	//reset on the codec
  HAL_GPIO_WritePin(AUDIO_RESET_GPIO, AUDIO_RESET_PIN, GPIO_PIN_RESET);
	DelayMicro(5000);
  HAL_GPIO_WritePin(AUDIO_RESET_GPIO, AUDIO_RESET_PIN, GPIO_PIN_SET);
	DelayMicro(5000);
	counter+=CODEC_IO_Write(DeviceAddr,0x02,0x01);
	/*Save Output device for mute ON/OFF procedure*/
	switch(OutputDevice)
	{
		case OUTPUT_DEVICE_SPEAKER:
			OutputDev=0xFA;
			break;
		case OUTPUT_DEVICE_HEADPHONE:
			OutputDev=0xAF;
			break;
		case OUTPUT_DEVICE_BOTH:
			OutputDev=0xAA;
			break;
		case OUTPUT_DEVICE_AUTO:
			OutputDev=0x05;
			break;
		default :
			OutputDev=0x05;
			break;
	}
	/*Clock configuration: Auto detection*/
	counter+=CODEC_IO_Write(DeviceAddr,0x04,OutputDev);
	/*Clock configuration: Auto detection Speed*/
	counter+=CODEC_IO_Write(DeviceAddr,0x05,0x81);

	counter+=CODEC_IO_Write(DeviceAddr,0x06,CODEC_STANDARD);
	/*Set the Master volume*/
	counter+=cs43l22_SetVolume(DeviceAddr,Volume);
	/*If the Speaker is enabled, set the Mono mode and Volume attenuation level*/
	if(OutputDevice!=OUTPUT_DEVICE_HEADPHONE)
	{
		/*Set the Speaker Mono mode*/
		counter+=CODEC_IO_Write(DeviceAddr,0x0F,0x06);
		/*Set the Speaker attenuation level*/
		counter+=CODEC_IO_Write(DeviceAddr,0x24,0x00);
		counter+=CODEC_IO_Write(DeviceAddr,0x25,0x00);
	}
	/*Disable the analog soft ramp*/
	counter+=CODEC_IO_Write(DeviceAddr,0x0A,0x00);
	/*Disable the digital soft ramp*/
	counter+=CODEC_IO_Write(DeviceAddr,0x0E,0x04);
	/*Disable the limiter attack level*/
	counter+=CODEC_IO_Write(DeviceAddr,0x27,0x00);
	/*Adjust Bass and Treble level*/
	counter+=CODEC_IO_Write(DeviceAddr,0x1F,0x0F);
	/*Adjust PCM volume level*/
	counter+=CODEC_IO_Write(DeviceAddr,0x1A,0x0A);
	counter+=CODEC_IO_Write(DeviceAddr,0x1B,0x0A);
	I2S3_Init(AudioFreq);
	return counter;
}
//------------------------------------------------------
uint32_t cs43l22_Stop(uint16_t DeviceAddr, uint32_t CodecPwdnMode)
{
	uint32_t counter=0;
	/*Mute the output first*/
	counter+=cs43l22_SetMute(DeviceAddr,AUDIO_MUTE_ON);
	/*Power down the DAC and the speaker (PMDAC and PMSPK bits)*/
		counter+=CODEC_IO_Write(DeviceAddr,0x02,0x9F);
	Is_cs43l22_Stop=1;
	return counter;
}
//------------------------------------------------------
uint32_t cs43l22_Play(uint16_t DeviceAddr,uint16_t *pBuffer, uint16_t Size)
{
	uint32_t counter=0;

	if(Is_cs43l22_Stop==1)
	{
		/*Enable the digital soft ramp*/
		counter+=CODEC_IO_Write(DeviceAddr,CS43L22_REG_MISC_CTL,0x06);
		/*Enable Output Device*/
		counter+= cs43l22_SetMute(DeviceAddr,AUDIO_MUTE_OFF);
		/*Power on the Codec*/
		counter+=CODEC_IO_Write(DeviceAddr,0x02,0x9E);
		Is_cs43l22_Stop=0;
	}
	return counter;
}
//------------------------------------------------------
uint32_t cs43l22_ReadID(uint16_t DeviceAddr)
{
	HAL_StatusTypeDef status = HAL_OK;
	uint8_t value=0;
	status = HAL_I2C_Mem_Read(&hi2c1, DeviceAddr,(uint16_t)CS43L22_CHIPID_ADDR,
						I2C_MEMADD_SIZE_8BIT,&value,1,0x1000);
	if(status==HAL_OK)
	{
		value=(value&CS43L22_ID_MASK);
		return ((uint32_t)value);
	}
	return 0;
}
//------------------------------------------------------
uint8_t AudioOut_Init(uint16_t OutputDevice, uint8_t Volume, uint32_t AudioFreq)
{
	uint8_t ret = AUDIO_ERROR;
	uint32_t deviceid=0x00;
	RCC_PeriphCLKInitTypeDef rccclkinit;
	uint8_t index=0,freqindex=0xFF;
	for(index=0;index<8;index++)
	{
		if(I2SFreq[index]==AudioFreq)
		{
			freqindex=index;
		}
	}
	//Enable PLLI2S clock
	HAL_RCCEx_GetPeriphCLKConfig(&rccclkinit);
	//PLLI2S_VCO Input=HSE_VALUE/PLL_M=1Mhz
	if((freqindex&0x7)==0)
	{
		/*I2S clock config
		PLLI2S_VCO=f(VCO clock) = f(PLLI2S clock input)/(PLLI2SN/PLLM)
		I2SCLK=f(PLLI2S clock input)=f(VCO clock)/PLLI2SR*/
		rccclkinit.PeriphClockSelection=RCC_PERIPHCLK_I2S;
		rccclkinit.PLLI2S.PLLI2SN=I2SPLLN[freqindex];
		rccclkinit.PLLI2S.PLLI2SR=I2SPLLR[freqindex];
		HAL_RCCEx_PeriphCLKConfig(&rccclkinit);
	}
	else
	{
		/*I2S clock config
		PLLI2S_VCO=f(VCO clock) = f(PLLI2S clock input)/(PLLI2SN/PLLM)
		I2SCLK=f(PLLI2S clock output)=f(VCO clock)/PLLI2SR*/
		rccclkinit.PeriphClockSelection=RCC_PERIPHCLK_I2S;
		rccclkinit.PLLI2S.PLLI2SN=258;
		rccclkinit.PLLI2S.PLLI2SR=3;
		HAL_RCCEx_PeriphCLKConfig(&rccclkinit);
	}
	//reset on the codec
  HAL_GPIO_WritePin(AUDIO_RESET_GPIO, AUDIO_RESET_PIN, GPIO_PIN_RESET);
	DelayMicro(5000);
  HAL_GPIO_WritePin(AUDIO_RESET_GPIO, AUDIO_RESET_PIN, GPIO_PIN_SET);
	DelayMicro(5000);
	deviceid=cs43l22_ReadID(AUDIO_I2C_ADDRESS);
	if((deviceid&CS43L22_ID_MASK)==CS43L22_ID)
	{
		ret=AUDIO_OK;
	}
	if(ret==AUDIO_OK)
	{
		cs43l22_Init(AUDIO_I2C_ADDRESS, OutputDevice, Volume, AudioFreq);
	}
	return ret;
}
//------------------------------------------------------
uint8_t AudioPlay_Stop(uint32_t Option)
{
	uint8_t ret=AUDIO_OK;
	/*Call Audio Codec Stop function*/
	if(cs43l22_Stop(AUDIO_I2C_ADDRESS,Option)!=0)
	{
		ret=AUDIO_ERROR;
	}
	if(ret==AUDIO_OK)
	{
		if(Option==CODEC_PDWN_HW)
		{
			/*Wait at least 100us*/
			DelayMicro(2000);
		}
		/*Stop DMA transfer of PCM samples towards the serial audio interface*/
		if(HAL_I2S_DMAStop(&hi2s3)!=HAL_OK)
		{
			ret=AUDIO_ERROR;
		}
	}
	return ret;
}
//------------------------------------------------------
uint8_t AudioPlay_Play(uint16_t *pBuffer, uint32_t Size)
{
	uint8_t ret=AUDIO_OK;
	if(cs43l22_Play(AUDIO_I2C_ADDRESS,pBuffer,Size)!=0)
	{
		Error();
		ret=AUDIO_ERROR;
	}
	if(ret==AUDIO_OK)
	{
		if(HAL_I2S_Transmit_DMA(&hi2s3,(uint16_t*)pBuffer,
									DMA_MAX(Size/AUDIODATA_SIZE))!=HAL_OK)
		{
			Error();
			ret=AUDIO_ERROR;
		}
	}
	return ret;
}
//------------------------------------------------------
uint8_t AudioPlay_SetVolume(uint8_t Volume)
{
	uint8_t ret=AUDIO_OK;
	/*Call the codec volume control function with converted volume value*/
	if(cs43l22_SetVolume(AUDIO_I2C_ADDRESS,Volume)!=0)
	{
		ret=AUDIO_ERROR;
	}
	/*Return AUDIO_OK when all operation are correctly done*/
	return ret;
}
//------------------------------------------------------
uint8_t AudioPlay_SetMute(uint32_t Cmd)
{
	uint8_t ret=AUDIO_OK;
	/*Call the codec Mute function*/
	if(cs43l22_SetMute(AUDIO_I2C_ADDRESS,Cmd)!=0)
	{
		ret=AUDIO_ERROR;
	}
	/*Return AUDIO_OK when all operation are correctly done*/
	return ret;
}
//------------------------------------------------------
void AudioPlay_ChangeBuffer(uint16_t *pData, uint16_t Size)
{
	HAL_I2S_Transmit_DMA(&hi2s3,(uint16_t*)pData,Size);
}
//------------------------------------------------------
void AudioPlay_HalfTransfer_CallBack(void)
{
	HalfTransfer_CallBack_FS();
}
//------------------------------------------------------
void AudioPlay_TransferComplete_CallBack(void)
{
	TransferComplete_CallBack_FS();
}
