/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

#include "TitleComponent.h"
#include "MercyLookAndFeel.h"

//==============================================================================
/**
*/
class MercyAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    MercyAudioProcessorEditor (MercyAudioProcessor&);
    ~MercyAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void mouseEnter(const juce::MouseEvent& event) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    MercyAudioProcessor& audioProcessor;

    juce::Slider lpfCutoffSlider, hpfCutoffSlider, lpfResoSlider, hpfResoSlider, gainSlider;
    //juce::SliderParameterAttachment lpfCutoffSliderAttatchment, hpfCutoffSliderAttatchment, lpfResoSliderAttatchment, hpfResoSliderAttatchment, gainSliderAttatchment;

    MercyLookAndFeel mercyLookAndFeel;
    TitleComponent titleComponent;
    juce::Label descLabel, dbLevelMeterLeft, dbLevelMeterRight;
    juce::Font pluginFont;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MercyAudioProcessorEditor)
};
