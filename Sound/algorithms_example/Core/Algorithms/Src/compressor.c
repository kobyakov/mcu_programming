#include <compressor.h>

int initCompressor(compressor_t* compressor, compressorParams_t* params) {
	if (!compressor)
		return 1;
	if (!params)
		return 1;

	compressor->attack = params->attack;
	compressor->release = params->release;
	compressor->hold = params->hold;
	compressor->threshold = params->threshold;
	compressor->ratio = params->ratio;

	compressor->state.timeout = 0;
	compressor->state.compOperation = COMP_NO_OPERATION,
	compressor->state.gainStepAttack = (1.0f - compressor->ratio) / compressor->attack;
	compressor->state.gainStepRelease = (1.0f - compressor->ratio) / compressor->release;
	compressor->state.gain = 1.0f;

	return 0;
}

float compressorAlgorithm(compressor_t* compressor, float input)
{
	if (fabs(input) > compressor->threshold) {
		if (compressor->state.gain >= compressor->ratio) {
			if (compressor->state.compOperation == COMP_NO_OPERATION) {
				compressor->state.compOperation = COMP_ATTACK;
				compressor->state.timeout = compressor->attack;
			}
			else if (compressor->state.compOperation == COMP_RELEASE) {
				compressor->state.compOperation = COMP_ATTACK;
				compressor->state.timeout = compressor->attack;
			}
		}
		if (compressor->state.compOperation == COMP_GAIN_REDUCTION) {
			compressor->state.timeout = compressor->hold;
		}
	}
	if (fabs(input) < compressor->threshold && compressor->state.gain <= 1.0f) {
		if (compressor->state.timeout == 0 && compressor->state.compOperation == COMP_GAIN_REDUCTION) {
			compressor->state.compOperation = COMP_RELEASE;
			compressor->state.timeout = compressor->release;
		}
	}
	switch(compressor->state.compOperation) {
		case COMP_ATTACK: {
			if (compressor->state.timeout >0 && compressor->state.gain > compressor->ratio) {
				compressor->state.gain -= compressor->state.gainStepAttack;
				compressor->state.timeout--;
			}
			else {
				compressor->state.compOperation = COMP_GAIN_REDUCTION;
				compressor->state.timeout = compressor->hold;
			}
		}
		break;
		case COMP_GAIN_REDUCTION: {
			if (compressor->state.timeout > 0) {
				compressor->state.timeout--;
			}
			else {
				compressor->state.compOperation = COMP_RELEASE;
				compressor->state.timeout = compressor->release;
			}
		}
		break;
		case COMP_RELEASE: {
			if (compressor->state.timeout > 0 && compressor->state.gain < 1.0f) {
				compressor->state.timeout--;
				compressor->state.gain+=compressor->state.gainStepRelease;
			}
			else {
				compressor->state.compOperation = COMP_NO_OPERATION;
			}
		}
		break;
		case COMP_NO_OPERATION: {
			if (compressor->state.gain < 1.0f) {
				compressor->state.gain = 1.0f;
			}
		}
		break;
	}
	float output = input * compressor->state.gain;
	return output;
}


