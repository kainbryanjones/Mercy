/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MercyAudioProcessorEditor::MercyAudioProcessorEditor(MercyAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p), titleComponent(juce::String("www.halogensband.com"), juce::URL("www.halogensband.com")) {

    setLookAndFeel(&mercyLookAndFeel);

    titleComponent.setDescription(juce::String("Visit our website!"));
    addAndMakeVisible(titleComponent);
    lpfCutoffSlider.setDescription(juce::String());
    addAndMakeVisible(lpfCutoffSlider);
    lpfResoSlider.setDescription(juce::String());
    addAndMakeVisible(lpfResoSlider);
    hpfCutoffSlider.setDescription(juce::String());
    addAndMakeVisible(hpfCutoffSlider);
    hpfResoSlider.setDescription(juce::String());
    addAndMakeVisible(hpfResoSlider);
    gainSlider.setDescription(juce::String());
    addAndMakeVisible(gainSlider);
    dbLevelMeterLeft.setDescription(juce::String());
    addAndMakeVisible(dbLevelMeterLeft);
    dbLevelMeterRight.setDescription(juce::String());
    addAndMakeVisible(dbLevelMeterRight);
    descLabel.setText("MERCY", juce::dontSendNotification);
    addAndMakeVisible(descLabel);

    lpfCutoffSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    lpfCutoffSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);

    lpfResoSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    lpfResoSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);

    hpfCutoffSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    hpfCutoffSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);

    hpfResoSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    hpfResoSlider.setTextBoxStyle(juce::Slider::TextBoxAbove, true, 0, 0);

    gainSlider.setSliderStyle(juce::Slider::LinearBarVertical);

    //gainSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);

    /*
* Custom font is stored as a .TTF file in the /assets/font folder.
* The font is initialised using the juce::Typeface class' "createSystemTypefaceFor" method.
* We pass this method the binary data using the BinaryData namespace. If you uploaded the .TTF file
* through the projucer then the BinaryData will be auto generated.
*/
    juce::Font titleFont = juce::Typeface::createSystemTypefaceFor(BinaryData::MOTOR__PERSONAL_USE_ONLY_TTF, BinaryData::MOTOR__PERSONAL_USE_ONLY_TTFSize);
    titleComponent.setFont(titleFont);

    lpfCutoffSliderAttatchment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, ParamIDs::lpfCutoff, lpfCutoffSlider);
    lpfResoSliderAttatchment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, ParamIDs::lpfReso, lpfResoSlider);

    hpfCutoffSliderAttatchment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, ParamIDs::hpfCutoff, hpfCutoffSlider);
    hpfResoSliderAttatchment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, ParamIDs::hpfReso, hpfResoSlider);

    gainSliderAttatchment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, ParamIDs::gain, gainSlider);
    gainSlider.textFromValueFunction = [](double value) {
        auto textFromValue = juce::String();
        auto minusInfGain = -6 * 8.f;
        value <= minusInfGain ? textFromValue.append("-INF ", juce::String("-INF ").length()) : textFromValue.append(juce::String(value), juce::String(value).length());
        return textFromValue;
    };
    gainSlider.setTextValueSuffix("dB");
    gainSlider.updateText();

    addMouseListener(this, true);
    setResizable(true, true);
    setSize (400, 300);
}

MercyAudioProcessorEditor::~MercyAudioProcessorEditor()
{
    setLookAndFeel(nullptr);
}

//==============================================================================
void MercyAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    g.setColour (juce::Colours::white);
    g.setColour(juce::Colours::green);
}

void MercyAudioProcessorEditor::resized()
{

    //area represents the bounds of the entire plugin
    //this object is used to keep track of what area remains where
    //we can place child components, such as sliders and buttons
    auto area = getLocalBounds();

    //the title component spans the entire width of the component
    //we can use area.getHeight() to get the height of the remaining area
    //here is the first call so the remaining area is all of the local bounds
    //however we use area.removeFromBottom to subtract a Rectangle object of height 1/10th of the total area
    //this means that every call of area.getHeight() from now on will be of the newly shrunk area
    //any call of removeFromBottom / removeFromTop / removeFromLeft / removeFromRight with shrink the area even further
    //notice that this rectangle is not a global object this is explained in the paint method
    auto titleComponentRect = area.removeFromBottom(area.getHeight() / 10);

    //now we can use a new inner-rectangle "filterParameterBounds" to place the filter parameters
    //after this call area is 1/10th less tall and 1/3rd less wide from when it was originally created
    //notice the .toFloat() method this is because the global object filterParameterBounds was declared
    //as a juce::Rectangle<float> object
    auto filterAndLabelBounds = area.removeFromLeft(area.getWidth() / 3);
    auto filterParameterBounds = filterAndLabelBounds.removeFromTop(2 * filterAndLabelBounds.getHeight() / 3).toFloat();
    auto labelBounds = filterAndLabelBounds;

    auto gainSliderBounds = area.removeFromLeft(area.getWidth() / 2).toFloat();
    auto dbLevelMeterBounds = area.toFloat();

    auto dbLevelMeterLeftBounds = dbLevelMeterBounds.removeFromLeft(dbLevelMeterBounds.getWidth() / 2);
    auto dbLevelMeterRightBounds = dbLevelMeterBounds.toFloat();

    auto parameterSliderHeight = filterParameterBounds.getHeight() / 2;
    auto parameterSliderWidth = filterParameterBounds.getWidth() / 2;

    auto lpfBounds = filterParameterBounds.removeFromTop(parameterSliderHeight);
    auto lpfCutoffSliderBounds = lpfBounds.removeFromLeft(parameterSliderWidth).toNearestInt();
    auto lpfResoSliderBounds = lpfBounds.toNearestInt();

    auto hpfBounds = filterParameterBounds.removeFromTop(parameterSliderHeight);
    auto hpfCutoffSliderBounds = hpfBounds.removeFromLeft(parameterSliderWidth).toNearestInt();
    auto hpfResoSliderBounds = hpfBounds.toNearestInt();

    lpfCutoffSlider.setBounds(lpfCutoffSliderBounds);
    lpfResoSlider.setBounds(lpfResoSliderBounds);
    hpfCutoffSlider.setBounds(hpfCutoffSliderBounds);
    hpfResoSlider.setBounds(hpfResoSliderBounds);
    descLabel.setBounds(labelBounds);

    gainSlider.setBounds(gainSliderBounds.toNearestInt());

    dbLevelMeterLeft.setBounds(dbLevelMeterLeftBounds.toNearestInt());
    dbLevelMeterRight.setBounds(dbLevelMeterRightBounds.toNearestInt());

    titleComponent.setBounds(titleComponentRect);
}

void MercyAudioProcessorEditor::mouseEnter(const juce::MouseEvent& event)
{
    if (this->getChildren().contains(event.eventComponent)){
        descLabel.setText(event.eventComponent->getDescription(), juce::dontSendNotification);
        repaint();
    }
}
