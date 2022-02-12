#ifndef ALGORITHMS_INC_REVERB_H_
#define ALGORITHMS_INC_REVERB_H_

#include <stdint.h>
#include <stddef.h>
#include <math.h>

typedef struct reverberator reverberator_t;
int initReverb(reverberator_t* r, float time, float wet);
float reverbAlgorithm(reverberator_t* r, float inputL, float inputR);

typedef struct  {
	float gain;
	float limit;
	float *buffer;
	size_t bufferSize;
	size_t readPointer;
} comboFilter_t;

typedef struct  {
	float gain;
	float limit;
	float *buffer;
	size_t bufferSize;
	size_t readPointer;
} allPassFilter_t;

typedef struct {
	comboFilter_t combo0;
	comboFilter_t combo1;
	comboFilter_t combo2;
	comboFilter_t combo3;
	allPassFilter_t allPass0;
	allPassFilter_t allPass1;
	allPassFilter_t allPass2;
} filters_t;

typedef struct reverberator {
	filters_t filters;
	float time;
	float wet;
} reverberator_t;

#endif /* ALGORITHMS_INC_REVERB_H_ */
