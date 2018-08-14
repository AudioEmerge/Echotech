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

#define _USE_MATH_DEFINES
#include <cmath>
#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DelayAudioProcessor::DelayAudioProcessor() : delayBufferLeft_(1, 1), delayBufferRight_(1, 1)
{
    // Set default values:
	inputGain_ = 0.0f;
	outputGain_ = 0.0f;
	delayLengthLeft_ = 0.5f;
	delayLengthRight_ = 0.5f;
	dryMix_ = 1.0f;
	wetMix_ = 0.5f;
	feedbackLeft_ = 0.25f;
	feedbackRight_ = 0.25f;
	delayBufferLengthLeft_ = 1.0f;
	delayBufferLengthRight_ = 1.0f;
	noteDivisionLeft_ = 1.0f;
	noteDivisionRight_ = 1.0f;
	syncBPM_ = false;
	linkDelayTimes_ = false;
	panPosition_ = 0.5f;
	cutoffFreq_ = 12500.0f;
	resValue_ = 1.0f;
	filterType_ = 1;
	waveType_ = 1;
	currentBPM_ = 120.0;
	lastBPM_ = 0.0;
	lfoEnable_ = 1.0f;
	lfoRate_ = 0.1;
	lfoRateSync_ = 0.0f;
	lfoPhase_ = 0.0f;
	syncLFO_ = 0.0f;
	modFilterFreq_ = 0.0f;
	modResonance_ = 0.0f;
	modPan_ = 0.0f;
	modWetMix_ = 0.0f;
	modDryMix_ = 0.0f;
	dryModAmount_ = 0.0f;
	wetModAmount_ = 0.0f;
	panModAmount_ = 0.0f;
	freqModAmount_ = 0.0f;
	resModAmount_ = 0.0f;
	dryMod_ = 0.0f;
	wetMod_ = 0.0f;
	width_ = 1.0f;
    
    // Start the circular buffer pointers at the beginning
	delayReadPositionLeft_ = 0;
	delayWritePositionLeft_ = 0;
	delayReadPositionRight_ = 0;
	delayWritePositionRight_ = 0;
    
    lastUIWidth_ = 1000;
    lastUIHeight_ = 300;	
}

DelayAudioProcessor::~DelayAudioProcessor()
{
}

//==============================================================================
const String DelayAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

int DelayAudioProcessor::getNumParameters()
{
    return kNumParameters;
}

float DelayAudioProcessor::getParameter (int index)
{
    // This method will be called by the host, probably on the audio thread, so
    // it's absolutely time-critical. Don't use critical sections or anything
    // UI-related, or anything at all that may block in any way!
    switch (index)
    {
		case kInputGainParam:				return inputGain_;
		case kOutputGainParam:				return outputGain_;
		case kDryMixParam:					return dryMix_;
		case kWetMixParam:					return wetMix_;
		case kFeedbackLeftParam:			return feedbackLeft_;
		case kFeedbackRightParam:			return feedbackRight_;
		case kDelayLengthLeftParam:			return delayLengthLeft_;
		case kDelayLengthRightParam:		return delayLengthRight_;
		case kNoteLengthLeftParam:			return noteDivisionLeft_;
		case kNoteLengthRightParam:			return noteDivisionRight_;
		case kPanPositionParam:				return panPosition_;
		case kWidenningParam:				return width_;
		case kCutoffParam:					return cutoffFreq_;
		case kResonanceParam:				return resValue_;
		case kFilterTypeParam:				return filterType_;
		case kWaveTypeParam:				return waveType_;
		case kSyncBPMParam:					return syncBPM_;
		case kLinkDelaysParam:				return linkDelayTimes_;
		case kLFOEnableParam:				return lfoEnable_;
		case kLFORateParam:					return lfoRate_;
		case kLFORateSyncedParam:			return lfoRateSync_;
		case kLFOPhaseParam:				return lfoPhase_;
		case kLFOSyncParam:					return syncLFO_;
		case kModDryMixParam:				return modDryMix_;
		case kModWetMixParam:				return modWetMix_;
		case kModPanParam:					return modPan_;
		case kModFilterFreqParam:			return modFilterFreq_;
		case kModResonanceParam:			return modResonance_;
		case kDryModAmountParam:			return dryModAmount_;
		case kWetModAmountParam:			return wetModAmount_;
		case kPanModAmountParam:			return panModAmount_;
		case kFreqModAmountParam:			return freqModAmount_;
		case kResModAmountParam:			return resModAmount_;


        default:               return 0.0f;
    }
}

void DelayAudioProcessor::setParameter (int index, float newValue)
{
    // This method will be called by the host, probably on the audio thread, so
    // it's absolutely time-critical. Don't use critical sections or anything
    // UI-related, or anything at all that may block in any way!
    switch (index)
    {
		case kInputGainParam:
			inputGain_ = newValue;
			break;
		case kOutputGainParam:
			outputGain_ = newValue;
			break;
        case kDryMixParam:
            dryMix_ = newValue;
            break;
        case kWetMixParam:
            wetMix_ = newValue;
            break;
        case kFeedbackLeftParam:
            feedbackLeft_ = newValue;
            break;
		case kFeedbackRightParam:
			feedbackRight_ = newValue;
			break;
        case kDelayLengthLeftParam:
            delayLengthLeft_ = newValue;
            delayReadPositionLeft_ = (int)(delayWritePositionLeft_ - (delayLengthLeft_ * getSampleRate())
														+ delayBufferLengthLeft_) % delayBufferLengthLeft_;
            break;
		case kDelayLengthRightParam:
			delayLengthRight_ = newValue;
			delayReadPositionRight_ = (int)(delayWritePositionRight_ - (delayLengthRight_ * getSampleRate())
		    											+ delayBufferLengthRight_) % delayBufferLengthRight_;
			break;
		case kPanPositionParam:
			panPosition_ = newValue;
			break;
		case kWidenningParam:
			width_ = newValue;
			break;
		case kWaveTypeParam:
			waveType_ = newValue;
			break;
		case kCutoffParam:
			cutoffFreq_ = newValue;
			break;
		case kResonanceParam:
			resValue_ = newValue;
			break;
		case kFilterTypeParam:
			filterType_ = newValue;
			break;
		case kNoteLengthLeftParam:
			noteDivisionLeft_ = newValue;
			break;
		case kNoteLengthRightParam:
			noteDivisionRight_ = newValue;
			break;
		case kSyncBPMParam:
			syncBPM_ = newValue;
			break;
		case kLinkDelaysParam:
			linkDelayTimes_ = newValue;
			break;
		case kLFOEnableParam:
			lfoEnable_ = newValue;
			break;
		case kLFORateParam:
			lfoRate_ = newValue;
			break;
		case kLFORateSyncedParam:
			lfoRateSync_ = newValue;
			break;
		case kLFOPhaseParam:
			lfoPhase_ = newValue;
			break;
		case kLFOSyncParam:
			syncLFO_ = newValue;
			break;
		case kModWetMixParam:
			modWetMix_ = newValue;
			break;
		case kModDryMixParam:
			modDryMix_ = newValue;
			break;
		case kModPanParam:
			modPan_ = newValue;
			break;
		case kModFilterFreqParam:
			modFilterFreq_ = newValue;
			break;
		case kModResonanceParam:
			modResonance_ = newValue;
			break;
		case kDryModAmountParam:
			dryModAmount_ = newValue;
			break;
		case kWetModAmountParam:
			wetModAmount_ = newValue;
			break;
		case kPanModAmountParam:
			panModAmount_ = newValue;
			break;
		case kFreqModAmountParam:
			freqModAmount_ = newValue;
			break;
		case kResModAmountParam:
			resModAmount_ = newValue;
			break;
        default:
            break;
    }
}

const String DelayAudioProcessor::getParameterName (int index)
{
    switch (index)
    {
		case kInputGainParam:				return "input gain";
		case kOutputGainParam:				return "output gain";
		case kDryMixParam:					return "dry mix";
		case kWetMixParam:					return "wet mix";
		case kFeedbackLeftParam:			return "feedback L";
		case kFeedbackRightParam:			return "feedback R";
		case kDelayLengthLeftParam:			return "delay left channel";
		case kDelayLengthRightParam:		return "delay right channel";
		case kPanPositionParam:				return "pan position";
		case kWidenningParam:				return "stereo spead";
		case kCutoffParam:					return "cutoff frequency";
		case kWaveTypeParam:				return "wave shape";
		case kResonanceParam:				return "resonance";
		case kFilterTypeParam:				return "filter type";
		case kNoteLengthLeftParam:			return "note division L";
		case kNoteLengthRightParam:			return "note division R";
		case kSyncBPMParam:					return "sync BPM";
		case kLinkDelaysParam:				return "link delay times";
		case kLFOEnableParam:				return "lfo on/off ";
		case kLFORateParam:					return "lfo rate ";
		case kLFORateSyncedParam:			return "lfo rate note division ";
		case kLFOPhaseParam:				return "lfo phase ";
		case kLFOSyncParam:					return "lfo rate sync ";
		case kModWetMixParam:				return "wet mix modulation On/Off";
		case kModDryMixParam:				return "dry mix modulation  On/Off";
		case kModPanParam:					return "pan modulation  On/Off";
		case kModFilterFreqParam:			return "cutoff frequency modulation  On/Off";
		case kModResonanceParam:			return "resonance modulation  On/Off";
		case kDryModAmountParam:			return "dry lfo amount";
		case kWetModAmountParam:			return "wet lfo amount";
		case kPanModAmountParam:			return "pan lfo amount";
		case kFreqModAmountParam:			return "cutoff freq lfo amount";
		case kResModAmountParam:			return "resonance lfo amount";

        default:               break;
    }
    
    return String::empty;
}

const String DelayAudioProcessor::getParameterText (int index)
{
    return String (getParameter (index), 2);
}

const String DelayAudioProcessor::getInputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

const String DelayAudioProcessor::getOutputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

bool DelayAudioProcessor::isInputChannelStereoPair (int index) const
{
    return true;
}

bool DelayAudioProcessor::isOutputChannelStereoPair (int index) const
{
    return true;
}

bool DelayAudioProcessor::silenceInProducesSilenceOut() const
{
#if JucePlugin_SilenceInProducesSilenceOut
    return true;
#else
    return false;
#endif
}
double DelayAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}
bool DelayAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool DelayAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

int DelayAudioProcessor::getNumPrograms()
{
    return 0;
}

int DelayAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DelayAudioProcessor::setCurrentProgram (int index)
{
}

const String DelayAudioProcessor::getProgramName (int index)
{
    return String::empty;
}

void DelayAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void DelayAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Allocate and zero the delay buffer (size will depend on current sample rate)
    // Sanity check the result so we don't end up with any zero-length calculations
    delayBufferLengthLeft_ = (int)(3.0*sampleRate); // 2 seconds delay @ fs
    if(delayBufferLengthLeft_ < 1 )
        delayBufferLengthLeft_ = 1;
    delayBufferLeft_.setSize(2, delayBufferLengthLeft_);;
    delayBufferLeft_.clear();

	delayBufferLengthRight_ = (int)(3.0*sampleRate); // 2 seconds delay @ fs
	if (delayBufferLengthRight_ < 1)
		delayBufferLengthRight_ = 1;
	delayBufferRight_.setSize(2, delayBufferLengthRight_);;
	delayBufferRight_.clear();

    
    // This method gives us the sample rate. Use this to figure out what the delay position
    // offset should be (since it is specified in seconds, and we need to convert it to a number
    // of samples)
    delayReadPositionLeft_ = (int)(delayWritePositionLeft_ - (delayLengthLeft_ * getSampleRate())
                               + delayBufferLengthLeft_) % delayBufferLengthLeft_;

	delayReadPositionRight_ = (int)(delayWritePositionRight_ - (delayLengthRight_ * getSampleRate())
		+ delayBufferLengthRight_) % delayBufferLengthRight_;

	
	//Filter
	lastSampleRate = sampleRate;

	dsp::ProcessSpec spec;
	spec.sampleRate = lastSampleRate;
	spec.maximumBlockSize = samplesPerBlock;
	spec.numChannels = getMainBusNumOutputChannels();

	stateVariableFilter.reset();
	updateFilter();
	stateVariableFilter.prepare(spec);

	lfo1_.phaseReset(lfoPhase_);

}

void DelayAudioProcessor::releaseResources()
{

	// The delay buffer will stay in memory until the effect is unloaded.
}

float DelayAudioProcessor::scaling(float inputValue, float minRange, float maxRange)
{
	float inVal = inputValue;
	float inMin = -1.0f;
	float inMax = 1.0f;
	float outMin = minRange;
	float outMax = maxRange;
	float expo = 0.00001f;
	
	//expr(exp((($f1 - $f2) / ($f3 - $f2) - 1)*$f6) - 1) / (exp(-$f6) - 1)*($f4 - $f5) + $f5
	float scaledValue = outMin + (outMax - outMin)*(inVal - inMin) / (inMax - inMin);

	return scaledValue;
}

void DelayAudioProcessor::updateFilter()
{
	float LFOFreq =0.0f;
	float LFORes= 0.0f;

	//Cutoff Frequency modulation
	if (modFilterFreq_ == true)
	{
		float normalisedFreq = lfoWave_ * (freqModAmount_ / 100.0f);
		LFOFreq = scaling(normalisedFreq, -5000.0f, 5000.0f);
	}
	else if (modFilterFreq_ == false)
	{
		LFOFreq = 0.0f;
	}

	float localCutoff = cutoffFreq_ + LFOFreq;

	//clip values if exceed min max values
	if (localCutoff < 20.0f) localCutoff = 20.0f;
	else if (localCutoff> 20000.0f) localCutoff = 20000.0f;

	//Resonance modulation
	if (modResonance_ == true)
	{
		float normalisedRes = lfoWave_ * (resModAmount_ / 100.0f);
		LFORes = scaling(normalisedRes, -5.0f, 5.0f);
	}
	else if (modResonance_ == false)
	{
		LFORes = 1.0f;
	}

	float localRes = resValue_ + LFORes;
	if (localRes < 1.0f) localRes = 1.0f;
	else if (localRes> 5.0f) localRes = 5.0f;

	//sets filter type, cutoff frequency and resonance value
	if (filterType_ == 1)
	{
		stateVariableFilter.state->type = dsp::StateVariableFilter::Parameters<float>::Type::lowPass;
		stateVariableFilter.state->setCutOffFrequency(lastSampleRate, localCutoff, localRes);
	}
	if (filterType_ == 2)
	{
		stateVariableFilter.state->type = dsp::StateVariableFilter::Parameters<float>::Type::bandPass;
		stateVariableFilter.state->setCutOffFrequency(lastSampleRate, localCutoff, localRes);
	}
	if (filterType_ == 3)
	{
		stateVariableFilter.state->type = dsp::StateVariableFilter::Parameters<float>::Type::highPass;
		stateVariableFilter.state->setCutOffFrequency(lastSampleRate, localCutoff, localRes);
	}
}

void DelayAudioProcessor::updateLFOParams(float *leftSample, float *rightSample)
{
	//Dry mix calculate modulation
	if (modDryMix_ == true)
	{
		float normalisedDry = (lfoWave_ * (dryModAmount_ / 100.0f));
		float LFODry = scaling(normalisedDry, -0.5f, 0.5f);
		dryMod_ = LFODry;
	}else if (modDryMix_ == false) dryMod_ = 0.0f;

	//Wet mix calculate modulation
	if (modWetMix_ == true)
	{
		float normalisedWet = (lfoWave_ * (wetModAmount_ / 100.0f));
		float LFOWet = scaling(normalisedWet, -0.5f, 0.5f);
		wetMod_ = LFOWet;
	}else if (modWetMix_ == false) wetMod_ = 0.0f;


	//Pan position apply modulation
	if (modPan_ == true)
	{
		float modPanPos;
		float normalisedPan = (lfoWave_ * (panModAmount_ / 100.0f));
		float LFOPan = scaling(normalisedPan, -0.5f, 0.5f);
		modPanPos = panPosition_ + LFOPan;

		if (modPanPos < 0.0f) modPanPos = 0.0f;
		else if (modPanPos > 1.0f) modPanPos = 1.0f;

		*leftSample *= cosf(modPanPos* M_PI_2);
		*rightSample *= sinf(modPanPos * M_PI_2);
	}
}

void DelayAudioProcessor::reset()
{
    // Use this method as the place to clear any delay lines, buffers, etc, as it
    // means there's been a break in the audio's continuity.
	delayBufferLeft_.clear();
	delayBufferRight_.clear();
}

void DelayAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    // Helpful information about this block of samples:
    const int numInputChannels = getNumInputChannels();     // How many input channels for our effect?
    const int numOutputChannels = getNumOutputChannels();   // How many output channels for our effect?
    const int numSamples = buffer.getNumSamples();          // How many samples in the buffer for this block?
    
    int channel, dprl, dpwl, dprr, dpwr; // dpr = delay read pointer; dpw = delay write pointer
    
    // Go through each channel of audio that's passed in. In this example we apply identical
    // effects to each channel, regardless of how many input channels there are. For some effects, like
    // a stereo chorus or panner, you might do something different for each channel.
    
    for (channel = 0; channel < numInputChannels; ++channel)
    {
        // channelData is an array of length numSamples which contains the audio for one channel
       // float* channelData = buffer.getWritePointer(channel);

		float* leftChannel = buffer.getWritePointer(0);
		float* rightChannel = buffer.getWritePointer(1);
        
        // delayData is the circular buffer for implementing delay on this channel
        float* delayDataLeft = delayBufferLeft_.getWritePointer (0);
		float* delayDataRight = delayBufferRight_.getWritePointer(1);
        
        // Make a temporary copy of any state variables declared in PluginProcessor.h which need to be
        // maintained between calls to processBlock(). Each channel needs to be processed identically
        // which means that the activity of processing one channel can't affect the state variable for
        // the next channel.
        
        dprl = delayReadPositionLeft_;
        dpwl = delayWritePositionLeft_;

		dprr = delayReadPositionRight_;
		dpwr = delayWritePositionRight_;
		for (int i = 0; i < numSamples; ++i)
		{
			//=================PlayHead-Info=======================================
			playHead = this->getPlayHead();
			playHead->getCurrentPosition(currentPositionInfo);
			currentBPM_ = currentPositionInfo.bpm;

			//=======================================================================
			//Apply input gain
			dsp_.applyGain(&leftChannel[i], &rightChannel[i], inputGain_);

			const float inLeft = leftChannel[i];
			const float inRight = rightChannel[i];
			float outLeft = 0.0;
			float outRight = 0.0;
			//modulate dry/wet parameters
			dryMod_ = dryMix_ + dryMod_;
			wetMod_ = wetMix_ + wetMod_;

			//making sure that the dry/wet mix parameters will not exceed (0-1)
			if (dryMod_ < 0.0) dryMod_ = 0.0f;  
			if (dryMod_ > 1.0) dryMod_ = 1.0f;
			if (wetMod_<0.0) wetMod_ = 0.0f;
			if (wetMod_>1.0) wetMod_ = 1.0;

			outLeft = (dryMod_ * inLeft + wetMod_ * delayDataLeft[dprl]);
			outRight = (dryMod_ * inRight + wetMod_ * delayDataRight[dprr]);

			// Store the current information in the delay buffer. delayData[dpr] is the delay sample we just read,
			// i.e. what came out of the buffer. delayData[dpw] is what we write to the buffer, i.e. what goes in
			delayDataLeft[dpwl] = inLeft + (delayDataLeft[dprl] * feedbackLeft_);
			delayDataRight[dpwr] = inRight + (delayDataRight[dprr] * feedbackRight_);

			if (++dprl >= delayBufferLengthLeft_)
				dprl = 0;
			if (++dpwl >= delayBufferLengthLeft_)
				dpwl = 0;

			if (++dprr >= delayBufferLengthRight_)
				dprr = 0;
			if (++dpwr >= delayBufferLengthRight_)
				dpwr = 0;

			// Store the output sample in the buffer, replacing the input
			leftChannel[i] = outLeft;
			rightChannel[i] = outRight;

			//Apply Panning
			dsp_.applyPanning(&leftChannel[i], &rightChannel[i], panPosition_);
			//Apply Widening
			dsp_.applyWidenning(&leftChannel[i], &rightChannel[i], width_);

			//===========LFO========================================================
			if (waveType_ == 1.0f)
			{
				lfoWave_ = lfo1_.sinewave(lfoRate_);
			}
			else if (waveType_ == 2.0f)
			{
				lfoWave_ = lfo1_.saw(lfoRate_);
			}
			else if (waveType_ == 3.0f)
			{
				lfoWave_ = lfo1_.triangle(lfoRate_);
			}
			else if (waveType_ == 4.0f)
			{
				lfoWave_ = lfo1_.square(lfoRate_);
				
			}
			
			updateLFOParams(&leftChannel[i], &rightChannel[i]);
			//Apply output gain
			dsp_.applyGain(&leftChannel[i], &rightChannel[i], outputGain_);
        }
    }
    

	//=================Filter=================
	const int totalNumInputChannels = getTotalNumInputChannels();
	const int totalNumOutputChannels = getTotalNumOutputChannels();

	for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
		buffer.clear(i, 0, buffer.getNumSamples());

	dsp::AudioBlock<float> block(buffer);
	updateFilter();
	stateVariableFilter.process(dsp::ProcessContextReplacing<float>(block));

	//=========================================================
	// Having made a local copy of the state variables for each channel, now transfer the result
    // back to the main state variable so they will be preserved for the next call of processBlock() 
    delayReadPositionLeft_ = dprl;
    delayWritePositionLeft_ = dpwl;
	delayReadPositionRight_ = dprr;
	delayWritePositionRight_ = dpwr;
    
    // In case we have more outputs than inputs, we'll clear any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    for (int i = numInputChannels; i < numOutputChannels; ++i)
    {
        buffer.clear (i, 0, buffer.getNumSamples());
    }
}



//==============================================================================
bool DelayAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* DelayAudioProcessor::createEditor()
{
    return new DelayAudioProcessorEditor (this);
}

//==============================================================================
void DelayAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    
    // Create an outer XML element..
    XmlElement xml("C4DMPLUGINSETTINGS");
    
    // add some attributes to it..
    xml.setAttribute("uiWidth", lastUIWidth_);
    xml.setAttribute("uiHeight", lastUIHeight_);
	xml.setAttribute("inputGain", inputGain_);
	xml.setAttribute("outputGain", outputGain_);
	xml.setAttribute("delayLengthLeft", delayLengthLeft_);
	xml.setAttribute("delayLengthRight", delayLengthRight_);
	xml.setAttribute("noteLength_L", noteDivisionLeft_);
	xml.setAttribute("noteLength_R", noteDivisionRight_);
	xml.setAttribute("feedback_L", feedbackLeft_);
	xml.setAttribute("feedback_R", feedbackRight_);
	xml.setAttribute("dryMix", dryMix_);
	xml.setAttribute("wetMix", wetMix_);
	xml.setAttribute("panPosition", panPosition_);
	xml.setAttribute("stereoSpread", width_);
	xml.setAttribute("cutoffFrequency", cutoffFreq_);
	xml.setAttribute("resonance", resValue_);
	xml.setAttribute("filterType", filterType_);
	xml.setAttribute("waveShape", waveType_);
	xml.setAttribute("syncBPM", syncBPM_);
	xml.setAttribute("linkDelayTime", linkDelayTimes_);
	xml.setAttribute("lfoRate", lfoRate_);
	xml.setAttribute("lfoRateSync", lfoRateSync_);
	xml.setAttribute("lfoPhase", lfoPhase_);
	xml.setAttribute("syncLFO", syncLFO_);
	xml.setAttribute("modWetMix", modWetMix_);
	xml.setAttribute("modDryMix", modDryMix_);
	xml.setAttribute("modPan", modPan_);
	xml.setAttribute("modFilterFreq", modFilterFreq_);
	xml.setAttribute("modResonance", modResonance_);
	xml.setAttribute("dryModAmount", dryModAmount_);
	xml.setAttribute("wetModAmount", wetModAmount_);
	xml.setAttribute("panModAmount", panModAmount_);
	xml.setAttribute("freqModAmount", freqModAmount_);
	xml.setAttribute("resModAmount", resModAmount_);
	
	
    // then use this helper function to stuff it into the binary blob and return it..
    copyXmlToBinary(xml, destData);
}

void DelayAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    
    // This getXmlFromBinary() helper function retrieves our XML from the binary blob..
    ScopedPointer<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    
    if(xmlState != 0)
    {
        // make sure that it's actually our type of XML object..
        if(xmlState->hasTagName("C4DMPLUGINSETTINGS"))
        {
            // ok, now pull out our parameters..
            lastUIWidth_      = xmlState->getIntAttribute("uiWidth", lastUIWidth_);
            lastUIHeight_     = xmlState->getIntAttribute("uiHeight", lastUIHeight_);
			inputGain_		  = (float)xmlState->getDoubleAttribute("inputGain", inputGain_);
			outputGain_		  = (float)xmlState->getDoubleAttribute("inputGain", outputGain_);
            delayLengthLeft_  = (float)xmlState->getDoubleAttribute("delayLengthLeft", delayLengthLeft_);
			delayLengthRight_ = (float)xmlState->getDoubleAttribute("delayLengthRight", delayLengthRight_);
			noteDivisionLeft_ = (float)xmlState->getDoubleAttribute("noteLength_L", noteDivisionLeft_);
			noteDivisionRight_= (float)xmlState->getDoubleAttribute("noteLength_R", noteDivisionRight_);
			feedbackLeft_     = (float)xmlState->getDoubleAttribute("feedback_L", feedbackLeft_);
			feedbackRight_    = (float)xmlState->getDoubleAttribute("feedback_R", feedbackRight_);
			dryMix_           = (float)xmlState->getDoubleAttribute("dryMix", dryMix_);
			wetMix_           = (float)xmlState->getDoubleAttribute("wetMix", wetMix_);
			panPosition_      = (float)xmlState->getDoubleAttribute("panPosition", panPosition_);
			cutoffFreq_       = (float)xmlState->getDoubleAttribute("cutoffFrequency", cutoffFreq_);
			width_			  = (float)xmlState->getDoubleAttribute("stereoSpread", width_);
			resValue_         = (float)xmlState->getDoubleAttribute("resonance", resValue_);
			filterType_       = xmlState->getIntAttribute("filterType", filterType_);
			waveType_         = xmlState->getIntAttribute("waveShape", waveType_);
			syncBPM_          = xmlState->getBoolAttribute("syncBPM", syncBPM_);
			linkDelayTimes_   = xmlState->getBoolAttribute("linkDelayTime", linkDelayTimes_);
			lfoRate_          = (float)xmlState->getDoubleAttribute("lfoRate", lfoRate_);
			lfoRateSync_      = (float)xmlState->getDoubleAttribute("lfoRateSync", lfoRateSync_);
			lfoPhase_		  = (float)xmlState->getDoubleAttribute("lfoPhase", lfoPhase_);
			syncLFO_		  = (float)xmlState->getDoubleAttribute("syncLFO", syncLFO_);
			modWetMix_		  = (float)xmlState->getDoubleAttribute("modWetMix", modWetMix_);
			modDryMix_		  = (float)xmlState->getDoubleAttribute("modDryMix", modDryMix_);
			modPan_			  = (float)xmlState->getDoubleAttribute("modPan", modPan_);
			modFilterFreq_	  = (float)xmlState->getDoubleAttribute("modFilterFreq", modFilterFreq_);
			modResonance_	  = (float)xmlState->getDoubleAttribute("modResonance", modResonance_);
			dryModAmount_	  = (float)xmlState->getDoubleAttribute("dryModAmount", dryModAmount_);
			wetModAmount_	  = (float)xmlState->getDoubleAttribute("wetModAmount", wetModAmount_);
			panModAmount_	  = (float)xmlState->getDoubleAttribute("panModAmount", panModAmount_);
			freqModAmount_	  = (float)xmlState->getDoubleAttribute("freqModAmount", freqModAmount_);
			resModAmount_	  = (float)xmlState->getDoubleAttribute("resModAmount", resModAmount_);
        }
    }
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DelayAudioProcessor();
}
