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

    setColour(ResizableWindow::backgroundColourId, getCurrentColourScheme().getUIColour(getCurrentColourScheme().windowBackground));

    setColour(uiColourIds::titleComponentTextColourId, getCurrentColourScheme().getUIColour(getCurrentColourScheme().defaultText));
    setColour(uiColourIds::titleComponentBackgroundColourId, getCurrentColourScheme().getUIColour(getCurrentColourScheme().widgetBackground));

}

void MercyLookAndFeel::drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider)
{
    auto outline = slider.findColour(Slider::rotarySliderOutlineColourId);
    auto fill = slider.findColour(Slider::rotarySliderFillColourId);
    auto background = getCurrentColourScheme().getUIColour(getCurrentColourScheme().defaultText);

    auto bounds = Rectangle<int>(x, y, width, height).toFloat().reduced(10);

    auto radius = jmin(bounds.getWidth(), bounds.getHeight()) / 2.f;
    auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    auto lineW = jmin(8.0f, radius * 0.5f) / 2.f;
    auto arcRadius = radius - lineW * 0.5f;

    g.saveState();
    g.setColour(background);
    //auto ellipseDiameter = jmin(slider.getLocalBounds().getWidth(), slider.getLocalBounds().getHeight());
    auto ellipseRectangle = juce::Rectangle<float>(arcRadius*2, arcRadius*2).withCentre(slider.getLocalBounds().getCentre().toFloat());
    g.fillEllipse(ellipseRectangle);
    g.restoreState();

    Path backgroundArc;
    backgroundArc.addCentredArc(bounds.getCentreX(),
        bounds.getCentreY(),
        arcRadius,
        arcRadius,
        0.0f,
        rotaryStartAngle,
        rotaryEndAngle,
        true);

    g.setColour(outline);
    g.strokePath(backgroundArc, PathStrokeType(lineW, PathStrokeType::curved, PathStrokeType::rounded));

    if (slider.isEnabled())
    {
        Path valueArc;
        valueArc.addCentredArc(bounds.getCentreX(),
            bounds.getCentreY(),
            arcRadius,
            arcRadius,
            0.0f,
            rotaryStartAngle,
            toAngle,
            true);

        g.setColour(fill);
        g.strokePath(valueArc, PathStrokeType(lineW, PathStrokeType::curved, PathStrokeType::rounded));
    }

    auto thumbWidth = lineW;
    Point<float> thumbPoint(bounds.getCentreX() + arcRadius * std::cos(toAngle - MathConstants<float>::halfPi),
        bounds.getCentreY() + arcRadius * std::sin(toAngle - MathConstants<float>::halfPi));

    Path middleLinePath;
    middleLinePath.startNewSubPath(slider.getLocalBounds().getCentre().toFloat());
    middleLinePath.lineTo(thumbPoint);
    middleLinePath.closeSubPath();

    g.strokePath(middleLinePath,juce::PathStrokeType(lineW));

    g.setColour(slider.findColour(Slider::thumbColourId));
    //g.fillRoundedRectangle(Rectangle<float>(thumbWidth, thumbWidth).withCentre(thumbPoint),4.f);
    g.fillRoundedRectangle(Rectangle<float>(thumbWidth * 1.5f , thumbWidth * 1.5f).withCentre(slider.getLocalBounds().getCentre().toFloat()), 4.f);
}


MercyLookAndFeel::~MercyLookAndFeel()
{
}

