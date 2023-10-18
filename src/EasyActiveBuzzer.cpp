/*
Name:		EasyActiveBuzzer.cpp
Version:	1.0.3
Created:	9/29/2017 12:03:48 AM
Updated:	2/19/2019 12:19:00 AM
Author:		Evert Arias
Github:		https://github.com/evert-arias/EasyActiveBuzzer
			Copyright (c) 2019 Evert Arias
*/

#include "EasyActiveBuzzer.h"

/* Class constructor */
EasyActiveBuzzerClass::EasyActiveBuzzerClass(){}
/* Class destructor */
EasyActiveBuzzerClass::~EasyActiveBuzzerClass() {}
/* Start beeping continuously at a given frequency. */
void EasyActiveBuzzerClass::beep()
{
	beep(1, 0, 1, 0, 0, NULL);
}
/* Beep at a given frequency an specific number of beeps. */
void EasyActiveBuzzerClass::beep(unsigned int beeps)
{
	beep(mOnDuration, mOffDuration, 1, mPauseDuration, beeps, NULL);
}
/* Beep at a given frequency an specific number of beeps, with callback functionality. */
void EasyActiveBuzzerClass::beep(unsigned int beeps, void (*finishedCallbackFunction)())
{
	beep(mOnDuration, mOffDuration, 1, mPauseDuration, beeps, finishedCallbackFunction);
}
/* Beep sequence at a given frequency. */
void EasyActiveBuzzerClass::beep(unsigned int const onDuration, unsigned int const offDuration, byte const beeps, unsigned int const pauseDuration, unsigned int const sequences)
{
	mOnDuration = onDuration ? _max(MINIMUM_INTERVAL, onDuration) : 0;
	mOffDuration = offDuration ? _max(MINIMUM_INTERVAL, offDuration) : 0;
	mBeeps = beeps;
	mPauseDuration = pauseDuration ? _max(MINIMUM_INTERVAL, pauseDuration) : 0;
	mSequences = sequences;
	mFinishedCallbackFunction = NULL;
	mStartTime = _max(millis(), 1);
	mLastRunTime = 0;
	update();
}
/* Beep sequence at a given frequency, with callback functionality. */
void EasyActiveBuzzerClass::beep(unsigned int const onDuration, unsigned int const offDuration, byte const beeps, unsigned int const pauseDuration, unsigned int const sequences, void (*finishedCallbackFunction)())
{
	mOnDuration = onDuration ? _max(MINIMUM_INTERVAL, onDuration) : 0;
	mOffDuration = offDuration ? _max(MINIMUM_INTERVAL, offDuration) : 0;
	mBeeps = beeps;
	mPauseDuration = pauseDuration ? _max(MINIMUM_INTERVAL, pauseDuration) : 0;
	mSequences = sequences;
	mFinishedCallbackFunction = finishedCallbackFunction;
	mStartTime = _max(millis(), 1);
	mLastRunTime = 0;
	update();
}
/* Start beeping at a given frequency, for an specific duration. */
void EasyActiveBuzzerClass::singleBeep(unsigned int duration)
{
	beep(duration, mOffDuration, 1, mPauseDuration, 1, NULL);
}
/* Start beeping at a given frequency, for an specific duration, with callback functionality. */
void EasyActiveBuzzerClass::singleBeep(unsigned int duration, void (*finishedCallbackFunction)())
{
	beep(duration, mOffDuration, 1, mPauseDuration, 1, finishedCallbackFunction);
}
/* Stop beeping. */
void EasyActiveBuzzerClass::stopBeep()
{

	pinMode(mPin, OUTPUT);
	digitalWrite(mPin, LOW);
}
/* Set the pin where the buzzer is connected. */
void EasyActiveBuzzerClass::setPin(unsigned int pin)
{
	mPin = pin;
}
/* Set On duration. */
void EasyActiveBuzzerClass::setOnDuration(unsigned int duration)
{
	mOnDuration = duration;
}
/* Set Off duration. */
void EasyActiveBuzzerClass::setOffDuration(unsigned int duration)
{
	mOffDuration = duration;
}
/* Set Pause duration. */
void EasyActiveBuzzerClass::setPauseDuration(unsigned int duration)
{
	mPauseDuration = duration;
}
/* Update function that keeps the library running. */
void EasyActiveBuzzerClass::update()
{
	unsigned long currentTime = millis();
	if (currentTime - mLastRunTime < MINIMUM_INTERVAL)
	{
		return;
	}
	mLastRunTime = currentTime;
	if (!mStartTime || !mPin)
	{
		return;
	}

	unsigned long elapsedTime = currentTime - mStartTime;
	unsigned int blinkDuration = mOnDuration + mOffDuration;
	unsigned int sequenceDuration = blinkDuration * mBeeps + mPauseDuration - mOffDuration;

	if (!sequenceDuration || (mSequences != 0 && elapsedTime / sequenceDuration >= mSequences))
	{
		mStartTime = 0;
		if (mFinishedCallbackFunction)
		{
			mFinishedCallbackFunction();
		}
		return;
	};

	unsigned int blinkingDuration = blinkDuration * mBeeps;
	unsigned int timeInSequence = elapsedTime % sequenceDuration;

	if (timeInSequence < blinkingDuration && timeInSequence % blinkDuration < mOnDuration)
	{
		digitalWrite(mPin, HIGH);
	}
	else
	{
		digitalWrite(mPin, LOW);
	};
}

EasyActiveBuzzerClass EasyActiveBuzzer;
