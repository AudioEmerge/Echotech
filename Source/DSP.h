/*
  ==============================================================================

    DSP.h
    Created: 24 Apr 2018 11:17:05am
    Author:  AUGUS

  ==============================================================================
*/

#ifndef DSP_h
#define DSP_h


class DSP
{

public:
	DSP();

	void applyWidenning(float *leftSample, float *rightSample, float widdenning);
	void applyPanning(float *leftSample, float *rightSample, float panPosition);
	void applyGain(float *leftSample, float *rightSample, float gainValue);


private:




};

#endif /* DSP */