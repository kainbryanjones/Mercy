/*
  ==============================================================================

    dBLevelMeterComponent.h
    Created: 19 Dec 2022 10:23:13am
    Author:  Kain

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class dBLevelMeterComponent  : public juce::Component
{
public:
    dBLevelMeterComponent();
    ~dBLevelMeterComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void setLevelAndRepaint(float level);

    void paintGridOverComponent(juce::Graphics&);

private:

    float level = 0.f;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (dBLevelMeterComponent)
};
