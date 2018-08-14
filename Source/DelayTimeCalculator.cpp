/*
  ==============================================================================

    DelayTimeCalculator.cpp
    Created: 21 Feb 2018 8:49:03am
    Author:  AUGUS

  ==============================================================================
*/

#define _USE_MATH_DEFINES
#include <cmath>
#include "DelayTimeCalculator.h"


DelayTimeCalculator::DelayTimeCalculator()
{
	wholeNormal = 0;
	wholeDotted = 0;
	wholeTriplet = 0;

	halfNormal = 0;
	halfDotted = 0;
	halfTriplet = 0;

	quarterNormal = 0;
	quarterDotted = 0;
	quarterTriplet = 0;

	eighthNormal = 0;
	eighthDotted = 0;
	eighthTriplet = 0;

	sixteenthNormal = 0;
	sixteenthDotted = 0;
	sixteenthTriplet = 0;

	thirtySecondNormal = 0;
	thirtySecondDotted = 0;
	thirtySecondTriplet = 0;

	sixtyFourthNormal = 0;
	sixtyFourthDotted = 0;
	sixtyFourthTriplet = 0;

	oneHundredAndTwentyEighthNormal = 0;
	oneHundredAndTwentyEighthDotted = 0;
	oneHundredAndTwentyEighthTriplet = 0;

}


void DelayTimeCalculator::calculateMilliseconds(double tempo)
{
	// quarterNormal value must be calculated first, since other DelayTimeCalculator depend on it
	quarterNormal = 60 / tempo;

	wholeNormal = quarterNormal * 4;
	wholeDotted = wholeNormal * 1.5;
	wholeTriplet = wholeNormal * (2 / 3.0f);

	halfNormal = quarterNormal * 2;
	halfDotted = halfNormal * 1.5;
	halfTriplet = halfNormal * (2 / 3.0f);

	// quarterNormal
	quarterDotted = quarterNormal * 1.5;
	quarterTriplet = quarterNormal * (2 / 3.0f);

	eighthNormal = quarterNormal / 2;
	eighthDotted = eighthNormal * 1.5;
	eighthTriplet = eighthNormal * (2 / 3.0f);

	sixteenthNormal = quarterNormal / 4;
	sixteenthDotted = sixteenthNormal * 1.5;
	sixteenthTriplet = sixteenthNormal * (2 / 3.0f);

	thirtySecondNormal = quarterNormal / 8;
	thirtySecondDotted = thirtySecondNormal * 1.5;
	thirtySecondTriplet = thirtySecondNormal * (2 / 3.0f);

	sixtyFourthNormal = quarterNormal / 16;
	sixtyFourthDotted = sixtyFourthNormal * 1.5;
	sixtyFourthTriplet = sixtyFourthNormal * (2 / 3.0f);

	oneHundredAndTwentyEighthNormal = quarterNormal / 32;
	oneHundredAndTwentyEighthDotted = oneHundredAndTwentyEighthNormal * 1.5;
	oneHundredAndTwentyEighthTriplet = oneHundredAndTwentyEighthNormal * (2 / 3.0f);
}

void DelayTimeCalculator::calculateHertz(double tempo)
{
	// quarterNormal value must be calculated first, since other DelayTimeCalculator depend on it
	quarterNormal = tempo / 60;

	octNormal = quarterNormal / 32;
	octDotted = octNormal * (2 / 3.0f);
	octTriplet = octNormal * 1.5;

	quadNormal = quarterNormal / 16;
	quadDotted = quadNormal * (2 / 3.0f);
	quadTriplet = quadNormal * 1.5;

	doubleNormal = quarterNormal / 8;
	doubleDotted = doubleNormal * (2 / 3.0f);
	doubleTriplet = doubleNormal * 1.5;

	wholeNormal = quarterNormal / 4;
	wholeDotted = wholeNormal * (2 / 3.0f);
	wholeTriplet = wholeNormal * 1.5;

	halfNormal = quarterNormal / 2;
	halfDotted = halfNormal * (2 / 3.0f);
	halfTriplet = halfNormal * 1.5;

	// quarterNormal
	quarterDotted = quarterNormal * (2 / 3.0f);
	quarterTriplet = quarterNormal * 1.5;

	eighthNormal = quarterNormal * 2;
	eighthDotted = eighthNormal * (2 / 3.0f);
	eighthTriplet = eighthNormal * 1.5;

	sixteenthNormal = quarterNormal * 4;
	sixteenthDotted = sixteenthNormal * (2 / 3.0f);
	sixteenthTriplet = sixteenthNormal * 1.5;

	thirtySecondNormal = quarterNormal * 8;
	thirtySecondDotted = thirtySecondNormal * (2 / 3.0f);
	thirtySecondTriplet = thirtySecondNormal * 1.5;

	sixtyFourthNormal = quarterNormal * 16;
	sixtyFourthDotted = sixtyFourthNormal * (2 / 3.0f);
	sixtyFourthTriplet = sixtyFourthNormal * 1.5;

	oneHundredAndTwentyEighthNormal = quarterNormal * 32;
	oneHundredAndTwentyEighthDotted = oneHundredAndTwentyEighthNormal * (2 / 3.0f);
	oneHundredAndTwentyEighthTriplet = oneHundredAndTwentyEighthNormal * 1.5;
}
double DelayTimeCalculator::getOctNormal() const
{
	return octNormal;
}

double DelayTimeCalculator::getOctDotted() const
{
	return octDotted;
}

double DelayTimeCalculator::getOctTriplet() const
{
	return octTriplet;
}

double DelayTimeCalculator::getQuadNormal() const
{
	return quadNormal;
}

double DelayTimeCalculator::getQuadDotted() const
{
	return quadDotted;
}

double DelayTimeCalculator::getQuadTriplet() const
{
	return quadTriplet;
}

double DelayTimeCalculator::getDoubleNormal() const
{
	return doubleNormal;
}

double DelayTimeCalculator::getDoubleDotted() const
{
	return doubleDotted;
}

double DelayTimeCalculator::getDoubleTriplet() const
{
	return doubleTriplet;
}

double DelayTimeCalculator::getWholeNormal() const
{
	return wholeNormal;
}

double DelayTimeCalculator::getWholeDotted() const
{
	return wholeDotted;
}

double DelayTimeCalculator::getWholeTriplet() const
{
	return wholeTriplet;
}

double DelayTimeCalculator::getHalfNormal() const
{
	return halfNormal;
}

double DelayTimeCalculator::getHalfDotted() const
{
	return halfDotted;
}

double DelayTimeCalculator::getHalfTriplet() const
{
	return halfTriplet;
}

double DelayTimeCalculator::getQuarterNormal() const
{
	return quarterNormal;
}

double DelayTimeCalculator::getQuarterDotted() const
{
	return quarterDotted;
}

double DelayTimeCalculator::getQuarterTriplet() const
{
	return quarterTriplet;
}

double DelayTimeCalculator::getEighthNormal() const
{
	return eighthNormal;
}

double DelayTimeCalculator::getEighthDotted() const
{
	return eighthDotted;
}

double DelayTimeCalculator::getEighthTriplet() const
{
	return eighthTriplet;
}

double DelayTimeCalculator::getSixteenthNormal() const
{
	return sixteenthNormal;
}

double DelayTimeCalculator::getSixteenthDotted() const
{
	return sixteenthDotted;
}

double DelayTimeCalculator::getSixteenthTriplet() const
{
	return sixteenthTriplet;
}

double DelayTimeCalculator::getThirtySecondNormal() const
{
	return thirtySecondNormal;
}

double DelayTimeCalculator::getThirtySecondDotted() const
{
	return thirtySecondDotted;
}

double DelayTimeCalculator::getThirtySecondTriplet() const
{
	return thirtySecondTriplet;
}

double DelayTimeCalculator::getSixtyFouthNormal() const
{
	return sixtyFourthNormal;
}

double DelayTimeCalculator::getSixtyFouthDotted() const
{
	return sixtyFourthDotted;
}

double DelayTimeCalculator::getSixtyFouthTriplet() const
{
	return sixtyFourthTriplet;
}

double DelayTimeCalculator::getOneHundredAndTwentyEighthNormal() const
{
	return oneHundredAndTwentyEighthNormal;
}

double DelayTimeCalculator::getOneHundredAndTwentyEighthDotted() const
{
	return oneHundredAndTwentyEighthDotted;
}

double DelayTimeCalculator::getOneHundredAndTwentyEighthTriplet() const
{
	return oneHundredAndTwentyEighthTriplet;
}