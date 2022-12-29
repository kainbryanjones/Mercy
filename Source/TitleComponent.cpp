/*
  ==============================================================================

    TitleComponent.cpp
    Created: 15 Dec 2022 12:53:30pm
    Author:  Kain

  ==============================================================================
*/

#include <JuceHeader.h>
#include "TitleComponent.h"
#include "MercyLookAndFeel.h"

//==============================================================================
TitleComponent::TitleComponent(juce::String title, juce::URL url)
{

    this->title = title;
    this->url = url;

}

TitleComponent::~TitleComponent()
{
}


void TitleComponent::mouseEnter(const juce::MouseEvent& event){
    //sets the mouse cursor to the "pointer" cursor
    setMouseCursor(juce::MouseCursor::StandardCursorType::PointingHandCursor);
    //component is repainted to update title text
    repaint();
}

void TitleComponent::mouseExit(const juce::MouseEvent& event) {
    //sets the mouse cursor back to the normal cursor
    setMouseCursor(juce::MouseCursor::StandardCursorType::NormalCursor);
    //component is repainted to update title text
    repaint();
}

void TitleComponent::mouseUp(const juce::MouseEvent& event) {
    //this if statement uses the URL classes isProbablyAWebsiteURL to determine
    //whether the URL should be launched.
    if (juce::URL::isProbablyAWebsiteURL(this->url.toString(false)))
    {
        //Opens URL address with default browser.
        this->url.launchInDefaultBrowser();
    }
}

void TitleComponent::paint (juce::Graphics& g)
{
    //boolean used to represent if the mouse is hovering over the title component
    auto mouseIsOverComponent = isMouseOver(true);

    //colour of the title font from lookandfeel
    auto fontColour = getLookAndFeel().findColour(MercyLookAndFeel::uiColourIds::titleComponentTextColourId);
    //background colour of title component from lookandfeel
    auto backgroundColour = getLookAndFeel().findColour(MercyLookAndFeel::uiColourIds::titleComponentBackgroundColourId);

    //modifiers change the title properties when the mouse is over the component
    //size modifier for title text
    auto fontHeightModifier = mouseIsOverComponent ? 0.825 : 0.85;
    //brightness modifier for title font
    auto brightnessModifier = mouseIsOverComponent ? 0.9 : 1;

    //set colour from lookandfeel
    g.setColour(backgroundColour);
    //fill the entire bounds with the background colour
    g.fillRect(getLocalBounds());

    //set colour from lookandfeel
    g.setColour(fontColour.withBrightness(brightnessModifier));

    //set font to the typeface created in the constructor
    //if the argument passed to setFont is a number then this changes the current font's height
    //here we use getLocalBounds().getHeight() to get the component height and multiply that by the
    //height modifier
    g.setFont(this->font);
    g.setFont(getLocalBounds().getHeight() * fontHeightModifier);

    //then we draw the text to fit within the local bounds of the component
    g.drawFittedText (this->title, getLocalBounds(), juce::Justification::centred, false); 
}

void TitleComponent::resized()
{
}

void TitleComponent::setFont(juce::Font font)
{
    this->font = font;
}
