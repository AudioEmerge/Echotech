/*
  ==============================================================================

    DelayTimeCalculator.h
    Created: 21 Feb 2018 8:49:03am
    Author:  AUGUS

  ==============================================================================
*/

#ifndef MillisecondsHertzValueBase_h
#define MillisecondsHertzValueBase_h




class DelayTimeCalculator
{


private:

	double octNormal;
	double octDotted;
	double octTriplet;

	double quadNormal;
	double quadDotted;
	double quadTriplet;

	double doubleNormal;
	double doubleDotted;
	double doubleTriplet;

	double wholeNormal;
	double wholeDotted;
	double wholeTriplet;

	double halfNormal;
	double halfDotted;
	double halfTriplet;

	double quarterNormal;
	double quarterDotted;
	double quarterTriplet;

	double eighthNormal;
	double eighthDotted;
	double eighthTriplet;

	double sixteenthNormal;
	double sixteenthDotted;
	double sixteenthTriplet;

	double thirtySecondNormal;
	double thirtySecondDotted;
	double thirtySecondTriplet;

	double sixtyFourthNormal;
	double sixtyFourthDotted;
	double sixtyFourthTriplet;

	double oneHundredAndTwentyEighthNormal;
	double oneHundredAndTwentyEighthDotted;
	double oneHundredAndTwentyEighthTriplet;

public:
	DelayTimeCalculator();

	void calculateMilliseconds(double tempo);
	void calculateHertz(double tempo);

	double getOctNormal() const;
	double getOctDotted() const;
	double getOctTriplet() const;

	double getQuadNormal() const;
	double getQuadDotted() const;
	double getQuadTriplet() const;

	double getDoubleNormal() const;
	double getDoubleDotted() const;
	double getDoubleTriplet() const;

	double getWholeNormal() const;
	double getWholeDotted() const;
	double getWholeTriplet() const;

	double getHalfNormal() const;
	double getHalfDotted() const;
	double getHalfTriplet() const;

	double getQuarterNormal() const;
	double getQuarterDotted() const;
	double getQuarterTriplet() const;

	double getEighthNormal() const;
	double getEighthDotted() const;
	double getEighthTriplet() const;

	double getSixteenthNormal() const;
	double getSixteenthDotted() const;
	double getSixteenthTriplet() const;

	double getThirtySecondNormal() const;
	double getThirtySecondDotted() const;
	double getThirtySecondTriplet() const;

	double getSixtyFouthNormal() const;
	double getSixtyFouthDotted() const;
	double getSixtyFouthTriplet() const;

	double getOneHundredAndTwentyEighthNormal() const;
	double getOneHundredAndTwentyEighthDotted() const;
	double getOneHundredAndTwentyEighthTriplet() const;


};

#endif /* MillisecondsHertzValueBase */