/*
  ==============================================================================

    DSP.cpp
    Created: 24 Apr 2018 11:17:05am
    Author:  AUGUS

  ==============================================================================
*/
#define _USE_MATH_DEFINES
#include <cmath>
#include "DSP.h"




DSP::DSP()
{



}

void DSP::applyWidenning(float *leftSample, float *rightSample, float width)
{

	float widthVal = width*0.5;
	float tmp = 1 / fmax(1 + width, 2);
	float coef_M = 1 * tmp;
	float coef_S = width * tmp;

	float mid = (*leftSample + *rightSample)*coef_M;
	float sides = (*rightSample - *leftSample)*coef_S;

	*leftSample = mid - sides;
	*rightSample = mid + sides;
	
}

void DSP::applyPanning(float *leftSample, float *rightSample, float panPosition_)
{
	*leftSample *= cosf(panPosition_* M_PI_2);
	*rightSample *= sinf(panPosition_ * M_PI_2);

}

void DSP::applyGain(float *leftSample, float *rightSample, float gainValue_)
{
	gainValue_ = pow(10, gainValue_ / 20);
	//gainValue_ = sqrt(pow(10, 0.1*gainValue_));
	float leftIn = *leftSample * gainValue_;
	float rightIn = *rightSample * gainValue_;

	*leftSample = leftIn;
	*rightSample = rightIn;
}


