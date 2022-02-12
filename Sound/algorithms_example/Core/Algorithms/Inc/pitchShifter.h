#ifndef ALGORITHMS_INC_PITCHSHIFTER_H_
#define ALGORITHMS_INC_PITCHSHIFTER_H_

#include <stdint.h>
#include <stddef.h>
#include <math.h>

#include <filter.h>

typedef struct pitchShifter pitchShifter_t;

int initPitchShifter(pitchShifter_t* p, float shift);

float pitchShifterAlgorithm(pitchShifter_t* p, float inputL, float inputR);

typedef struct pitchShifter {
	size_t writePtr;
	float readPtr;
	float shift;
	filter_t filter;
	float* buffer;
	size_t bufferSize;
} pitchShifter_t;

#endif /* ALGORITHMS_INC_PITCHSHIFTER_H_ */
