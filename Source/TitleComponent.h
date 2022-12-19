/*
  ==============================================================================

    TitleComponent.h
    Created: 15 Dec 2022 12:53:30pm
    Author:  Kain

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
* TitleComponent is a custom component which presents a clickable piece of text. Upon clicking the text the
* user will be redirected to a URL which is declared in the constructor.
*/
class TitleComponent  : public juce::Component
{
public:
    
    TitleComponent(juce::String title, juce::URL url);
    ~TitleComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void setFont(juce::Font);

    void mouseEnter(const juce::MouseEvent& event) override;
    void mouseExit(const juce::MouseEvent& event) override;
    void mouseUp(const juce::MouseEvent& event) override;

private:

    juce::String title;
    juce::URL url;
    juce::Font font;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TitleComponent)
};
