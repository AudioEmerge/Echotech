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

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DelayAudioProcessorEditor::DelayAudioProcessorEditor(DelayAudioProcessor* ownerFilter)
	: AudioProcessorEditor(ownerFilter),
	inputGainLabel_("", "Input Gain"),
	outputGainLabel_("", "Output Gain"),
	delayLengthLabel_("", "Delay Time"),
	noteLengthLabel_("", "Delay Time"),
	noteDivLeftLabel_("", ""),
	noteDivRightLabel_("", ""),
	feedbackLabel_("", "Feedback"),
	dryMixLabel_("", "Dry Mix Level"),
	wetMixLabel_("", "Wet Mix Level"),
	panLabel_("", "Pan Position"),
	widenningLabel_ ("", "Stereo Width"),
	cutoffLabel_("", "Cutoff Freq"),
	resonanceLabel_("", "Resonance"),
	filterTypeLabel_("", "Filter Type"),
	shapeLabel_("", "Wave Shape"),
	lfoRateLabel_("", "Rate (Hz)"),
	lfoRateSyncLabel_("", "Rate (Note)"),
	lfoRateSyncDisplayLabel_("", ""),
	lfoEnableLabel_("", "LFO Enable"),
	cutoffModLabel_("", "Cutoff Freq"),
	lfoPhaseLabel_("", "Phase (0-1)")

{
	lastBPMValue_ = 120;

	//GroupComponents
	addAndMakeVisible(gainGroup_);
	gainGroup_.setText("Gain");
	gainGroup_.setColour(GroupComponent::outlineColourId, Colours::white);

	addAndMakeVisible(delayGroup_);
	delayGroup_.setText("Delay");
	delayGroup_.setColour(GroupComponent::outlineColourId, Colours::white);

	addAndMakeVisible(panGroup_);
	panGroup_.setText("Pan");
	panGroup_.setColour(GroupComponent::outlineColourId, Colours::white);

	addAndMakeVisible(filterGroup_);
	filterGroup_.setText("Cutoff");
	filterGroup_.setColour(GroupComponent::outlineColourId, Colours::white);

	addAndMakeVisible(mixGroup_);
	mixGroup_.setText("Mix");
	mixGroup_.setColour(GroupComponent::outlineColourId, Colours::white);

	addAndMakeVisible(lfoGroup_);
	lfoGroup_.setText("LFO");
	lfoGroup_.setColour(GroupComponent::outlineColourId, Colours::white);


    // Set up the sliders
    addAndMakeVisible (&delayLengthLeftSlider_);
    delayLengthLeftSlider_.setSliderStyle (Slider::Rotary);
    delayLengthLeftSlider_.addListener (this);
    delayLengthLeftSlider_.setRange (0.0001, 3.0000, 0.001);
	delayLengthLeftSlider_.setTextBoxStyle(Slider::TextBoxBelow, false, 70, 16);
	delayLengthLeftSlider_.setTextValueSuffix("  sec");

	addAndMakeVisible(&delayLengthRightSlider_);
	delayLengthRightSlider_.setSliderStyle(Slider::Rotary);
	delayLengthRightSlider_.addListener(this);
	delayLengthRightSlider_.setRange(0.0001, 3.0000, 0.001);
	delayLengthRightSlider_.setTextBoxStyle(Slider::TextBoxBelow, false, 70, 16);
	delayLengthRightSlider_.setTextValueSuffix("  sec");

	addAndMakeVisible(&noteLengthLeftSlider_);
	noteLengthLeftSlider_.setSliderStyle(Slider::Rotary);
	noteLengthLeftSlider_.addListener(this);
	noteLengthLeftSlider_.setRange(4, 27, 1);
	noteLengthLeftSlider_.setTextBoxStyle(Slider::TextBoxBelow, false, 70, 16);
	noteLengthLeftSlider_.setColour(Slider::textBoxTextColourId, Colours::grey.withAlpha(0.0f));
	noteLengthLeftSlider_.setVisible(false);

	addAndMakeVisible(&noteLengthRightSlider_);
	noteLengthRightSlider_.setSliderStyle(Slider::Rotary);
	noteLengthRightSlider_.addListener(this);
	noteLengthRightSlider_.setRange(4, 27, 1);
	noteLengthRightSlider_.setTextBoxStyle(Slider::TextBoxBelow, false, 70, 16);
	noteLengthRightSlider_.setColour(Slider::textBoxTextColourId, Colours::grey.withAlpha(0.0f));
	noteLengthRightSlider_.setVisible(false);
    
	addAndMakeVisible (&feedbackSliderLeft_);
	feedbackSliderLeft_.setSliderStyle (Slider::Rotary);
	feedbackSliderLeft_.addListener (this);
	feedbackSliderLeft_.setRange (0.0, 0.995, 0.001);
	feedbackSliderLeft_.setTextBoxStyle(Slider::TextBoxBelow, false, 70, 16);
	feedbackSliderLeft_.setTextValueSuffix("  sec");

	addAndMakeVisible(&feedbackSliderRight_);
	feedbackSliderRight_.setSliderStyle(Slider::Rotary);
	feedbackSliderRight_.addListener(this);
	feedbackSliderRight_.setRange(0.0, 0.995, 0.001);
	feedbackSliderRight_.setTextBoxStyle(Slider::TextBoxBelow, false, 70, 16);
	feedbackSliderRight_.setTextValueSuffix("  sec");

	addAndMakeVisible(&inputGainSlider_);
	inputGainSlider_.setSliderStyle(Slider::Rotary);
	inputGainSlider_.addListener(this);
	inputGainSlider_.setRange(-75, 20, 0.1);
	inputGainSlider_.setTextValueSuffix(" dB");
	inputGainSlider_.setTextBoxStyle(Slider::TextBoxBelow, false, 70, 16);
	
	addAndMakeVisible(&outputGainSlider_);
	outputGainSlider_.setSliderStyle(Slider::Rotary);
	outputGainSlider_.addListener(this);
	outputGainSlider_.setRange(-75, 20, 0.1);
	outputGainSlider_.setTextValueSuffix(" dB");
	outputGainSlider_.setTextBoxStyle(Slider::TextBoxBelow, false, 70, 16);

    addAndMakeVisible (&dryMixSlider_);
    dryMixSlider_.setSliderStyle (Slider::Rotary);
    dryMixSlider_.addListener (this);
    dryMixSlider_.setRange (0.0, 1.0, 0.01);
	dryMixSlider_.setTextBoxStyle(Slider::TextBoxBelow, false, 40, 16);
	
    addAndMakeVisible (&wetMixSlider_);
    wetMixSlider_.setSliderStyle (Slider::Rotary);
    wetMixSlider_.addListener (this);
    wetMixSlider_.setRange (0.0, 1.0, 0.01);
	wetMixSlider_.setTextBoxStyle(Slider::TextBoxBelow, false, 40, 16);

	addAndMakeVisible(&panSlider_);
	panSlider_.setSliderStyle(Slider::Rotary);
	panSlider_.addListener(this);
	panSlider_.setRange(0.0, 1.0, 0.01);
	panSlider_.setTextBoxStyle(Slider::TextBoxBelow, false, 40, 16);

	addAndMakeVisible(&widenningSlider_);
	widenningSlider_.setSliderStyle(Slider::Rotary);
	widenningSlider_.addListener(this);
	widenningSlider_.setRange(0.0, 2.0, 0.01);
	widenningSlider_.setTextBoxStyle(Slider::TextBoxBelow, false, 40, 16);

	addAndMakeVisible(&lfoRateSlider_);
	lfoRateSlider_.setSliderStyle(Slider::Rotary);
	lfoRateSlider_.addListener(this);
	lfoRateSlider_.setRange(0.0001, 3.0000, 0.001);
	lfoRateSlider_.setTextBoxStyle(Slider::TextBoxBelow, false, 60, 16);
	lfoRateSlider_.setTextValueSuffix("  Hz");

	addAndMakeVisible(&lfoPhaseSlider_);
	lfoPhaseSlider_.setSliderStyle(Slider::Rotary);
	lfoPhaseSlider_.addListener(this);
	lfoPhaseSlider_.setRange(0.0, 1.0, 0.1);
	lfoPhaseSlider_.setTextBoxStyle(Slider::TextBoxBelow, false, 60, 16);
	
	addAndMakeVisible(&lfoRateNoteDivisionSlider_);
	lfoRateNoteDivisionSlider_.setSliderStyle(Slider::Rotary);
	lfoRateNoteDivisionSlider_.addListener(this);
	lfoRateNoteDivisionSlider_.setRange(1, 24, 1);
	lfoRateNoteDivisionSlider_.setTextBoxStyle(Slider::TextBoxBelow, false, 40, 16);
	lfoRateNoteDivisionSlider_.setColour(Slider::textBoxTextColourId, Colours::grey.withAlpha(0.0f));
	lfoRateNoteDivisionSlider_.setVisible(false);

	addAndMakeVisible(&cutoffSlider_);
	cutoffSlider_.setSliderStyle(Slider::Rotary);
	cutoffSlider_.addListener(this);
	cutoffSlider_.setRange(26.0f, 20000.0f, 1.0f);
	cutoffSlider_.setTextBoxStyle(Slider::TextBoxBelow, false, 60, 16);
	cutoffSlider_.setTextValueSuffix("  Hz");

	addAndMakeVisible(&resonanceSlider_);
	resonanceSlider_.setSliderStyle(Slider::Rotary);
	resonanceSlider_.addListener(this);
	resonanceSlider_.setRange(1.0f, 5.0f);
	resonanceSlider_.setTextBoxStyle(Slider::TextBoxBelow, false, 40, 16);
	resonanceSlider_.setTextValueSuffix("  Q");
	
	//LFO Modulation Parameters

	//LFO On/Off buttons
	addAndMakeVisible(modGroup_);
	modGroup_.setText("Modulation Mapping");
	modGroup_.setColour(GroupComponent::outlineColourId, Colours::white);
	
	addAndMakeVisible(&dryModButton_);
	dryModButton_.addListener(this);
	dryModButton_.setButtonText("Dry Mix");
	dryModButton_.setClickingTogglesState(true);
	dryModButton_.setLookAndFeel(&otherGUILookAndFeel);

	addAndMakeVisible(&wetModButton_);
	wetModButton_.addListener(this);
	wetModButton_.setButtonText("Wet Mix");
	wetModButton_.setClickingTogglesState(true);
	wetModButton_.setLookAndFeel(&otherGUILookAndFeel);

	addAndMakeVisible(&panModButton_);
	panModButton_.addListener(this);
	panModButton_.setButtonText("Pan Position");
	panModButton_.setClickingTogglesState(true);
	panModButton_.setLookAndFeel(&otherGUILookAndFeel);

	addAndMakeVisible(&cutoffModButton_);
	cutoffModButton_.addListener(this);
	cutoffModButton_.setButtonText("Cutoff Freq");
	cutoffModButton_.setClickingTogglesState(true);
	cutoffModButton_.setLookAndFeel(&otherGUILookAndFeel);

	addAndMakeVisible(&resModButton_);
	resModButton_.addListener(this);
	resModButton_.setButtonText("Resonance Q");
	resModButton_.setClickingTogglesState(true);
	resModButton_.setLookAndFeel(&otherGUILookAndFeel);

	// Amount of Modulation Sliders
	addAndMakeVisible(&dryModDepthSlider_);
	dryModDepthSlider_.setSliderStyle(Slider::LinearBar);
	dryModDepthSlider_.addListener(this);
	dryModDepthSlider_.setRange(0.0, 100.0f, 1.0f);
	dryModDepthSlider_.setTextValueSuffix(" %");
	dryModDepthSlider_.setColour(Slider::textBoxTextColourId, Colours::white);
	dryModDepthSlider_.setColour(Slider::trackColourId, Colours::blue.fromRGBA(0, 255, 255, 200));

	addAndMakeVisible(&wetModDepthSlider_);
	wetModDepthSlider_.setSliderStyle(Slider::LinearBar);
	wetModDepthSlider_.addListener(this);
	wetModDepthSlider_.setRange(0.0, 100.0f, 1.0f);
	wetModDepthSlider_.setTextValueSuffix(" %");
	wetModDepthSlider_.setColour(Slider::textBoxTextColourId, Colours::white);
	wetModDepthSlider_.setColour(Slider::trackColourId, Colours::blue.fromRGBA(0, 255, 255, 200));

	addAndMakeVisible(&panModDepthSlider_);
	panModDepthSlider_.setSliderStyle(Slider::LinearBar);
	panModDepthSlider_.addListener(this);
	panModDepthSlider_.setRange(0.0, 100.0f, 1.0f);
	panModDepthSlider_.setTextValueSuffix(" %");
	panModDepthSlider_.setColour(Slider::textBoxTextColourId, Colours::white);
	panModDepthSlider_.setColour(Slider::trackColourId, Colours::blue.fromRGBA(0, 255, 255, 200));

	addAndMakeVisible(&cutoffModDepthSlider_);
	cutoffModDepthSlider_.setSliderStyle(Slider::LinearBar);
	cutoffModDepthSlider_.addListener(this);
	cutoffModDepthSlider_.setRange(0.0, 100.0f, 1.0f);
	cutoffModDepthSlider_.setTextValueSuffix(" %");
	cutoffModDepthSlider_.setColour(Slider::textBoxTextColourId, Colours::white);
	cutoffModDepthSlider_.setColour(Slider::trackColourId, Colours::blue.fromRGBA(0, 255, 255, 200));

	addAndMakeVisible(&resModDepthSlider_);
	resModDepthSlider_.setSliderStyle(Slider::LinearBar);
	resModDepthSlider_.addListener(this);
	resModDepthSlider_.setRange(0.0, 100.0f, 1.0f);
	resModDepthSlider_.setTextValueSuffix(" %");
	resModDepthSlider_.setColour(Slider::textBoxTextColourId, Colours::white);
	resModDepthSlider_.setColour(Slider::trackColourId, Colours::blue.fromRGBA(0, 255, 255, 200));

	//SetUp ComboBox 
	addAndMakeVisible(&waveShapeBox_);
	waveShapeBox_.setJustificationType(Justification::centred);
	waveShapeBox_.addListener(this);
	waveShapeBox_.addItem("Sine", 1);
	waveShapeBox_.addItem("Saw", 2);
	waveShapeBox_.addItem("Tri", 3);
	waveShapeBox_.addItem("Sqr", 4);

	addAndMakeVisible(&filterMenu_);
	filterMenu_.setJustificationType(Justification::centred);
	filterMenu_.addListener(this);
	filterMenu_.addItem("LP", 1);
	filterMenu_.addItem("BP", 2);
	filterMenu_.addItem("HP", 3);
	

	//SetUp Buttons
	addAndMakeVisible(&tempoSyncLFOButton_);
	tempoSyncLFOButton_.addListener(this);
	tempoSyncLFOButton_.setButtonText("Sync");
	tempoSyncLFOButton_.setClickingTogglesState(true);
	tempoSyncLFOButton_.setLookAndFeel(&otherGUILookAndFeel);

	addAndMakeVisible(&tempoSyncButton_);
	tempoSyncButton_.addListener(this);
	tempoSyncButton_.setButtonText("Sync");
	tempoSyncButton_.setClickingTogglesState(true);
	tempoSyncButton_.setLookAndFeel(&otherGUILookAndFeel);

	addAndMakeVisible(&linkDelayButton_);
	linkDelayButton_.addListener(this);
	linkDelayButton_.setButtonText("Link");
	linkDelayButton_.setClickingTogglesState(true);
	linkDelayButton_.setLookAndFeel(&otherGUILookAndFeel);
	
	addAndMakeVisible(&lfoEnableButton_);
	lfoEnableButton_.addListener(this);

	//Labels
	inputGainLabel_.attachToComponent(&inputGainSlider_, false);
	inputGainLabel_.setFont(Font(12.0f));

	outputGainLabel_.attachToComponent(&outputGainSlider_, false);
	outputGainLabel_.setFont(Font(12.0f));
	
	delayLengthLabel_.attachToComponent(&delayLengthLeftSlider_, false);
	delayLengthLabel_.setFont(Font(12.0f));

	noteLengthLabel_.attachToComponent(&noteLengthLeftSlider_, false);
	noteLengthLabel_.setFont(Font(12.0f));

	addAndMakeVisible(&noteDivLeftLabel_);
	noteDivLeftLabel_.setFont(Font(12.0f));
	noteDivLeftLabel_.setVisible(false);

	addAndMakeVisible(&noteDivRightLabel_);
	noteDivRightLabel_.setFont(Font(12.0f));
	noteDivRightLabel_.setVisible(false);

	feedbackLabel_.attachToComponent(&feedbackSliderLeft_, false);
	feedbackLabel_.setFont(Font(12.0f));

	dryMixLabel_.attachToComponent(&dryMixSlider_, false);
	dryMixLabel_.setFont(Font (12.0f));
	
	wetMixLabel_.attachToComponent(&wetMixSlider_, false);
	wetMixLabel_.setFont(Font(12.0f));

	panLabel_.attachToComponent(&panSlider_, false);
	panLabel_.setFont(Font(12.0f));

	widenningLabel_.attachToComponent(&widenningSlider_, false);
	widenningLabel_.setFont(Font(12.0f));

	cutoffLabel_.attachToComponent(&cutoffSlider_, false);
	cutoffLabel_.setFont(Font(12.0f));

	resonanceLabel_.attachToComponent(&resonanceSlider_, false);
	resonanceLabel_.setFont(Font(12.0f));

	filterTypeLabel_.attachToComponent(&filterMenu_, false);
	filterTypeLabel_.setFont(Font(12.0f));

	shapeLabel_.attachToComponent(&waveShapeBox_, false);
	shapeLabel_.setFont(Font(12.0f));

	lfoRateLabel_.attachToComponent(&lfoRateSlider_, false);
	lfoRateLabel_.setFont(Font(12.0f));

	lfoRateSyncLabel_.attachToComponent(&lfoRateNoteDivisionSlider_, false);
	lfoRateSyncLabel_.setFont(Font(12.0f));

	addAndMakeVisible(&lfoRateSyncDisplayLabel_);
	lfoRateSyncDisplayLabel_.setFont(Font(12.0f));
	lfoRateSyncDisplayLabel_.setVisible(false);

	lfoPhaseLabel_.attachToComponent(&lfoPhaseSlider_, false);
	lfoPhaseLabel_.setFont(Font(12.0f));

	//Set Look & feel to GUI elements
	delayLengthLeftSlider_.setLookAndFeel(&delayKnobLookAndFeel);
	delayLengthRightSlider_.setLookAndFeel(&delayKnobLookAndFeel);
	noteLengthLeftSlider_.setLookAndFeel(&delayKnobLookAndFeel);
	noteLengthRightSlider_.setLookAndFeel(&delayKnobLookAndFeel);
	feedbackSliderLeft_.setLookAndFeel(&otherGUILookAndFeel);
	feedbackSliderRight_.setLookAndFeel(&otherGUILookAndFeel);
	inputGainSlider_.setLookAndFeel(&otherGUILookAndFeel);
	outputGainSlider_.setLookAndFeel(&otherGUILookAndFeel);
	dryMixSlider_.setLookAndFeel(&otherGUILookAndFeel);
	wetMixSlider_.setLookAndFeel(&otherGUILookAndFeel);
	panSlider_.setLookAndFeel(&otherGUILookAndFeel);
	widenningSlider_.setLookAndFeel(&otherGUILookAndFeel);
	lfoRateSlider_.setLookAndFeel(&otherGUILookAndFeel);
	lfoPhaseSlider_.setLookAndFeel(&otherGUILookAndFeel);
	lfoRateNoteDivisionSlider_.setLookAndFeel(&otherGUILookAndFeel);
	cutoffSlider_.setLookAndFeel(&otherGUILookAndFeel);
	resonanceSlider_.setLookAndFeel(&otherGUILookAndFeel);
	
	// add the triangular resizer component for the bottom-right of the UI
	addAndMakeVisible(resizer_ = new ResizableCornerComponent(this, &resizeLimits_));
	resizeLimits_.setSizeLimits(1000, 300, 1000, 300);

	// set our component's initial size to be the last one that was stored in the plugin settings
	setSize(ownerFilter->lastUIWidth_,
		ownerFilter->lastUIHeight_);

	startTimer(50);

}

DelayAudioProcessorEditor::~DelayAudioProcessorEditor()
{
}

//==============================================================================
void DelayAudioProcessorEditor::paint (Graphics& g)
{
   
	Image background = ImageCache::getFromMemory(BinaryData::Background_Labels_png, BinaryData::Background_Labels_pngSize);
	g.drawImageAt(background, 0, 0);
}

void DelayAudioProcessorEditor::resized()
{
	gainGroup_.setBounds(34, 16, 104, 250);
	inputGainSlider_.setBounds(53, 60, 70, 70);
	outputGainSlider_.setBounds(53, 164, 70, 70);
	
	delayGroup_.setBounds(141, 16, 248, 250);
	delayLengthLeftSlider_.setBounds(166, 55, 90, 90);
	delayLengthRightSlider_.setBounds(166, 160, 90, 90);

	noteLengthLeftSlider_.setBounds(166, 55, 90, 90);
	noteDivLeftLabel_.setBounds(182, 125, 80, 24);
	noteLengthRightSlider_.setBounds(166, 160, 90, 90);
	noteDivRightLabel_.setBounds(182, 230, 80, 24);
	
	feedbackSliderLeft_.setBounds (267, 60, 70, 70);
	feedbackSliderRight_.setBounds(267, 164, 70, 70);
	feedbackLabel_.setBounds(270, 32, 60, 24);
	linkDelayButton_.setBounds(338, 136, 40, 16);
	tempoSyncButton_.setBounds(338, 154, 40, 16);

	mixGroup_.setBounds(393, 16, 104, 250);
	dryMixSlider_.setBounds(411, 60, 70, 70);
	wetMixSlider_.setBounds(411, 164, 70, 70);

	panGroup_.setBounds(501, 16, 100, 250);
	panSlider_.setBounds(518, 60, 70, 70);
	widenningSlider_.setBounds(518, 164, 70, 70);
	
	filterGroup_.setBounds(605, 16, 100, 250);
	filterMenu_.setBounds(628, 50, 56, 24);
	cutoffSlider_.setBounds(626, 100, 64, 64);
	resonanceSlider_.setBounds(626, 190,64, 64);

	lfoGroup_.setBounds(709, 16, 260, 250);
	waveShapeBox_.setBounds(729, 50, 64, 24);
	lfoRateSlider_.setBounds(803, 50, 70, 70);
	lfoRateNoteDivisionSlider_.setBounds(803, 50, 70, 70);
	lfoRateSyncDisplayLabel_.setBounds(810, 74, 70, 70);
	lfoPhaseSlider_.setBounds(870, 50, 70, 70);
	tempoSyncLFOButton_.setBounds(729, 86, 40, 16);

	modGroup_.setBounds(727, 126, 228, 132);
	dryModButton_.setBounds(745, 148, 56, 18);
	wetModButton_.setBounds(745, 168, 56, 18);
	panModButton_.setBounds(745, 188, 56, 18);
	cutoffModButton_.setBounds(745, 208, 56, 18);
	resModButton_.setBounds(745, 228, 56, 18);

	dryModDepthSlider_.setBounds(828, 148, 112, 16);
	wetModDepthSlider_.setBounds(828, 168, 112, 16);
	panModDepthSlider_.setBounds(828, 188, 112, 16);
	cutoffModDepthSlider_.setBounds(828, 208, 112, 16);
	resModDepthSlider_.setBounds(828, 228, 112, 16);

}

//==============================================================================
// This timer periodically checks whether any of the parameters have changed...
void DelayAudioProcessorEditor::timerCallback()
{
    DelayAudioProcessor* ourProcessor = getProcessor();
    
	inputGainSlider_.setValue(ourProcessor->inputGain_, dontSendNotification);
	outputGainSlider_.setValue(ourProcessor->outputGain_, dontSendNotification);
	delayLengthLeftSlider_.setValue(ourProcessor->delayLengthLeft_, dontSendNotification);
	delayLengthRightSlider_.setValue(ourProcessor->delayLengthRight_, dontSendNotification);
	noteLengthLeftSlider_.setValue(ourProcessor->noteDivisionLeft_, dontSendNotification);
	noteLengthRightSlider_.setValue(ourProcessor->noteDivisionRight_, dontSendNotification);
	feedbackSliderLeft_.setValue(ourProcessor->feedbackLeft_, dontSendNotification);
	feedbackSliderRight_.setValue(ourProcessor->feedbackRight_, dontSendNotification);
    dryMixSlider_.setValue(ourProcessor->dryMix_, dontSendNotification);
    wetMixSlider_.setValue(ourProcessor->wetMix_, dontSendNotification);
	panSlider_.setValue(ourProcessor->panPosition_, dontSendNotification);
	widenningSlider_.setValue(ourProcessor->width_, dontSendNotification);
	cutoffSlider_.setValue(ourProcessor->cutoffFreq_, dontSendNotification);
	resonanceSlider_.setValue(ourProcessor->resValue_, dontSendNotification);
	lfoRateSlider_.setValue(ourProcessor->lfoRate_, dontSendNotification);
	lfoPhaseSlider_.setValue(ourProcessor->lfoPhase_, dontSendNotification);
	lfoRateNoteDivisionSlider_.setValue(ourProcessor->lfoRateSync_, dontSendNotification);
	tempoSyncLFOButton_.setToggleState(ourProcessor->syncLFO_, dontSendNotification);
	filterMenu_.setSelectedId(ourProcessor->filterType_, dontSendNotification);
	waveShapeBox_.setSelectedId(ourProcessor->waveType_, dontSendNotification);
	tempoSyncButton_.setToggleState(ourProcessor->syncBPM_, dontSendNotification);
	linkDelayButton_.setToggleState(ourProcessor->linkDelayTimes_, dontSendNotification);
	dryModButton_.setToggleState(ourProcessor->modDryMix_, dontSendNotification);
	wetModButton_.setToggleState(ourProcessor->modWetMix_, dontSendNotification);
	panModButton_.setToggleState(ourProcessor->modPan_, dontSendNotification);
	cutoffModButton_.setToggleState(ourProcessor->modFilterFreq_, dontSendNotification);
	resModButton_.setToggleState(ourProcessor->modResonance_, dontSendNotification);
	dryModDepthSlider_.setValue(ourProcessor->dryModAmount_, dontSendNotification);
	wetModDepthSlider_.setValue(ourProcessor->wetModAmount_, dontSendNotification);
	panModDepthSlider_.setValue(ourProcessor->panModAmount_, dontSendNotification);
	cutoffModDepthSlider_.setValue(ourProcessor->freqModAmount_, dontSendNotification);
	resModDepthSlider_.setValue(ourProcessor->resModAmount_, dontSendNotification);
	
	//check if host BPM has changed and if it has, recalculate delay times
	double BPM = getProcessor()->currentBPM_;

	if(BPM != lastBPMValue_)
	{
		lfoRateHZ.calculateHertz(BPM);
		delayTime_.calculateMilliseconds(BPM);
		lastBPMValue_ = BPM;
	
	}
	
	if (tempoSyncButton_.getToggleState() == false)
	{
		delayLengthLeftSlider_.setVisible(true);
		delayLengthRightSlider_.setVisible(true);
		noteLengthLeftSlider_.setVisible(false);
		noteLengthRightSlider_.setVisible(false);

	}else if (tempoSyncButton_.getToggleState() == true)
	{
		delayLengthLeftSlider_.setVisible(false);
		delayLengthRightSlider_.setVisible(false);
		noteLengthLeftSlider_.setVisible(true);
		noteLengthRightSlider_.setVisible(true);
		noteDivLeftLabel_.setVisible(true);
		noteDivRightLabel_.setVisible(true);

		string divisionLeft = noteDivisionDisplay(noteLengthLeftSlider_.getValue());
		noteDivLeftLabel_.setText(divisionLeft, dontSendNotification);

		string divisionRight = noteDivisionDisplay(noteLengthRightSlider_.getValue());
		noteDivRightLabel_.setText(divisionRight, dontSendNotification);

		if (linkDelayButton_.getToggleState() == false) 
		{ 
			noteLengthRightSlider_.setEnabled(true); 
			noteDivRightLabel_.setEnabled(true);
		}
		else if (linkDelayButton_.getToggleState() == true)
		{
			noteLengthRightSlider_.setEnabled(false);
			noteDivRightLabel_.setEnabled(false);

			getProcessor()->setParameterNotifyingHost(DelayAudioProcessor::kNoteLengthRightParam,
				(float)noteLengthLeftSlider_.getValue());

			getProcessor()->setParameterNotifyingHost(DelayAudioProcessor::kDelayLengthRightParam,
				findChosenNote((float)noteLengthLeftSlider_.getValue()));

			noteLengthRightSlider_.setValue(noteLengthLeftSlider_.getValue(), dontSendNotification);

			string division = noteDivisionDisplay(noteLengthLeftSlider_.getValue());
			noteDivRightLabel_.setText(division, dontSendNotification);
		}
		
	}

	if (tempoSyncLFOButton_.getToggleState() == true)
	{
		lfoRateSlider_.setVisible(false);
		lfoRateNoteDivisionSlider_.setVisible(true);

		string division = noteDivisionDisplay(lfoRateNoteDivisionSlider_.getValue());
		lfoRateSyncDisplayLabel_.setText(division, dontSendNotification);

		lfoRateSyncDisplayLabel_.setVisible(true);
	}
	else if (tempoSyncLFOButton_.getToggleState() == false)
	{
		lfoRateNoteDivisionSlider_.setVisible(false);
		lfoRateSyncDisplayLabel_.setVisible(false);
		lfoRateSlider_.setVisible(true);
	}
	
}

// This is our Slider::Listener callback, when the user drags a slider.
void DelayAudioProcessorEditor::sliderValueChanged (Slider* slider)
{
    // It's vital to use setParameterNotifyingHost to change any parameters that are automatable
    // by the host, rather than just modifying them directly, otherwise the host won't know
    // that they've changed.

	if (slider == &inputGainSlider_)
	{
		getProcessor()->setParameterNotifyingHost(DelayAudioProcessor::kInputGainParam,(float)inputGainSlider_.getValue());
	}
	else if (slider == &outputGainSlider_)
	{
		getProcessor()->setParameterNotifyingHost(DelayAudioProcessor::kOutputGainParam,(float)outputGainSlider_.getValue());
	}
	else if (slider == &delayLengthLeftSlider_)
    {
        getProcessor()->setParameterNotifyingHost (DelayAudioProcessor::kDelayLengthLeftParam,(float)delayLengthLeftSlider_.getValue());

		if (linkDelayButton_.getToggleState() == true)
		{
			getProcessor()->setParameterNotifyingHost(DelayAudioProcessor::kDelayLengthRightParam,(float)delayLengthLeftSlider_.getValue());

			delayLengthRightSlider_.setValue(delayLengthLeftSlider_.getValue(), dontSendNotification);
		}

    }else if (slider == &delayLengthRightSlider_)
	{

		getProcessor()->setParameterNotifyingHost(DelayAudioProcessor::kDelayLengthRightParam,(float)delayLengthRightSlider_.getValue());

		
	}else if (slider == &noteLengthLeftSlider_)
	{
		getProcessor()->setParameterNotifyingHost(DelayAudioProcessor::kNoteLengthLeftParam,(float)noteLengthLeftSlider_.getValue());

		string division = noteDivisionDisplay(noteLengthLeftSlider_.getValue());
		noteDivLeftLabel_.setText(division, dontSendNotification);

		
		 if (tempoSyncButton_.getToggleState() == true)
		{
			
			 getProcessor()->setParameterNotifyingHost(DelayAudioProcessor::kDelayLengthLeftParam, findChosenNote((float)noteLengthLeftSlider_.getValue()));

			 string Division = noteDivisionDisplay(noteLengthLeftSlider_.getValue());
			 noteDivLeftLabel_.setText(Division, dontSendNotification);

			 if (linkDelayButton_.getToggleState() == true)
			 {
				 getProcessor()->setParameterNotifyingHost(DelayAudioProcessor::kNoteLengthRightParam,(float)noteLengthLeftSlider_.getValue());

				 getProcessor()->setParameterNotifyingHost(DelayAudioProcessor::kDelayLengthRightParam, findChosenNote((float)noteLengthLeftSlider_.getValue()));

				 noteLengthRightSlider_.setValue(noteLengthLeftSlider_.getValue(), dontSendNotification);

				 string division = noteDivisionDisplay(noteLengthLeftSlider_.getValue());
				 noteDivRightLabel_.setText(division, dontSendNotification);
			 }
			 

		}
		
	}else if (slider == &noteLengthRightSlider_)
	{
		getProcessor()->setParameterNotifyingHost(DelayAudioProcessor::kNoteLengthRightParam, (float)noteLengthRightSlider_.getValue());

		string division = noteDivisionDisplay(noteLengthRightSlider_.getValue());
		noteDivRightLabel_.setText(division, dontSendNotification);
		
		if (tempoSyncButton_.getToggleState() == true)
		{ 
				getProcessor()->setParameterNotifyingHost(DelayAudioProcessor::kDelayLengthRightParam, findChosenNote((float)noteLengthRightSlider_.getValue()));

				string division = noteDivisionDisplay(noteLengthRightSlider_.getValue());
				noteDivRightLabel_.setText(division, dontSendNotification);
		}
		
	}else if (slider == &feedbackSliderLeft_)
	{
		getProcessor()->setParameterNotifyingHost(DelayAudioProcessor::kFeedbackLeftParam, (float)feedbackSliderLeft_.getValue());
	}
	else if (slider == &feedbackSliderRight_)
	{
		getProcessor()->setParameterNotifyingHost(DelayAudioProcessor::kFeedbackRightParam, (float)feedbackSliderRight_.getValue());
	}
    else if (slider == &dryMixSlider_)
    {
        getProcessor()->setParameterNotifyingHost (DelayAudioProcessor::kDryMixParam, (float)dryMixSlider_.getValue());
    }
    else if (slider == &wetMixSlider_)
    {
        getProcessor()->setParameterNotifyingHost (DelayAudioProcessor::kWetMixParam, (float)wetMixSlider_.getValue());
    }
	else if (slider == &panSlider_)
	{
		getProcessor()->setParameterNotifyingHost(DelayAudioProcessor::kPanPositionParam, (float)panSlider_.getValue());
		
	}
	else if (slider == &widenningSlider_)
	{
		getProcessor()->setParameterNotifyingHost(DelayAudioProcessor::kWidenningParam, (float)widenningSlider_.getValue());

	}
	else if (slider == &cutoffSlider_)
	{
		getProcessor()->setParameterNotifyingHost(DelayAudioProcessor::kCutoffParam, (float)cutoffSlider_.getValue());

	}
	else if (slider == &resonanceSlider_)
	{
		getProcessor()->setParameterNotifyingHost(DelayAudioProcessor::kResonanceParam, (float)resonanceSlider_.getValue());
	}
	else if (slider == &lfoRateSlider_)
	{
			getProcessor()->setParameterNotifyingHost(DelayAudioProcessor::kLFORateParam, (float)lfoRateSlider_.getValue());

	}else if (slider == &lfoRateNoteDivisionSlider_)
	{
		getProcessor()->setParameterNotifyingHost(DelayAudioProcessor::kLFORateSyncedParam, (float)lfoRateNoteDivisionSlider_.getValue());
		getProcessor()->setParameterNotifyingHost(DelayAudioProcessor::kLFORateParam, findLengthInHZ((float)lfoRateNoteDivisionSlider_.getValue()));

		string division = noteDivisionDisplay(lfoRateNoteDivisionSlider_.getValue());
		lfoRateSyncDisplayLabel_.setText(division, dontSendNotification);

		if (tempoSyncLFOButton_.getToggleState() == true)
		{
			getProcessor()->setParameterNotifyingHost(DelayAudioProcessor::kLFORateParam, findLengthInHZ((float)lfoRateNoteDivisionSlider_.getValue()));
			string division = noteDivisionDisplay(lfoRateNoteDivisionSlider_.getValue());
			lfoRateSyncDisplayLabel_.setText(division, dontSendNotification);
		}

	}
	else if (slider == &lfoPhaseSlider_)
	{
		getProcessor()->setParameterNotifyingHost(DelayAudioProcessor::kLFOPhaseParam, (float)lfoPhaseSlider_.getValue());

	}else if (slider == &dryModDepthSlider_)
	{
		getProcessor()->setParameterNotifyingHost(DelayAudioProcessor::kDryModAmountParam, (float)dryModDepthSlider_.getValue());

	}
	else if (slider == &wetModDepthSlider_)
	{
		getProcessor()->setParameterNotifyingHost(DelayAudioProcessor::kWetModAmountParam, (float)wetModDepthSlider_.getValue());

	}else if (slider == &panModDepthSlider_)
	{
		getProcessor()->setParameterNotifyingHost(DelayAudioProcessor::kPanModAmountParam, (float)panModDepthSlider_.getValue());

	}else if (slider == &cutoffModDepthSlider_)
	{
		getProcessor()->setParameterNotifyingHost(DelayAudioProcessor::kFreqModAmountParam, (float)cutoffModDepthSlider_.getValue());

	}
	else if (slider == &resModDepthSlider_)
	{
		getProcessor()->setParameterNotifyingHost(DelayAudioProcessor::kResModAmountParam, float)resModDepthSlider_.getValue());

	}

}

void DelayAudioProcessorEditor::buttonClicked(Button* buttonThatWasClicked)
{


	if (buttonThatWasClicked == &tempoSyncButton_)
	{
		getProcessor()->setParameterNotifyingHost(DelayAudioProcessor::kSyncBPMParam, (bool)tempoSyncButton_.getToggleState());

		findChosenNote((float)noteLengthLeftSlider_.getValue());
		findChosenNote((float)noteLengthRightSlider_.getValue());
		string divisionLeft = noteDivisionDisplay(noteLengthLeftSlider_.getValue());
		noteDivLeftLabel_.setText(divisionLeft, dontSendNotification);

		string divisionRight = noteDivisionDisplay(noteLengthRightSlider_.getValue());
		noteDivRightLabel_.setText(divisionRight, dontSendNotification);

		if (tempoSyncButton_.getToggleState() == false)
		{
			delayLengthLeftSlider_.setVisible(true);
			delayLengthRightSlider_.setVisible(true);
			noteLengthLeftSlider_.setVisible(false);
			noteLengthRightSlider_.setVisible(false);
			noteDivLeftLabel_.setVisible(false);
			noteDivRightLabel_.setVisible(false);


		}else if (tempoSyncButton_.getToggleState() == true)
		{
			delayLengthLeftSlider_.setVisible(false);
			delayLengthRightSlider_.setVisible(false);
			noteLengthLeftSlider_.setVisible(true);
			noteLengthRightSlider_.setVisible(true);
			noteDivLeftLabel_.setVisible(true);
			noteDivRightLabel_.setVisible(true);

			if(linkDelayButton_.getToggleState() == false)
			{

			getProcessor()->setParameterNotifyingHost(DelayAudioProcessor::kDelayLengthLeftParam, findChosenNote((float)noteLengthLeftSlider_.getValue()));


			getProcessor()->setParameterNotifyingHost(DelayAudioProcessor::kDelayLengthRightParam, findChosenNote((float)noteLengthRightSlider_.getValue()));
			}
			else if (linkDelayButton_.getToggleState() == true)
			{
				getProcessor()->setParameterNotifyingHost(DelayAudioProcessor::kNoteLengthRightParam, (float)noteLengthLeftSlider_.getValue());

				getProcessor()->setParameterNotifyingHost(DelayAudioProcessor::kDelayLengthRightParam, findChosenNote((float)noteLengthLeftSlider_.getValue()));

				noteLengthRightSlider_.setValue(noteLengthLeftSlider_.getValue(), dontSendNotification);

				string division = noteDivisionDisplay(noteLengthLeftSlider_.getValue());
				noteDivRightLabel_.setText(division, dontSendNotification);
			}

		}
		
	}else if (buttonThatWasClicked == &linkDelayButton_)
	{
	
		getProcessor()->setParameterNotifyingHost(DelayAudioProcessor::kLinkDelaysParam,
											(bool)linkDelayButton_.getToggleState());
		

			if (linkDelayButton_.getToggleState() == true)
			{
				if(tempoSyncButton_.getToggleState() == false)
				{
				delayLengthRightSlider_.setEnabled(false);
				delayLengthRightSlider_.setValue(getProcessor()->delayLengthLeft_, dontSendNotification);
				getProcessor()->setParameterNotifyingHost(DelayAudioProcessor::kDelayLengthRightParam, (float)delayLengthLeftSlider_.getValue());
				}
			}else if (linkDelayButton_.getToggleState() == false)
			{
				if (tempoSyncButton_.getToggleState() == false)
				{
					delayLengthRightSlider_.setEnabled(true);
				}
			}else if (linkDelayButton_.getToggleState() == true)
			{
				if (tempoSyncButton_.getToggleState() == true)
				{
					noteLengthRightSlider_.setEnabled(false);

					getProcessor()->setParameterNotifyingHost(DelayAudioProcessor::kNoteLengthRightParam, (float)noteLengthLeftSlider_.getValue());

					getProcessor()->setParameterNotifyingHost(DelayAudioProcessor::kDelayLengthRightParam, findChosenNote((float)noteLengthLeftSlider_.getValue()));

					noteLengthRightSlider_.setValue(noteLengthLeftSlider_.getValue(), dontSendNotification);

					string division = noteDivisionDisplay(noteLengthLeftSlider_.getValue());
					noteDivRightLabel_.setText(division, dontSendNotification);
				}
			}else if (linkDelayButton_.getToggleState() == false)
			{
				if (tempoSyncButton_.getToggleState() == true)
				{
					noteLengthRightSlider_.setEnabled(true);
					noteDivRightLabel_.setEnabled(true);
				}
			}
	}else if (buttonThatWasClicked == &tempoSyncLFOButton_)
	{	
		getProcessor()->setParameterNotifyingHost(DelayAudioProcessor::kLFOSyncParam, (bool)tempoSyncLFOButton_.getToggleState());

		string division = noteDivisionDisplay(lfoRateNoteDivisionSlider_.getValue());
		lfoRateSyncDisplayLabel_.setText(division, dontSendNotification);

		if (tempoSyncLFOButton_.getToggleState() == true)
		{
			findLengthInHZ((float)lfoRateSlider_.getValue());
			lfoRateSlider_.setVisible(false);
			lfoRateNoteDivisionSlider_.setVisible(true);
			lfoRateSyncDisplayLabel_.setVisible(true);
		}
		else if (tempoSyncLFOButton_.getToggleState() == false)
		{
			lfoRateNoteDivisionSlider_.setVisible(false);
			lfoRateSyncDisplayLabel_.setVisible(false);
			lfoRateSlider_.setVisible(true);
		}
		
	}else if (buttonThatWasClicked == &dryModButton_)
	{
		getProcessor()->setParameterNotifyingHost(DelayAudioProcessor::kModDryMixParam, (float)dryModButton_.getToggleState());
	}
	else if (buttonThatWasClicked == &wetModButton_)
	{
		getProcessor()->setParameterNotifyingHost(DelayAudioProcessor::kModWetMixParam, (float)wetModButton_.getToggleState());
	}
	else if (buttonThatWasClicked == &panModButton_)
	{
		getProcessor()->setParameterNotifyingHost(DelayAudioProcessor::kModPanParam, (float)panModButton_.getToggleState());
	}
	else if (buttonThatWasClicked == &cutoffModButton_)
	{
		getProcessor()->setParameterNotifyingHost(DelayAudioProcessor::kModFilterFreqParam, (float)cutoffModButton_.getToggleState());
	}
	else if (buttonThatWasClicked == &resModButton_)
	{
		getProcessor()->setParameterNotifyingHost(DelayAudioProcessor::kModResonanceParam, (float)resModButton_.getToggleState());
	}

}



// This is our comboBox::Listener callback, when the user makes a selection.
void DelayAudioProcessorEditor::comboBoxChanged(ComboBox* comboboxThatHasChanged)
{

	if (comboboxThatHasChanged == &waveShapeBox_)
	{
		getProcessor()->setParameterNotifyingHost(DelayAudioProcessor::kWaveTypeParam, (int)waveShapeBox_.getSelectedId());
	}
	else if (comboboxThatHasChanged == &filterMenu_)
	{
		getProcessor()->setParameterNotifyingHost(DelayAudioProcessor::kFilterTypeParam, (int)filterMenu_.getSelectedId());
	}
}



double DelayAudioProcessorEditor::findChosenNote(float noteChoice)
{

double noteLength;

		if (noteChoice == 4.0f)
		{
			noteLength = (double)delayTime_.getWholeDotted();

		}else if (noteChoice == 5.0f)
		{
			noteLength = (double)delayTime_.getWholeNormal();

		}else if (noteChoice == 6.0f)
		{
			noteLength = (double)delayTime_.getWholeTriplet();

		}else if (noteChoice == 7.0f)
		{
			noteLength = (double)delayTime_.getHalfDotted();

		}else if (noteChoice == 8.0f)
		{
			noteLength = (double)delayTime_.getHalfNormal();

		}else if (noteChoice == 9.0f)
		{
			noteLength = (double)delayTime_.getHalfTriplet();

		}else if (noteChoice == 10.0f)
		{
			noteLength = (double)delayTime_.getQuarterDotted();

		}else if (noteChoice == 11.0f)
		{
			noteLength = (double)delayTime_.getQuarterNormal();	

		}else if (noteChoice == 12.0f)
		{
			noteLength = (double)delayTime_.getQuarterTriplet();

		}else if (noteChoice == 13.0f)
		{
			noteLength = (double)delayTime_.getEighthDotted();

		}else if (noteChoice == 14.0f)
		{
			noteLength = (double)delayTime_.getEighthNormal();

		}else if (noteChoice == 15.0f)
		{
			noteLength = (double)delayTime_.getEighthTriplet();

		}else if (noteChoice == 16.0f)
		{
			noteLength = (double)delayTime_.getSixteenthDotted();

		}else if (noteChoice == 17.0f)
		{
			noteLength = (double)delayTime_.getSixteenthNormal();

		}else if (noteChoice == 18.0f)
		{
			noteLength = (double)delayTime_.getSixteenthTriplet();

		}else if (noteChoice == 19.0f)
		{
			noteLength = (double)delayTime_.getThirtySecondDotted();

		}else if (noteChoice == 20.0f)
		{
			noteLength = (double)delayTime_.getThirtySecondNormal();

		}else if (noteChoice == 21.0f)
		{
			noteLength = (double)delayTime_.getThirtySecondTriplet();

		}else if (noteChoice == 22.0f)
		{
			noteLength = (double)delayTime_.getSixtyFouthDotted();

		}else if (noteChoice == 23.0f)
		{
			noteLength = (double)delayTime_.getSixtyFouthNormal();

		}else if (noteChoice == 24.0f)
		{
			noteLength = (double)delayTime_.getSixtyFouthTriplet();

		}else if (noteChoice == 25.0f)
		{
			noteLength = (double)delayTime_.getOneHundredAndTwentyEighthDotted();

		}else if (noteChoice == 26.0f)
		{
			noteLength = (double)delayTime_.getOneHundredAndTwentyEighthNormal();

		}else if (noteChoice == 27.0f)
		{
			noteLength = (double)delayTime_.getOneHundredAndTwentyEighthTriplet();

		}
	
	return noteLength;
}




double DelayAudioProcessorEditor::findLengthInHZ(float noteChoice)
{


	double rateHz;

		if (noteChoice == 1.0f)
		{
			rateHz = (double)lfoRateHZ.getOctDotted();

		}else if (noteChoice == 2.0f)
		{
			rateHz = (double)lfoRateHZ.getOctNormal();

		}else if (noteChoice == 3.0f)
		{
			rateHz = (double)lfoRateHZ.getOctTriplet();

		}else if (noteChoice == 4.0f)
		{
			rateHz = (double)lfoRateHZ.getQuadDotted();

		}else if (noteChoice == 5.0f)
		{
			rateHz = (double)lfoRateHZ.getQuadNormal();

		}else if (noteChoice == 6.0f)
		{
				rateHz = (double)lfoRateHZ.getQuadTriplet();

		}else if (noteChoice == 7.0f)
		{
		rateHz = (double)lfoRateHZ.getDoubleDotted();

		}else if (noteChoice == 8.0f)
		{
			rateHz = (double)lfoRateHZ.getDoubleNormal();

		}else if (noteChoice == 9.0f)
		{
			rateHz = (double)lfoRateHZ.getDoubleTriplet();

		}else if (noteChoice == 10.0f)
		{
			rateHz = (double)lfoRateHZ.getWholeDotted();

		}else if (noteChoice == 11.0f)
		{
			rateHz = (double)lfoRateHZ.getWholeNormal();

		}else if (noteChoice == 12.0f)
		{
			rateHz = (double)lfoRateHZ.getWholeTriplet();

		}else if (noteChoice == 13.0f)
		{
			rateHz = (double)lfoRateHZ.getHalfDotted();

		}else if (noteChoice == 14.0f)
		{
			rateHz = (double)lfoRateHZ.getHalfNormal();

		}else if (noteChoice == 15.0f)
		{
			rateHz = (double)lfoRateHZ.getHalfTriplet();

		}else if (noteChoice == 16.0f)
		{
			rateHz = (double)lfoRateHZ.getQuarterDotted();

		}else if (noteChoice == 17.0f)
		{
			rateHz = (double)lfoRateHZ.getQuarterNormal();

		}else if (noteChoice == 18.0f)
		{
			rateHz = (double)lfoRateHZ.getQuarterTriplet();

		}else if (noteChoice == 19.0f)
		{
			rateHz = (double)lfoRateHZ.getEighthDotted();

		}else if (noteChoice == 20.0f)
		{
			rateHz = (double)lfoRateHZ.getEighthNormal();

		}else if (noteChoice == 21.0f)
		{
			rateHz = (double)lfoRateHZ.getEighthTriplet();

		}else if (noteChoice == 22.0f)
		{
			rateHz = (double)lfoRateHZ.getSixteenthDotted();

		}else if (noteChoice == 23.0f)
		{
			rateHz = (double)lfoRateHZ.getSixteenthNormal();

		}else if (noteChoice == 24.0f)
		{
			rateHz = (double)lfoRateHZ.getSixteenthTriplet();
		}

	return rateHz;
}


string DelayAudioProcessorEditor::noteDivisionDisplay(float noteChoice)
{
	string noteLength;
	
		if (noteChoice == 1.0f)
		{
			noteLength = "8 bars";

		}else if (noteChoice == 2.0f)
		{
			noteLength = "4 Bars";

		}else if (noteChoice == 3.0f)
		{
			noteLength = "2 Bars";

		}else if (noteChoice == 4.0f)
		{
			noteLength = "1/1 D";

		}else if (noteChoice == 5.0f)
		{
			noteLength = "1/1";

		}else if (noteChoice == 6.0f)
		{
			noteLength = "1/1 T";

		}else if (noteChoice == 7.0f)
		{
			noteLength = "1/2 D";

		}else if (noteChoice == 8.0f)
		{
			noteLength =  "1/2";

		}else if (noteChoice == 9.0f)
		{
			noteLength = "1/2 T";

		}else if (noteChoice == 10.0f)
		{
			noteLength = "1/4 D";

		}else if (noteChoice == 11.0f)
		{
			noteLength = "1/4";

		}else if (noteChoice == 12.0f)
		{
			noteLength = "1/4 T";

		}else if (noteChoice == 13.0f)
		{
			noteLength = "1/8 D";

		}else if (noteChoice == 14.0f)
		{
			noteLength = "1/8 ";
		}
		else if (noteChoice == 15.0f)
		{
			noteLength = "1/8 T";

		}else if (noteChoice == 16.0f)
		{
			noteLength = "1/16 D";

		}else if (noteChoice == 17.0f)
		{
			noteLength = "1/16";

		}else if (noteChoice == 18.0f)
		{
			noteLength = "1/16 T";

		}else if (noteChoice == 19.0f)
		{
			noteLength = "1/32 D";

		}else if (noteChoice == 20.0f)
		{
			noteLength = "1/32";

		}else if (noteChoice == 21.0f)
		{
			noteLength = "1/32 T";

		}else if (noteChoice == 22.0f)
		{
			noteLength = "1/64 D";

		}else if (noteChoice == 23.0f)
		{
			noteLength = "1/64";

		}else if (noteChoice == 24.0f)
		{
			noteLength = "1/64 T";

		}else if (noteChoice == 25.0f)
		{
			noteLength = "1/128 D";

		}else if (noteChoice == 26.0f)
		{
			noteLength = "1/128";

		}
		else if (noteChoice == 27.0f)
		{
			noteLength = "1/128 T";

		}
	
	return noteLength;

}


void DelayAudioProcessorEditor::updateDelayCalculatorValues(double tempo)
{
	delayTime_.calculateHertz(tempo);
	delayTime_.calculateMilliseconds(tempo);
}



