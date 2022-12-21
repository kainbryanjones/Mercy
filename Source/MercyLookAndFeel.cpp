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

    setColourScheme(darkColourScheme);

    setColour(juce::ResizableWindow::backgroundColourId, getCurrentColourScheme().getUIColour(getCurrentColourScheme().windowBackground));

    setColour(uiColourIds::titleComponentTextColourId, getCurrentColourScheme().getUIColour(getCurrentColourScheme().defaultText));
    setColour(uiColourIds::titleComponentBackgroundColourId, getCurrentColourScheme().getUIColour(getCurrentColourScheme().widgetBackground));

}

MercyLookAndFeel::~MercyLookAndFeel()
{
}

