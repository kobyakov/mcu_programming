#include <filter.h>

int initFilter(filter_t* filter, filter_params_t* params)
{
	if (!filter)
		return 1;
	if (!params)
		return 1;

	filter->coeffs.a0 = params->a0;
	filter->coeffs.a1 = params->a1;
	filter->coeffs.a2 = params->a2;

	filter->coeffs.b1 = params->b1;
	filter->coeffs.b2 = params->b2;

	filter->state.in_z1 = 0.0f;
	filter->state.in_z2 = 0.0f;
	filter->state.out_z1 = 0.0f;
	filter->state.out_z2 = 0.0f;

	return 0;
}

float filterAlgorithm(filter_t* filter, float input)
{
	float output =
	 	  filter->coeffs.a0 * input
		+ filter->coeffs.a1 * filter->state.in_z1
		+ filter->coeffs.a2 * filter->state.in_z2
		- filter->coeffs.b1 * filter->state.out_z1
		- filter->coeffs.b2 * filter->state.out_z2;
	filter->state.in_z2 = filter->state.in_z1;
	filter->state.in_z1 = input;
	filter->state.out_z2 = filter->state.out_z1;
	filter->state.out_z1 = output;
	return output;
}

