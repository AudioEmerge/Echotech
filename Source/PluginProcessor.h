/*

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
 
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __PLUGINPROCESSOR_H_4693CB6E__
#define __PLUGINPROCESSOR_H_4693CB6E__

#include "../JuceLibraryCode/JuceHeader.h"
#include "maximilian.h"
#include "DSP.h"
#include "DelayTimeCalculator.h"

//==============================================================================

class DelayAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    DelayAudioProcessor();
    ~DelayAudioProcessor();
	
    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock);
    void releaseResources();
    void reset();
    void processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages);
    
    //==============================================================================
    AudioProcessorEditor* createEditor();
    bool hasEditor() const;

    //==============================================================================
    const String getName() const;

    int getNumParameters();

    float getParameter (int index);
    void setParameter (int index, float newValue);

    const String getParameterName (int index);
    const String getParameterText (int index);

    const String getInputChannelName (int channelIndex) const;
    const String getOutputChannelName (int channelIndex) const;
    bool isInputChannelStereoPair (int index) const;
    bool isOutputChannelStereoPair (int index) const;

    bool silenceInProducesSilenceOut() const;
    double getTailLengthSeconds() const;
    bool acceptsMidi() const;
    bool producesMidi() const;

    //==============================================================================
    int getNumPrograms();
    int getCurrentProgram();
    void setCurrentProgram (int index);
    const String getProgramName (int index);
    void changeProgramName (int index, const String& newName);

    //==============================================================================
    void getStateInformation (MemoryBlock& destData);
    void setStateInformation (const void* data, int sizeInBytes);

	float msPerNote;
	
    //==============================================================================
    // these are used to persist the UI's size - the values are stored along with the
    // filter's other parameters, and the UI component will update them when it gets
    // resized.
    int lastUIWidth_, lastUIHeight_;
    
	enum Parameters
	{
		kInputGainParam = 0,
		kOutputGainParam,
		kDelayLengthLeftParam,
		kDelayLengthRightParam,
		kNoteLengthLeftParam,
		kNoteLengthRightParam,
		kFeedbackLeftParam,
		kFeedbackRightParam,
		kDryMixParam,
		kWetMixParam,
		kPanPositionParam,
		kWidenningParam,
		kCutoffParam,
		kResonanceParam,
		kFilterTypeParam,
		kWaveTypeParam,
		kSyncBPMParam,
		kLFOEnableParam,
		kLFORateParam,
		kLFORateSyncedParam,
		kLFOSyncParam,
		kLFOPhaseParam,
		kModDryMixParam,
		kModWetMixParam,
		kModPanParam,
		kModFilterFreqParam,
		kModResonanceParam,
		kDryModAmountParam,
		kWetModAmountParam,
		kPanModAmountParam,
		kFreqModAmountParam,
		kResModAmountParam,
		kNumParameters,
		kLinkDelaysParam
    };
    
    // Adjustable parameters:
    float delayLengthLeft_, delayLengthRight_; // Length of delay line in seconds
    float dryMix_;      // Mix level of original signal (0-1)
    float wetMix_;      // Mix level of delayed signal (0-1)
    float feedbackLeft_, feedbackRight_;    // Feedback level (0-just less than 1)
	float panPosition_, inputGain_, outputGain_, width_; 
	float cutoffFreq_, resValue_;
	int filterType_, waveType_;
	float noteDivisionLeft_, noteDivisionRight_;
	bool syncBPM_, linkDelayTimes_;
	double currentBPM_, lastBPM_;
	float lfoEnable_, lfoWave_, lfoRate_, lfoRateSync_, lfoPhase_,
	dryModAmount_, wetModAmount_, dryMod_, wetMod_, panModAmount_, freqModAmount_, resModAmount_;
	float  modWetMix_, modDryMix_, modPan_, modFilterFreq_, modResonance_, syncLFO_;

	void updateFilter();
	void updateLFOParams(float *leftSample, float *rightSample);
	float scaling(float inputValue, float minRange, float maxRange);
	
private:
	dsp::ProcessorDuplicator<dsp::StateVariableFilter::Filter<float>, dsp::StateVariableFilter::Parameters<float>> stateVariableFilter;
   
    // Circular buffer variables for implementing delay
    AudioSampleBuffer delayBufferLeft_, delayBufferRight_;
    int delayBufferLengthLeft_, delayBufferLengthRight_;
    int delayReadPositionLeft_, delayReadPositionRight_, delayWritePositionLeft_, delayWritePositionRight_;

	//Variables for LFO, Filter, Playhead, Delay calcuator
	maxiOsc lfo1_;
	IIRFilter filter_;
	float lastSampleRate;

	AudioPlayHead* playHead;
	AudioPlayHead::CurrentPositionInfo currentPositionInfo;
	DSP dsp_;
 
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DelayAudioProcessor);
};

#endif  // __PLUGINPROCESSOR_H_4693CB6E__
