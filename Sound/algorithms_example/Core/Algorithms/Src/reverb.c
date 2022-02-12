#include <reverb.h>

/*
 * 1 sample = 1/48kHz = ~20us
 * 36.04ms -> 1802
 * 31.13ms -> 1557
 * 40.44ms -> 2022
 * 44.92ms -> 2246
 * 5.0ms   -> 25
 * 1.68ms  -> 8
 * 0.48ms  -> 2
 */

// Shroeder delays
#define l_CB0 1802*2
#define l_CB1 1557*2
#define l_CB2 2022*2
#define l_CB3 2246*2
#define l_AP0 25*2
#define l_AP1 8*2
#define l_AP2 2*2

float cfbuf0[l_CB0];
float cfbuf1[l_CB1];
float cfbuf2[l_CB2];
float cfbuf3[l_CB3];
float apbuf0[l_AP0];
float apbuf1[l_AP1];
float apbuf2[l_AP2];

comboFilter_t combo0 = {
	.gain = 0.805,
	.limit = l_CB0,
	.buffer = cfbuf0,
	.bufferSize = l_CB0,
	.readPointer = 0
};

comboFilter_t combo1 = {
	.gain = 0.805,
	.limit = l_CB1,
	.buffer = cfbuf1,
	.bufferSize = l_CB1,
	.readPointer = 0
};

comboFilter_t combo2 = {
	.gain = 0.805,
	.limit = l_CB2,
	.buffer = cfbuf2,
	.bufferSize = l_CB2,
	.readPointer = 0
};

comboFilter_t combo3 = {
	.gain = 0.805,
	.limit = l_CB3,
	.buffer = cfbuf3,
	.bufferSize = l_CB3,
	.readPointer = 0
};

allPassFilter_t allPass0 = {
	.gain = 0.7,
	.limit = l_AP0,
	.buffer = apbuf0,
	.bufferSize = l_AP0,
	.readPointer = 0
};

allPassFilter_t allPass1 = {
	.gain = 0.7,
	.limit = l_AP1,
	.buffer = apbuf1,
	.bufferSize = l_AP1,
	.readPointer = 0
};

allPassFilter_t allPass2 = {
	.gain = 0.7,
	.limit = l_AP2,
	.buffer = apbuf2,
	.bufferSize = l_AP2,
	.readPointer = 0
};

static float comboProcess(comboFilter_t* filter, float input) {
	float readback = filter->buffer[filter->readPointer];
	float new = readback*filter->gain + input;
	filter->buffer[filter->readPointer] = new;
	filter->readPointer++;
	if (filter->readPointer==filter->limit ||
		filter->readPointer==filter->bufferSize) filter->readPointer = 0;
	return readback;
}

static float allPassProcess(allPassFilter_t* filter, float input) {
	float readback = filter->buffer[filter->readPointer];
	readback += (-filter->gain)*input;
	float new = readback*filter->gain + input;
	filter->buffer[filter->readPointer] = new;
	filter->readPointer++;
	if (filter->readPointer==filter->limit ||
		filter->readPointer==filter->bufferSize) filter->readPointer = 0;
	return readback;
}


int initReverb(reverberator_t* r, float time, float wet)
{
	if (time > 1.0)
		return 1;
	if (wet > 1.0)
		return 1;
	r->filters.combo0 = combo0;
	r->filters.combo1 = combo1;
	r->filters.combo2 = combo2;
	r->filters.combo3 = combo3;
	r->filters.allPass0 = allPass0;
	r->filters.allPass1 = allPass1;
	r->filters.allPass2 = allPass2;
	r->time = time;
	r->wet = wet;

	r->filters.combo0.limit = time * l_CB0;
	r->filters.combo1.limit = time * l_CB1;
	r->filters.combo2.limit = time * l_CB2;
	r->filters.combo3.limit = time * l_CB3;

	r->filters.allPass0.limit = time*l_AP0;
	r->filters.allPass1.limit = time*l_AP1;
	r->filters.allPass2.limit = time*l_AP2;

	return 0;
}
float reverbAlgorithm(reverberator_t* r, float inputL, float inputR) {
	float input = (inputL + inputR) / 2.0f;
	float newsample =
			 (  comboProcess(&(r->filters.combo0), input)
			  + comboProcess(&(r->filters.combo1), input)
			  + comboProcess(&(r->filters.combo2), input)
			  + comboProcess(&(r->filters.combo3), input)
			 ) / 4.0f;
	newsample = allPassProcess(&(r->filters.allPass0), newsample);
	newsample = allPassProcess(&(r->filters.allPass1), newsample);
	newsample = allPassProcess(&(r->filters.allPass2), newsample);
	float output = (1.0f-r->wet)*input + r->wet*newsample;
	return output;
}



