/*
  ==============================================================================

    MercyLookAndFeel.h
    Created: 15 Dec 2022 3:26:46pm
    Author:  Kain

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class MercyLookAndFeel  : public juce::LookAndFeel_V4
{
public:
    MercyLookAndFeel();
    ~MercyLookAndFeel() override;

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

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MercyLookAndFeel)
};
