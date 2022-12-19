/*
  ==============================================================================

    MercyLookAndFeel.cpp
    Created: 15 Dec 2022 3:26:46pm
    Author:  Kain

  ==============================================================================
*/

#include <JuceHeader.h>
#include "MercyLookAndFeel.h"

//==============================================================================
MercyLookAndFeel::MercyLookAndFeel()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    setColour(juce::ResizableWindow::backgroundColourId, juce::Colours::mistyrose);

    setColour(uiColourIds::titleComponentTextColourId, juce::Colours::floralwhite);
    setColour(uiColourIds::titleComponentBackgroundColourId, juce::Colours::coral);

}

MercyLookAndFeel::~MercyLookAndFeel()
{
}

