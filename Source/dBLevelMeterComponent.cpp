/*
  ==============================================================================

    dBLevelMeterComponent.cpp
    Created: 19 Dec 2022 10:23:13am
    Author:  Kain

  ==============================================================================
*/

#include <JuceHeader.h>
#include "dBLevelMeterComponent.h"

//==============================================================================
dBLevelMeterComponent::dBLevelMeterComponent()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
}

dBLevelMeterComponent::~dBLevelMeterComponent()
{
}

void dBLevelMeterComponent::setLevelAndRepaint(float level) {
    this->level = level;
    repaint();
}

void dBLevelMeterComponent::paintGridOverComponent(juce::Graphics& g)
{
    auto numGridRects = 12.f;
    auto gridRectHeight = getLocalBounds().getHeight() / numGridRects;

    auto bottomLeft = getLocalBounds().getBottomLeft();


    for (auto i = 0; i < numGridRects; i++) {
        auto gridRectBottomLeft = bottomLeft.withY(gridRectHeight * i).toFloat();
        auto gridRectTopRight = bottomLeft.withX(bottomLeft.getX() + getLocalBounds().getWidth()).withY((gridRectHeight * i) + gridRectHeight).toFloat();
        auto gridRect = juce::Rectangle<float>(gridRectBottomLeft, gridRectTopRight);
        g.drawRect(gridRect);
    }

}

void dBLevelMeterComponent::paint (juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();
    g.setColour(juce::Colours::grey);
    g.fillRoundedRectangle(bounds, 5.f);

    auto mappedLevel = juce::jmap(level,-6 * 8.f, +0.f ,0.f, static_cast<float>(getHeight()));

    if (level >= 0.f) {
        g.setColour(juce::Colours::red);
    }
    else {
        g.setColour(juce::Colours::white);
    }

    g.fillRoundedRectangle(bounds.removeFromBottom(mappedLevel), 5.f);

    paintGridOverComponent(g);
}

void dBLevelMeterComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}
