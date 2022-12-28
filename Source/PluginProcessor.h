/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <../Source/ParamIds.h>

//==============================================================================
/**
*/
class MercyAudioProcessor : public juce::AudioProcessor
#if JucePlugin_Enable_ARA
	, public juce::AudioProcessorARAExtension
#endif
{
public:
	//==============================================================================
	MercyAudioProcessor();
	~MercyAudioProcessor() override;

	//==============================================================================
	void prepareToPlay(double sampleRate, int samplesPerBlock) override;
	void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
	bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

	void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

	//==============================================================================
	juce::AudioProcessorEditor* createEditor() override;
	bool hasEditor() const override;

	//==============================================================================
	const juce::String getName() const override;

	bool acceptsMidi() const override;
	bool producesMidi() const override;
	bool isMidiEffect() const override;
	double getTailLengthSeconds() const override;

	//==============================================================================
	int getNumPrograms() override;
	int getCurrentProgram() override;
	void setCurrentProgram(int index) override;
	const juce::String getProgramName(int index) override;
	void changeProgramName(int index, const juce::String& newName) override;

	//==============================================================================
	void getStateInformation(juce::MemoryBlock& destData) override;
	void setStateInformation(const void* data, int sizeInBytes) override;

	juce::AudioProcessorValueTreeState apvts;
	juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

	juce::Array<float> rmsLevels;
	float getRMSLevel(const int channel) const;

private:
	//==============================================================================

	//Smoothed Paramaters
	/*
	* Often in audio processing it is useful to have parameters which are "smooth". This means that
	* their values don't change immediately in response to a slider or button interaction, but rather they change over a given
	* ammount of time. This is because if we change something like the gain, for example, suddenly from -6dB to -18dB then the resulting
	* amplitude change causes an audible crack or pop. This can be avoided by smoothing the change over the time of about 0.3 seconds.
	* LinearSmoothValues change their value linearly so the gain will change by -4dB every 0.1 seconds for 0.3 seconds.
	*/
	juce::LinearSmoothedValue<float> lowPassCutoff, lowPassReso, highPassCutoff, highPassReso, gain;

	juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>,juce::dsp::IIR::Coefficients<float>> lowPassFilter;
	juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> highPassFilter;

	float rmsLevelLeft, rmsLevelRight;
	juce::LinearSmoothedValue<float> smoothedLevelLeft, smoothedLevelRight;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MercyAudioProcessor)
};
