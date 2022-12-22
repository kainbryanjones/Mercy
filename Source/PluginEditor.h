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
class MercyAudioProcessorEditor  : public juce::AudioProcessorEditor, juce::Slider::Listener
{
public:
    MercyAudioProcessorEditor (MercyAudioProcessor&);
    ~MercyAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void mouseEnter(const juce::MouseEvent& event) override;
    void mouseExit(const juce::MouseEvent& event) override;

    void sliderValueChanged(juce::Slider*) override;
    void sliderDragStarted(juce::Slider*) override;
    void sliderDragEnded(juce::Slider*) override;


private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    MercyAudioProcessor& audioProcessor;

    juce::Label descLabel, valueLabel, dbLevelMeterLeft, dbLevelMeterRight;
    juce::Font pluginFont;

    juce::Slider lpfCutoffSlider, hpfCutoffSlider, lpfResoSlider, hpfResoSlider, gainSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lpfCutoffSliderAttatchment, hpfCutoffSliderAttatchment, lpfResoSliderAttatchment, hpfResoSliderAttatchment, gainSliderAttatchment;

    juce::ToggleButton bypassButton;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> bypassButtonAttatchment;

    MercyLookAndFeel mercyLookAndFeel;

    TitleComponent titleComponent;



    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MercyAudioProcessorEditor)
};
