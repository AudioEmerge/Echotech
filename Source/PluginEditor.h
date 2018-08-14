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

#ifndef __PLUGINEDITOR_H_6E48F605__
#define __PLUGINEDITOR_H_6E48F605__

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "DelayTimeCalculator.h"

//==============================================================================
class DelayKnobLookAndFeel : public LookAndFeel_V4
{
public:
	DelayKnobLookAndFeel()
	{
		
		setColour(Slider::textBoxOutlineColourId, Colours::grey.withAlpha(0.0f));
		
		
	}

	void drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos,
		const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider) override
	{

		

		// This is the binary image data that uses very little CPU when rotating
		Image delayKnobStrip = ImageCache::getFromMemory(BinaryData::DelayTimeKnobStrip_png, BinaryData::DelayTimeKnobStrip_pngSize);
		
		const double fractRotation = (slider.getValue() - slider.getMinimum()) / (slider.getMaximum() - slider.getMinimum()); //value between 0 and 1 for current amount of rotation
		const int nFrames = delayKnobStrip.getHeight() / delayKnobStrip.getWidth(); // number of frames for vertical film strip
		const int frameIdx = (int)ceil(fractRotation * ((double)nFrames - 1.0)); // current index from 0 --> nFrames-1

		const float radius = jmin(width / 2.0f, height / 2.0f);
		const float centreX = x + width * 0.5f;
		const float centreY = y + height * 0.5f;
		const float rx = centreX - radius - 1.0f;
		const float ry = centreY - radius - 1.0f;

		g.drawImage(delayKnobStrip,
			(int)rx,
			(int)ry,
			2 * (int)radius,
			2 * (int)radius,   //Dest
			0,
			frameIdx*delayKnobStrip.getWidth(),
			delayKnobStrip.getWidth(),
			delayKnobStrip.getWidth()); //Source
	}

};

//==============================================================================
class OtherGUILookAndFeel : public LookAndFeel_V4
{
public:
	OtherGUILookAndFeel()
	{
		setColour(Slider::textBoxOutlineColourId, Colours::grey.withAlpha(0.0f));
		setColour(TextButton::buttonColourId, Colours::transparentBlack.withAlpha(0.0f));
		setColour(TextButton::buttonOnColourId, Colours::transparentWhite);
		setColour(TextButton::textColourOffId, Colours::white);
		setColour(TextButton::textColourOnId, Colours::blue.fromRGB(0, 255, 255));
		setColour(TextButton::buttonColourId, Colours::grey.withAlpha(0.0f));
		setColour(TextButton::buttonOnColourId, Colours::blue.fromRGBA(0, 255, 255, 200));
		setColour(TextButton::textColourOffId, Colours::white);
		setColour(TextButton::textColourOnId, Colours::black);
		setColour(TextEditor::textColourId, Colours::black);
		setColour(TextEditor::backgroundColourId, Colour(0x00000000));
	}

	void drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos,
		const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider) override
	{
		Image paramsStrip = ImageCache::getFromMemory(BinaryData::ParamsRotaryKnob_png, BinaryData::ParamsRotaryKnob_pngSize);

		const double fractRotation = (slider.getValue() - slider.getMinimum()) / (slider.getMaximum() - slider.getMinimum()); //value between 0 and 1 for current amount of rotation
		const int nFrames = paramsStrip.getHeight() / paramsStrip.getWidth(); // number of frames for vertical film strip
		const int frameIdx = (int)ceil(fractRotation * ((double)nFrames - 1.0)); // current index from 0 --> nFrames-1

		const float radius = jmin(width / 2.0f, height / 2.0f);
		const float centreX = x + width * 0.5f;
		const float centreY = y + height * 0.5f;
		const float rx = centreX - radius - 1.0f;
		const float ry = centreY - radius - 1.0f;

		g.drawImage(paramsStrip,
			(int)rx,
			(int)ry,
			2 * (int)radius,
			2 * (int)radius,   //Dest
			0,
			frameIdx*paramsStrip.getWidth(),
			paramsStrip.getWidth(),
			paramsStrip.getWidth()); //Source
	}
};

//==============================================================================
class DelayAudioProcessorEditor  : public AudioProcessorEditor,
                                   public SliderListener,
								   public ComboBoxListener,
								   public ButtonListener,
                                   public Timer
{
public:
    DelayAudioProcessorEditor (DelayAudioProcessor* ownerFilter);
    ~DelayAudioProcessorEditor();

    //==============================================================================
    void timerCallback();
    void paint (Graphics& g);
    void resized();
    void sliderValueChanged (Slider*);
	void comboBoxChanged(ComboBox*);
	void buttonClicked(Button* buttonThatWasClicked);
	double findChosenNote(float noteChoice);
	double findLengthInHZ(float noteChoice);
	void updateDelayCalculatorValues(double tempo);
	string noteDivisionDisplay(float noteChoice);
	double lastBPMValue_;
    
private:
	DelayKnobLookAndFeel delayKnobLookAndFeel;
	OtherGUILookAndFeel otherGUILookAndFeel;
	Label inputGainLabel_, outputGainLabel_, delayLengthLabel_, feedbackLabel_, dryMixLabel_, wetMixLabel_, noteDivLeftLabel_, noteDivRightLabel_, panLabel_, widenningLabel_, cutoffLabel_, resonanceLabel_, filterTypeLabel_, shapeLabel_, noteLengthLabel_, lfoEnableLabel_,
	lfoRateLabel_, lfoPhaseLabel_, lfoRateSyncLabel_, lfoRateSyncDisplayLabel_, cutoffModLabel_;
    Slider inputGainSlider_, outputGainSlider_, delayLengthLeftSlider_, delayLengthRightSlider_, feedbackSliderLeft_, feedbackSliderRight_, dryMixSlider_, wetMixSlider_, panSlider_, widenningSlider_, cutoffSlider_, 
	resonanceSlider_, noteLengthLeftSlider_, noteLengthRightSlider_, lfoRateSlider_, lfoPhaseSlider_, lfoRateNoteDivisionSlider_, dryModDepthSlider_, wetModDepthSlider_, panModDepthSlider_, cutoffModDepthSlider_, resModDepthSlider_;
	ComboBox waveShapeBox_, filterMenu_;
	DelayTimeCalculator delayTime_, lfoRateHZ;
	TextButton linkDelayButton_, tempoSyncButton_, tempoSyncLFOButton_, lfoEnableButton_, panModButton_, cutoffModButton_, resModButton_, dryModButton_, wetModButton_;
	GroupComponent gainGroup_, delayGroup_, panGroup_, lfoGroup_, modGroup_, filterGroup_, mixGroup_;
	 ScopedPointer<ResizableCornerComponent> resizer_;
    ComponentBoundsConstrainer resizeLimits_;
    
    DelayAudioProcessor* getProcessor() const
    {
        return static_cast <DelayAudioProcessor*> (getAudioProcessor());
    }
};


#endif  // __PLUGINEDITOR_H_6E48F605__
