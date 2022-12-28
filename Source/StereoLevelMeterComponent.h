/*
  ==============================================================================

    StereoLevelMeterComponent.h
    Created: 28 Dec 2022 6:54:51am
    Author:  Kain

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "dBLevelMeterComponent.h"
//==============================================================================
/*
*/
class StereoLevelMeterComponent  : public juce::Component
{
public:
    StereoLevelMeterComponent();
    ~StereoLevelMeterComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void setLevelsAndRepaint(float leftLevel, float rightLevel);
    void paintOverChildren(juce::Graphics&) override;

private:

    dBLevelMeterComponent leftMeter, rightMeter;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StereoLevelMeterComponent)
};
