#ifndef ALGORITHMS_INC_FILTER_H_
#define ALGORITHMS_INC_FILTER_H_

#include <stdint.h>
#include <stddef.h>
#include <math.h>

typedef struct {
	float a0;
	float a1;
	float a2;
	float b1;
	float b2;
} filter_params_t;

typedef struct filter filter_t ;

int initFilter(filter_t* filter, filter_params_t* params);
float filterAlgorithm(filter_t* filter, float input);

typedef struct {
	float a0;
	float a1;
	float a2;
	float b1;
	float b2;
} filterCoeffs_t;

typedef struct {
	float in_z1;
	float in_z2;
	float out_z1;
	float out_z2;
} filterState_t;

typedef struct filter {
	filterCoeffs_t coeffs;
	filterState_t state;
} filter_t;

#endif /* ALGORITHMS_INC_FILTER_H_ */
