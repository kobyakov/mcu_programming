#ifndef ALGORITHMS_INC_WAVESHAPER_H_
#define ALGORITHMS_INC_WAVESHAPER_H_

#include <stdint.h>
#include <stddef.h>
#include <math.h>

typedef struct {
	float noiseThreshold;
	float lowThreshold;
	float lowGain;
	float highThreshold;
	float highGain;
	float outGain;
} waveShaperParams_t;

typedef struct waveShaper waveShaper_t;

int initWaveShaper(waveShaper_t* waveShaper, waveShaperParams_t* params);
float waveShaperAlgorithm(waveShaper_t* waveShaper, float input);

typedef struct waveShaper {
	float noiseThreshold;
	float lowThreshold;
	float lowGain;
	float highThreshold;
	float highGain;
	float outGain;
} waveShaper_t;


#endif /* ALGORITHMS_INC_WAVESHAPER_H_ */
