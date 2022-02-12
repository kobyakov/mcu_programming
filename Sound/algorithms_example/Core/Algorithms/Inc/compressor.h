#ifndef ALGORITHMS_INC_COMPRESSOR_H_
#define ALGORITHMS_INC_COMPRESSOR_H_

#include <stdint.h>
#include <stddef.h>
#include <math.h>

typedef struct {
	size_t attack;
	size_t release;
	size_t hold;
	float threshold;
	float ratio;
} compressorParams_t;

typedef struct compressor compressor_t;

int initCompressor(compressor_t* compressor, compressorParams_t* params);
float compressorAlgorithm(compressor_t* compressor, float input);

typedef enum {
	COMP_NO_OPERATION,
	COMP_ATTACK,
	COMP_GAIN_REDUCTION,
	COMP_RELEASE
} compressorOperation_t;

typedef struct {
	size_t timeout;
	compressorOperation_t compOperation;
	float gainStepAttack;
	float gainStepRelease;
	float gain;
} compressorState_t;

typedef struct compressor {
	size_t attack;
	size_t release;
	size_t hold;
	float threshold;
	float ratio;
	compressorState_t state;
} compressor_t;

#endif /* ALGORITHMS_INC_COMPRESSOR_H_ */
