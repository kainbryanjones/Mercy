/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MercyAudioProcessorEditor::MercyAudioProcessorEditor(MercyAudioProcessor& p)
: AudioProcessorEditor(&p), audioProcessor(p), colourSchemeItemList("Dark Theme","Light Theme"), titleComponent(juce::String("www.halogensband.com"), juce::URL("https://www.halogensband.com")) {

	setLookAndFeel(&mercyLookAndFeel);

	titleComponent.setDescription(juce::String("Visit our website!"));
	lpfCutoffSlider.setDescription(juce::String("Low Pass Filter Cutoff"));
	lpfResoSlider.setDescription(juce::String("Low Pass Filter Resonance"));
	hpfCutoffSlider.setDescription(juce::String("High Pass Filter Cutoff"));
	hpfResoSlider.setDescription(juce::String("High Pass Filter Resonance"));
	gainSlider.setDescription(juce::String("Gain Control"));
	levelMeter.setDescription(juce::String("Decibel Level Meter"));
	bypassButton.setDescription(juce::String("Bypass dsp"));

	descLabel.setText("MERCY", juce::dontSendNotification);

	auto outlineColour = mercyLookAndFeel.getCurrentColourScheme().getUIColour(mercyLookAndFeel.getCurrentColourScheme().outline);
	auto fillColour = mercyLookAndFeel.getCurrentColourScheme().getUIColour(mercyLookAndFeel.getCurrentColourScheme().widgetBackground);
	auto textColour = mercyLookAndFeel.getCurrentColourScheme().getUIColour(mercyLookAndFeel.getCurrentColourScheme().defaultText);

	descLabel.setColour(juce::Label::ColourIds::outlineColourId, outlineColour);
	//descLabel.setColour(juce::Label::ColourIds::backgroundColourId, fillColour);
	descLabel.setColour(juce::Label::ColourIds::textColourId, textColour);
	valueLabel.setColour(juce::Label::ColourIds::outlineColourId, outlineColour);
	//valueLabel.setColour(juce::Label::ColourIds::backgroundColourId, fillColour);
	valueLabel.setColour(juce::Label::ColourIds::textColourId, textColour);

	descLabel.setFont(pluginFont);
	valueLabel.setFont(pluginFont);

	descLabel.setJustificationType(juce::Justification::centred);
	valueLabel.setJustificationType(juce::Justification::centred);

	addAndMakeVisible(titleComponent);
	addAndMakeVisible(lpfCutoffSlider);
	addAndMakeVisible(lpfResoSlider);
	addAndMakeVisible(hpfCutoffSlider);
	addAndMakeVisible(hpfResoSlider);
	addAndMakeVisible(gainSlider);
	addAndMakeVisible(levelMeter);
	addAndMakeVisible(valueLabel);
	addAndMakeVisible(descLabel);
	addAndMakeVisible(bypassButton);
    addAndMakeVisible(colourSchemeComboBox);
        

	for (auto child : getChildren())
	{
		if (auto slider = dynamic_cast<juce::Slider*>(child)) {
			slider->addListener(this);
			slider->setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
		}
	}

	lpfCutoffSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
	lpfResoSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
	hpfCutoffSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
	hpfResoSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
	gainSlider.setSliderStyle(juce::Slider::LinearVertical);

	bypassButton.setButtonText("bypass");

	/*
	* Custom font is stored as a .TTF file in the /assets/font folder.
	* The font is initialised using the juce::Typeface class' "createSystemTypefaceFor" method.
	* We pass this method the binary data using the BinaryData namespace. If you uploaded the .TTF file
	* through the projucer then the BinaryData will be auto generated.
	*/
	juce::Font titleFont = juce::Typeface::createSystemTypefaceFor(BinaryData::MOTOR__PERSONAL_USE_ONLY_ttf, BinaryData::MOTOR__PERSONAL_USE_ONLY_ttfSize);
	titleComponent.setFont(titleFont);

	lpfCutoffSliderAttatchment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, ParamIDs::lpfCutoff, lpfCutoffSlider);
	lpfResoSliderAttatchment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, ParamIDs::lpfReso, lpfResoSlider);

	hpfCutoffSliderAttatchment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, ParamIDs::hpfCutoff, hpfCutoffSlider);
	hpfResoSliderAttatchment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, ParamIDs::hpfReso, hpfResoSlider);

	gainSliderAttatchment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, ParamIDs::gain, gainSlider);

	bypassButtonAttatchment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, ParamIDs::bypass, bypassButton);

	std::function gainSliderTextFromValueFunction = [](double value) {
		auto textFromValue = juce::String();
		auto minusInfGain = -6 * 8.f;

		if (value >= 0.f) textFromValue.append(juce::String("+"), juce::String("+").length());
		value <= minusInfGain ? textFromValue.append("-INF ", juce::String("-INF ").length()) : textFromValue.append(juce::String(value), juce::String(value).length());

        return textFromValue;
	};
        
    std::function colourSchemeComboBoxChangedFunction = [this](){
        if (auto laf4 = dynamic_cast<juce::LookAndFeel_V4*> (&getLookAndFeel()))
        {
            auto colourScheme = colourSchemeComboBox.getSelectedIdAsValue() == 1 ? laf4->getGreyColourScheme() : laf4->getLightColourScheme();
            laf4->setColourScheme(colourScheme);
            repaint();
        }
    };
    
    colourSchemeComboBox.addItemList(colourSchemeItemList, 1);
    colourSchemeComboBox.setSelectedId(1);
    colourSchemeComboBox.setJustificationType(juce::Justification::centred);
    colourSchemeComboBox.onChange = colourSchemeComboBoxChangedFunction;
    
	gainSlider.textFromValueFunction = gainSliderTextFromValueFunction;

	lpfCutoffSlider.setTextValueSuffix("Hz");
	hpfCutoffSlider.setTextValueSuffix("Hz");

	gainSlider.setTextValueSuffix("dB");
	gainSlider.updateText();

	startTimerHz(24);

	addMouseListener(this, true);

	auto minWidth = 600;
	auto minHeight = 300;
	auto maxWidth = minWidth * 2;
	auto maxHeight = minHeight * 2;
	auto ratio = float(minWidth / minHeight);

	getConstrainer()->setSizeLimits(minWidth, minHeight, maxWidth, maxHeight);
	getConstrainer()->setFixedAspectRatio(ratio);
	setResizable(true, true);
	setSize(minWidth, minHeight);
}

MercyAudioProcessorEditor::~MercyAudioProcessorEditor()
{
	setLookAndFeel(nullptr);
}

//==============================================================================
void MercyAudioProcessorEditor::paint(juce::Graphics& g)
{
	// (Our component is opaque, so we must completely fill the background with a solid colour)
	g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
	g.setColour(juce::Colours::white);
	g.setColour(juce::Colours::green);

	auto filterParamBounds = juce::Rectangle<float>(lpfCutoffSlider.getBounds().getTopLeft().toFloat(), hpfResoSlider.getBounds().getBottomRight().toFloat());
	auto fillColour = mercyLookAndFeel.getCurrentColourScheme().getUIColour(mercyLookAndFeel.getCurrentColourScheme().widgetBackground);
	g.setColour(fillColour);
	g.fillRect(filterParamBounds);
}

void MercyAudioProcessorEditor::resized()
{

	auto area = getLocalBounds();


	auto filterAndLabelBounds = area.removeFromLeft(area.getWidth() / 3);
	auto filterParameterBounds = filterAndLabelBounds.removeFromTop(2 * filterAndLabelBounds.getHeight() / 3).toFloat();
	auto labelBounds = filterAndLabelBounds;

	auto gainSliderBounds = area.removeFromLeft(area.getWidth() / 2).toFloat();
	auto dbLevelMeterBounds = area.toFloat();
    auto titleComponentRect = dbLevelMeterBounds.removeFromBottom(dbLevelMeterBounds.getHeight() / 10).toNearestInt();

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
    
    auto comboBoxBounds = labelBounds.removeFromTop(labelBounds.getHeight() / 4 );
    colourSchemeComboBox.setBounds(comboBoxBounds);

	auto valueLabelBounds = labelBounds.removeFromBottom(labelBounds.getHeight() / 2);
	valueLabel.setBounds(valueLabelBounds);
    valueLabel.setFont(valueLabel.getLocalBounds().getHeight() * 0.65f);
	descLabel.setBounds(labelBounds);
    descLabel.setFont(descLabel.getLocalBounds().getHeight() * 0.65f);

	auto sliderTextBoxWidth = lpfCutoffSliderBounds.getWidth();

	gainSlider.setBounds(gainSliderBounds.toNearestInt().reduced(gainSliderBounds.getWidth() / 8.f, 0.f));

	levelMeter.setBounds(dbLevelMeterBounds.toNearestInt());

	titleComponent.setBounds(titleComponentRect);
}

void MercyAudioProcessorEditor::mouseEnter(const juce::MouseEvent& event)
{
	auto component = event.eventComponent;

	if (this->getChildren().contains(component) && !juce::String().equalsIgnoreCase(component->getDescription())) {

		auto descriptionLabelText = juce::String(event.eventComponent->getDescription());

		descLabel.setText(descriptionLabelText, juce::dontSendNotification);
		descLabel.repaint();
	}
	else {
		descLabel.setText(juce::String("Mercy"), juce::dontSendNotification);
		descLabel.repaint();
	}
}

void MercyAudioProcessorEditor::mouseExit(const juce::MouseEvent& event)
{

}

void MercyAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
	valueLabel.setText(slider->getTextFromValue(slider->getValue()), juce::dontSendNotification);
	valueLabel.repaint();
}

void MercyAudioProcessorEditor::sliderDragStarted(Slider* slider)
{
}

void MercyAudioProcessorEditor::sliderDragEnded(Slider* slider)
{
	valueLabel.setText("", juce::dontSendNotification);
	valueLabel.repaint();
}

void MercyAudioProcessorEditor::paintOverChildren(juce::Graphics& g)
{
	auto filterParamBounds = juce::Rectangle<float>(lpfCutoffSlider.getBounds().getTopLeft().toFloat(), hpfResoSlider.getBounds().getBottomRight().toFloat());
	auto outlineColour = mercyLookAndFeel.getCurrentColourScheme().getUIColour(mercyLookAndFeel.getCurrentColourScheme().outline);

	g.setColour(outlineColour);
	g.drawRect(filterParamBounds, 1.f);
}

void MercyAudioProcessorEditor::timerCallback()
{
	auto leftSmoothedRMS = audioProcessor.getSmoothedRMSLevel(0);
	auto rightSmoothedRMS = audioProcessor.getSmoothedRMSLevel(1);

	levelMeter.setLevelsAndRepaint(leftSmoothedRMS, rightSmoothedRMS);
}

void MercyAudioProcessorEditor::comboBoxChanged (juce::ComboBox *comboBoxThatHasChanged){
    
}

