/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MercyAudioProcessor::MercyAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
	: AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
		.withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
		.withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
	),
#endif
	apvts(*this, nullptr, "Paramters", createParameterLayout()), lowPassFilter(juce::dsp::IIR::Coefficients<float>::makeLowPass(44100.f, 20000.f, 0.1f)), highPassFilter(juce::dsp::IIR::Coefficients<float>::makeHighPass(44100.f, 20.f, 0.1f))
{

}

juce::AudioProcessorValueTreeState::ParameterLayout MercyAudioProcessor::createParameterLayout() {

	juce::AudioProcessorValueTreeState::ParameterLayout layout;

	const auto frequencyRange = juce::NormalisableRange<float>{ 20.0f, 20000.0f, 1.f, 0.25f };
	const auto lpfCutoffDefaultValue = frequencyRange.getRange().getEnd();
	const auto hpfCutoffDefaultValue = frequencyRange.getRange().getStart();

	const auto resoRange = juce::NormalisableRange<float>{ 0.1f, 10.0f, 1.f };
	const float lpfResoDefaultValue = resoRange.getRange().getStart();
	const float hpfResoDefaultValue = resoRange.getRange().getStart();

	layout.add(std::make_unique<juce::AudioParameterFloat>(
		juce::ParameterID{ ParamIDs::lpfCutoff,1 },
		ParamIDs::lpfCutoff,
		frequencyRange,
		lpfCutoffDefaultValue,
		juce::String(),
		juce::AudioProcessorParameter::genericParameter));

	layout.add(std::make_unique<juce::AudioParameterFloat>(
		juce::ParameterID{ ParamIDs::lpfReso,1 },
		ParamIDs::lpfReso,
		resoRange,
		lpfResoDefaultValue,
		juce::String(),
		juce::AudioProcessorParameter::genericParameter));

	layout.add(std::make_unique<juce::AudioParameterFloat>(
		juce::ParameterID{ ParamIDs::hpfCutoff,1 },
		ParamIDs::hpfCutoff,
		frequencyRange,
		hpfCutoffDefaultValue,
		juce::String(),
		juce::AudioProcessorParameter::genericParameter));

	layout.add(std::make_unique<juce::AudioParameterFloat>(
		juce::ParameterID{ ParamIDs::hpfReso,1 },
		ParamIDs::hpfReso,
		resoRange,
		hpfResoDefaultValue,
		juce::String(),
		juce::AudioProcessorParameter::genericParameter));

	return layout;
}

MercyAudioProcessor::~MercyAudioProcessor()
{
}

//==============================================================================
const juce::String MercyAudioProcessor::getName() const
{

	return JucePlugin_Name;
}

bool MercyAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
	return true;
#else
	return false;
#endif
}

bool MercyAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
	return true;
#else
	return false;
#endif
}

bool MercyAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
	return true;
#else
	return false;
#endif
}

double MercyAudioProcessor::getTailLengthSeconds() const
{
	return 0.0;
}

int MercyAudioProcessor::getNumPrograms()
{
	return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
	// so this should be at least 1, even if you're not really implementing programs.
}

int MercyAudioProcessor::getCurrentProgram()
{
	return 0;
}

void MercyAudioProcessor::setCurrentProgram(int index)
{
}

const juce::String MercyAudioProcessor::getProgramName(int index)
{
	return {};
}

void MercyAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
}

//==============================================================================
void MercyAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
	// Use this method as the place to do any pre-playback
	// initialisation that you need..

	juce::dsp::ProcessSpec spec;
	spec.sampleRate = sampleRate;
	spec.maximumBlockSize = samplesPerBlock;
	spec.numChannels = getNumOutputChannels();

	lowPassFilter.prepare(spec);
	lowPassFilter.reset();

	highPassFilter.prepare(spec);
	highPassFilter.reset();
}

void MercyAudioProcessor::releaseResources()
{
	// When playback stops, you can use this as an opportunity to free up any
	// spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MercyAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
	juce::ignoreUnused(layouts);
	return true;
#else
	// This is the place where you check if the layout is supported.
	// In this template code we only support mono or stereo.
	// Some plugin hosts, such as certain GarageBand versions, will only
	// load plugins that support stereo bus layouts.
	if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
		&& layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
		return false;

	// This checks if the input layout matches the output layout
#if ! JucePlugin_IsSynth
	if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
		return false;
#endif

	return true;
#endif
}
#endif

void MercyAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
	juce::ScopedNoDenormals noDenormals;
	auto totalNumInputChannels = getTotalNumInputChannels();
	auto totalNumOutputChannels = getTotalNumOutputChannels();

	auto lpfCutoff = apvts.getRawParameterValue(ParamIDs::lpfCutoff)->load();
	auto lpfReso = apvts.getRawParameterValue(ParamIDs::lpfReso)->load();

	auto hpfCutoff = apvts.getRawParameterValue(ParamIDs::hpfCutoff)->load();
	auto hpfReso = apvts.getRawParameterValue(ParamIDs::hpfReso)->load();

	auto sampleRate = getSampleRate();

	*lowPassFilter.state = *juce::dsp::IIR::Coefficients<float>::makeLowPass(sampleRate, lpfCutoff, lpfReso);
	*highPassFilter.state = *juce::dsp::IIR::Coefficients<float>::makeHighPass(sampleRate, hpfCutoff, hpfReso);

	// In case we have more outputs than inputs, this code clears any output
	// channels that didn't contain input data, (because these aren't
	// guaranteed to be empty - they may contain garbage).
	// This is here to avoid people getting screaming feedback
	// when they first compile a plugin, but obviously you don't need to keep
	// this code if your algorithm always overwrites all the output channels.
	for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
		buffer.clear(i, 0, buffer.getNumSamples());

	juce::dsp::AudioBlock<float> audioBlock(buffer);
	juce::dsp::ProcessContextReplacing<float> ctx(audioBlock);

	highPassFilter.process(ctx);
	lowPassFilter.process(ctx);

}

//==============================================================================
bool MercyAudioProcessor::hasEditor() const
{
	return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* MercyAudioProcessor::createEditor()
{
	return new MercyAudioProcessorEditor(*this);
}

//==============================================================================
void MercyAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
	// You should use this method to store your parameters in the memory block.
	// You could do that either as raw data, or use the XML or ValueTree classes
	// as intermediaries to make it easy to save and load complex data.
}

void MercyAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
	// You should use this method to restore your parameters from this memory block,
	// whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
	return new MercyAudioProcessor();
}
