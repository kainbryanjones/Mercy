/*
  ==============================================================================

	MercyLookAndFeel.h
	Created: 15 Dec 2022 3:26:46pm
	Author:  Kain

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

using namespace juce;

//==============================================================================
/*
*/
class MercyLookAndFeel : public juce::LookAndFeel_V4
{
public:
	MercyLookAndFeel();
	~MercyLookAndFeel() override;

	void drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider) override;
    
    void drawLinearSlider (Graphics& g, int x, int y, int width, int height,
                           float sliderPos,
                           float minSliderPos,
                           float maxSliderPos,
                           const Slider::SliderStyle style, Slider& slider) override;


    
	/*
	* For each GUI widget there are a collection of unique colour ids which are used
	* in the LookAndFeel's SetColour method. The id is an int value used to identify which part
	* of the widget should change colour.
	*
	* This enum is used to identify colours for custom components.
	* This solution was proposed on this forum thread.
	* https://forum.juce.com/t/custom-colour-ids-for-lookandfeel/43196/3
	*/
	enum uiColourIds
	{
		titleComponentTextColourId = uint32_t(0),
		titleComponentBackgroundColourId = uint32_t(1),
	};

	//ColourScheme lightColourScheme = { 0xffefefef, 0xffffffff, 0xffffffff, 0xffdddddd, 0xff000000, 0xffa9a9a9,0xffffffff, 0xff42a2c8, 0xff000000 };
	//ColourScheme darkColourScheme = { 0xff323e44, 0xff263238, 0xff323e44, 0xff8e989b, 0xffffffff, 0xff42a2c8, 0xffffffff, 0xff181f22, 0xffffffff };

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MercyLookAndFeel)
};
