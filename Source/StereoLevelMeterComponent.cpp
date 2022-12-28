/*
  ==============================================================================

    StereoLevelMeterComponent.cpp
    Created: 28 Dec 2022 6:54:51am
    Author:  Kain

  ==============================================================================
*/

#include <JuceHeader.h>
#include "StereoLevelMeterComponent.h"

//==============================================================================
StereoLevelMeterComponent::StereoLevelMeterComponent()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    addAndMakeVisible(leftMeter);
    addAndMakeVisible(rightMeter);
}

StereoLevelMeterComponent::~StereoLevelMeterComponent()
{
}

void StereoLevelMeterComponent::paint (juce::Graphics& g)
{

}

void StereoLevelMeterComponent::resized()
{
    auto area = getLocalBounds();
    leftMeter.setBounds(area.removeFromLeft(area.getWidth() / 2));
    rightMeter.setBounds(area);

}

void StereoLevelMeterComponent::setLevelsAndRepaint(float leftLevel, float rightLevel)
{
    leftMeter.setLevelAndRepaint(leftLevel);
    rightMeter.setLevelAndRepaint(rightLevel);
}

void StereoLevelMeterComponent::paintOverChildren(juce::Graphics& g)
{

}
