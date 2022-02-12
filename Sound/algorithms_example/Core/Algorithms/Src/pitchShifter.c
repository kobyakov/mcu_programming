#include <pitchShifter.h>

const float SHIFTING_COEFF = 0.5f;

#define BUF_SIZE 1000
float buf[BUF_SIZE];
const size_t bufSize = BUF_SIZE;

const size_t overlap = 100;

size_t writePtr = 0;
float readPtr = 0.0f;
float shift = SHIFTING_COEFF;

int initPitchShifter(pitchShifter_t* pitchShifter, float shift) {
	if (!pitchShifter)
		return 1;
	if (shift < 0)
		return 1;
	// Fc = 300hz, Fs = 48kHz, Q = 0.7
	filter_params_t params = {
		.a0 = 0.9726136465544695f,
		.a1 = -1.945227293108939f,
		.a2 = 0.9726136465544695f,
		.b1 = -1.9444771540706607f,
		.b2 = 0.9459774321472173f
	};
	initFilter(&(pitchShifter->filter), &params);
	pitchShifter->writePtr = 0;
	pitchShifter->readPtr = 0.0;
	pitchShifter->shift = shift;
	pitchShifter->buffer = buf;
	pitchShifter->bufferSize = BUF_SIZE;
	return 0;
}


float pitchShifterAlgorithm(pitchShifter_t* pitchShifter, float inputL, float inputR)
{
	float sum = inputL/2 + inputR/2;
	sum = filterAlgorithm(&pitchShifter->filter, sum);
	pitchShifter->buffer[pitchShifter->writePtr] = sum;

	int rdPtr_Int = roundf(pitchShifter->readPtr);
	int rdPtr_Int2 = 0;

	if (rdPtr_Int >= pitchShifter->bufferSize/2) {
		rdPtr_Int2 = rdPtr_Int - (pitchShifter->bufferSize/2);
	}
	else {
		rdPtr_Int2 = rdPtr_Int + (pitchShifter->bufferSize/2);
	}

	float rd0 = pitchShifter->buffer[rdPtr_Int];
	float rd1 = pitchShifter->buffer[rdPtr_Int2];
	float crossFade;

	if (overlap >= (pitchShifter->writePtr - rdPtr_Int) &&
			(pitchShifter->writePtr-rdPtr_Int) >=0 &&
			pitchShifter->shift != 1.0f) {
		int rel = pitchShifter->writePtr - rdPtr_Int;
		crossFade = ((float)rel)/((float)overlap);
	}
	else if (writePtr - rdPtr_Int == 0) {
		crossFade = 0.0f;
	}

	if (overlap >= (pitchShifter->writePtr - rdPtr_Int2) &&
			(pitchShifter->writePtr-rdPtr_Int2) >=0 &&
			pitchShifter->shift != 1.0f) {
		int rel = pitchShifter->writePtr - rdPtr_Int2;
		crossFade = 1.0f - ((float)rel)/((float)overlap);
	}
	else if (pitchShifter->writePtr - rdPtr_Int2 == 0) {
		crossFade = 1.0f;
	}

	sum = rd0*crossFade + rd1*(1-crossFade);

	pitchShifter->readPtr+=shift;
	pitchShifter->writePtr++;

	if (pitchShifter->writePtr == pitchShifter->bufferSize) pitchShifter->writePtr = 0;
	if (roundf(pitchShifter->readPtr) >= pitchShifter->bufferSize) pitchShifter->readPtr = 0.0f;

	return sum;

}

