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

/**
 Most of the Mercy documentation is in the header file for this LookAndFeel class.
 The code you see below is (mostly) original JUCE code copied from the LookAndFeel classes version 1 - 4.
 To see the difference between this code and the original please reference the original JUCE code located within the JUCE modules folder or the JUCE github https://github.com/juce-framework/JUCE
 
 The one exception to this howver is the drawRotarySlider method, which is commented with a  goal to understand what the JUCE code is doing
 so that you can edit it according to your specification. Something to note here is that rarely will you change absolutely every aspect of a LookAndFeel method. The JUCE
 team have gone to great lengths to make this code expandable and so commonly will you find useful work and calculations already done for you.
 
 The best way to learn from the LookAndFeel methods is to mess around with the code and see the results on the screen for yourself. It's all well and good seeing the addCentredArc method and probably understanding what that does, but the best way to get a feel for what it does is
 to actually see the resullt.
 
 Sidenote:
 If you intend to overtide a draw method and build the code from the ground then a basic understand of trigonometry will go far. For people without the most mathematical backgrounds(like the developer of Mercy)
 this can seem intimitading, so here are some useful topics along with some resources. Taking a bit of time to go over these topics will come in extremely handy and save a lot of faff.
 # Funamental Topics
 *  Triangles (SOH CAH TOA)
 * Unit Circles (Pi)
 * Trigonometric Function (Sin, Cos, Tan, ...)
 * Angles (Degrees / Radians)
 For an excellent all-in-1 resource for learning these concepts please refer to the playlist by The Organic Chemistry Tutor on YouTube https://www.youtube.com/playlist?list=PL0o_zxa4K1BVCB8iCVCGOES9pEF6byTMT
 
 Another sidenote:
 One brilliant thing about is JUCE(and C++) is that a lot of this low level mathematics has been done for you.
 # References
 *  JUCE Maths Modules : https://docs.juce.com/master/group__juce__core-maths.html
 * JUCE Maths Constants : https://docs.juce.com/master/structMathConstants.html
 * JUCE Degrees Helper Functions : https://docs.juce.com/master/classDecibels.html
 * C++ Standard Maths(cmath) : https://cplusplus.com/reference/cmath/
 */

MercyLookAndFeel::MercyLookAndFeel()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    
    setColourScheme(getLightColourScheme());
    
//    setColour(ResizableWindow::backgroundColourId, getCurrentColourScheme().getUIColour(getCurrentColourScheme().windowBackground));

}

/**
 For original code see JUCE modules > juce gui basics > lookandfeel > juce LookAndFeel V4
 */
void MercyLookAndFeel::drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider)
{
    //You will see edited code when it is commented with '//Edit'
    
    /**
     Colours fetched from the slider object. These can be changed  using the setColour method or by updating the colour scheme.
     */
    auto outline = slider.findColour(Slider::rotarySliderOutlineColourId);
    auto fill = slider.findColour(Slider::rotarySliderFillColourId);
    auto background = getCurrentColourScheme().getUIColour(getCurrentColourScheme().defaultText); //EDIT

    /**
     The rectangle object used to bound the paint method's output.
     The rectangle is positioned at the x and y of the slider's position with a width and height equivaelnt to the slider's dimensions.
     */
    auto bounds = Rectangle<int>(x, y, width, height).toFloat().reduced(10);

    //The radius of the slider is equal to half of either width or height, depending on which one is smaller
    auto radius = jmin(bounds.getWidth(), bounds.getHeight()) / 2.f;
    //The 'toAngle' represents the angle from the start to where the value is on the slider.
    //
    auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    //Width of the line is either half radius or 8.0f, whichever is smaller.
    auto lineW = jmin(8.0f, radius * 0.5f);
    //The 'arcRadius' is the horizontal/veritcal radius of the arc. As it is the same then the arc is a circle.
    //The arcRadius is the radius minus half the line width. This makes the arc radius fit just nicely in the middle of the outer line.
    auto arcRadius = radius - lineW * 0.5f;
    
    
    //The next 5 lines of code is not original JUCE code
    //The g.saveState method can be used to save the current state of the graphics object. This is so that we can make changes
    //and then restore that state if the changes are only small but reuire things such as colour to be changed.
    g.saveState(); //EDIT
    g.setColour(background); //EDIT
    //This drawns and fills elipse with radius half that of the arc radius and centered in the middle of the bounds.
    auto ellipseRectangle = juce::Rectangle<float>(arcRadius*2, arcRadius*2).withCentre(bounds.getCentre().toFloat()); //EDIT
    g.fillEllipse(ellipseRectangle); //EDIT
    g.restoreState(); //EDIT

    //The background arc is used to draw the slider's track.
    // The arc us centered around the bounds center. It has a radius which was calculated previously.
    //The 0.0f represents the angle reference for where the arc is measured from (which is straight down from the center).
    //The rotaryStartAngle and rotaryEndAngle are passed in to this method.
    //These are defined in the Slider class as
    /*
     rotaryParams.startAngleRadians = MathConstants<float>::pi * 1.2f;
     rotaryParams.endAngleRadians   = MathConstants<float>::pi * 2.8f;
     */
    Path backgroundArc;
    backgroundArc.addCentredArc(bounds.getCentreX(),
        bounds.getCentreY(),
        arcRadius,
        arcRadius,
        0.0f,
        rotaryStartAngle,
        rotaryEndAngle,
        true);

    //We then draw a line along the arc with the outline color.
    g.setColour(outline);
    g.strokePath(backgroundArc, PathStrokeType(lineW, PathStrokeType::curved, PathStrokeType::rounded));

    //When the slider is in 'diabled' mode the value of the slider isn't represented graphically
    if (slider.isEnabled())
    {
        Path valueArc; //This arc represents the value along the track.
        valueArc.addCentredArc(bounds.getCentreX(),
            bounds.getCentreY(),
            arcRadius,
            arcRadius,
            0.0f,
            rotaryStartAngle,
            toAngle, //This arc is exactly the same as the background arc except this time we use the previously calulated 'toAngle'.
            true);

        g.setColour(fill);
        g.strokePath(valueArc, PathStrokeType(lineW, PathStrokeType::curved, PathStrokeType::rounded));
    }

    //The thumb width is 1 and a half times larger than the line width.
    auto thumbWidth = lineW * 1.5f; //EDIT
    
    //An example of a useful calculation done by the JUCE team. This point is where the thumb will sit.
    Point<float> thumbPoint(bounds.getCentreX() + arcRadius * std::cos(toAngle - MathConstants<float>::halfPi),
        bounds.getCentreY() + arcRadius * std::sin(toAngle - MathConstants<float>::halfPi));

    //This line is drawn from the center to the thumb point. This is not oriignal JUCE code.
    Path middleLinePath; //EDIT
    middleLinePath.startNewSubPath(slider.getLocalBounds().getCentre().toFloat()); //EDIT
    middleLinePath.lineTo(thumbPoint); //EDIT
    middleLinePath.closeSubPath(); //EDIT
    g.strokePath(middleLinePath,juce::PathStrokeType(lineW));

    
    g.setColour(slider.findColour(Slider::thumbColourId));
    //Originally the JUCE code had an ellipse here however now the thumb is a rounded rectangle.
    g.fillRoundedRectangle(Rectangle<float>(thumbWidth , thumbWidth).withCentre(slider.getLocalBounds().getCentre().toFloat()), 4.f); //EDIT
}

Font MercyLookAndFeel::getComboBoxFont (ComboBox& box)
{
    return { jmax (16.0f, (float) box.getHeight() * 0.65f) };
}



void MercyLookAndFeel::drawLinearSlider (Graphics& g, int x, int y, int width, int height,
                                       float sliderPos,
                                       float minSliderPos,
                                       float maxSliderPos,
                                       const Slider::SliderStyle style, Slider& slider)
{
    if (slider.isBar())
    {
        g.setColour (slider.findColour (Slider::trackColourId));
        g.fillRect (slider.isHorizontal() ? Rectangle<float> (static_cast<float> (x), (float) y + 0.5f, sliderPos - (float) x, (float) height - 1.0f)
                                          : Rectangle<float> ((float) x + 0.5f, sliderPos, (float) width - 1.0f, (float) y + ((float) height - sliderPos)));
    }
    else
    {
        auto isTwoVal   = (style == Slider::SliderStyle::TwoValueVertical   || style == Slider::SliderStyle::TwoValueHorizontal);
        auto isThreeVal = (style == Slider::SliderStyle::ThreeValueVertical || style == Slider::SliderStyle::ThreeValueHorizontal);

        auto trackWidth = jmin (6.0f, slider.isHorizontal() ? (float) height * 0.25f : (float) width * 0.25f);

        Point<float> startPoint (slider.isHorizontal() ? (float) x : (float) x + (float) width * 0.5f,
                                 slider.isHorizontal() ? (float) y + (float) height * 0.5f : (float) (height + y));

        Point<float> endPoint (slider.isHorizontal() ? (float) (width + x) : startPoint.x,
                               slider.isHorizontal() ? startPoint.y : (float) y);

        Path backgroundTrack;
        backgroundTrack.startNewSubPath (startPoint);
        backgroundTrack.lineTo (endPoint);
        g.setColour (slider.findColour (Slider::backgroundColourId));
        g.strokePath (backgroundTrack, { trackWidth, PathStrokeType::curved, PathStrokeType::rounded });

        Path valueTrack;
        Point<float> minPoint, maxPoint, thumbPoint;

        if (isTwoVal || isThreeVal)
        {
            minPoint = { slider.isHorizontal() ? minSliderPos : (float) width * 0.5f,
                         slider.isHorizontal() ? (float) height * 0.5f : minSliderPos };

            if (isThreeVal)
                thumbPoint = { slider.isHorizontal() ? sliderPos : (float) width * 0.5f,
                               slider.isHorizontal() ? (float) height * 0.5f : sliderPos };

            maxPoint = { slider.isHorizontal() ? maxSliderPos : (float) width * 0.5f,
                         slider.isHorizontal() ? (float) height * 0.5f : maxSliderPos };
        }
        else
        {
            auto kx = slider.isHorizontal() ? sliderPos : ((float) x + (float) width * 0.5f);
            auto ky = slider.isHorizontal() ? ((float) y + (float) height * 0.5f) : sliderPos;

            minPoint = startPoint;
            maxPoint = { kx, ky };
        }

        auto thumbWidth = slider.getBounds().getWidth()*0.75f; //EDIT
        auto thumbHeight = thumbWidth / 8.f; //EDIT

        valueTrack.startNewSubPath (minPoint);
        valueTrack.lineTo (isThreeVal ? thumbPoint : maxPoint);
        g.setColour (slider.findColour (Slider::trackColourId));
        g.strokePath (valueTrack, { trackWidth, PathStrokeType::curved, PathStrokeType::rounded });

        if (! isTwoVal)
        {
            g.setColour (slider.findColour (Slider::thumbColourId)); //EDIT
            g.fillRoundedRectangle(Rectangle<float> (static_cast<float> (thumbWidth), static_cast<float> (thumbHeight)).withCentre (isThreeVal ? thumbPoint : maxPoint),1.f);
        }

        if (isTwoVal || isThreeVal)
        {
            auto sr = jmin (trackWidth, (slider.isHorizontal() ? (float) height : (float) width) * 0.4f);
            auto pointerColour = slider.findColour (Slider::thumbColourId);

            if (slider.isHorizontal())
            {
                drawPointer (g, minSliderPos - sr,
                             jmax (0.0f, (float) y + (float) height * 0.5f - trackWidth * 2.0f),
                             trackWidth * 2.0f, pointerColour, 2);

                drawPointer (g, maxSliderPos - trackWidth,
                             jmin ((float) (y + height) - trackWidth * 2.0f, (float) y + (float) height * 0.5f),
                             trackWidth * 2.0f, pointerColour, 4);
            }
            else
            {
                drawPointer (g, jmax (0.0f, (float) x + (float) width * 0.5f - trackWidth * 2.0f),
                             minSliderPos - trackWidth,
                             trackWidth * 2.0f, pointerColour, 1);

                drawPointer (g, jmin ((float) (x + width) - trackWidth * 2.0f, (float) x + (float) width * 0.5f), maxSliderPos - sr,
                             trackWidth * 2.0f, pointerColour, 3);
            }
        }
    }
}

MercyLookAndFeel::~MercyLookAndFeel()
{
}

