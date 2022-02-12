#ifndef AUDIOPLAY_H_
#define AUDIOPLAY_H_

#ifdef __cplusplus
 extern "C" {
#endif
//------------------------------------------------
#include "stm32f4xx_hal.h"
//#include "usbd_audio_if.h"
//------------------------------------------------

#define I2S3                            SPI3

/* Codec output DEVICE */
#define OUTPUT_DEVICE_SPEAKER         1
#define OUTPUT_DEVICE_HEADPHONE       2
#define OUTPUT_DEVICE_BOTH            3
#define OUTPUT_DEVICE_AUTO            4

/* Audio status definition */

#define AUDIO_OK                            ((uint8_t)0)
#define AUDIO_ERROR                         ((uint8_t)1)
#define AUDIO_TIMEOUT                       ((uint8_t)2)

 /* Codec POWER DOWN modes */
#define CODEC_PDWN_HW                 1
#define CODEC_PDWN_SW                 2

uint8_t AudioOut_Init(uint16_t OutputDevice, uint8_t Volume, uint32_t AudioFreq);

uint8_t AudioPlay_Play(uint16_t* pBuffer, uint32_t Size);
uint8_t AudioPlay_Stop(uint32_t Option);
void AudioPlay_ChangeBuffer(uint16_t *pData, uint16_t Size);
uint8_t AudioPlay_SetVolume(uint8_t Volume);
uint8_t AudioPlay_SetMute(uint32_t Cmd);
void AudioPlay_HalfTransfer_CallBack(void);
void AudioPlay_TransferComplete_CallBack(void);
//------------------------------------------------
#ifdef __cplusplus
}
#endif
#endif /* AUDIOPLAY_H_ */
