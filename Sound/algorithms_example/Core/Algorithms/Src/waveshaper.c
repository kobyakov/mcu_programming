#include <waveshaper.h>

int initWaveShaper(waveShaper_t* waveShaper, waveShaperParams_t* params)
{
	if (!waveShaper)
		return 1;
	if (!params)
		return 1;

	waveShaper->noiseThreshold = params->noiseThreshold;
	waveShaper->lowThreshold = params->lowThreshold;
	waveShaper->lowGain = params->lowGain;
	waveShaper->highThreshold = params->highThreshold;
	waveShaper->highGain = params->highGain;
	waveShaper->outGain = params->outGain;

	return 0;
}

float waveShaperAlgorithm(waveShaper_t* waveShaper, float input)
{
	float sample = input;
	if (fabs(input) < waveShaper->lowThreshold && fabs(input) > waveShaper->noiseThreshold)
		sample *= waveShaper->lowGain;
	else if (fabs(input) > waveShaper->highThreshold)
		sample *= waveShaper->highGain;
	return sample * waveShaper->outGain;
}

