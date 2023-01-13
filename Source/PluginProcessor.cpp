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

	const auto resoRange = juce::NormalisableRange<float>{ 0.1f, 10.0f, 0.1f };
	const float lpfResoDefaultValue = resoRange.getRange().getStart();
	const float hpfResoDefaultValue = resoRange.getRange().getStart();

	const auto gainRange = juce::NormalisableRange<float>{ -6 * 8.f, +6 * 4.f, 0.1f };

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

	layout.add(std::make_unique<juce::AudioParameterFloat>(
		juce::ParameterID{ ParamIDs::gain,1 },
		ParamIDs::gain,
		gainRange,
		-6.f,
		juce::String(),
		juce::AudioProcessorParameter::genericParameter));

	layout.add(std::make_unique<juce::AudioParameterBool>(
		juce::ParameterID{ ParamIDs::bypass,1 },
		ParamIDs::bypass,
		false));

	return layout;
}

float MercyAudioProcessor::getRMSLevel(const int channel) const
{
	jassert(channel == 0 || channel == 1);

	if (channel == 0) {
        return rmsLevelLeft;
	}
	if (channel == 1) {
        return rmsLevelRight;
	}
	return 0.f;
}

float MercyAudioProcessor::getSmoothedRMSLevel(const int channel) const
{
    jassert(channel == 0 || channel == 1);
    
    
    if(channel == 0){
        return smoothedLevelLeft.getCurrentValue();
    }
    
    if(channel == 1){
        return smoothedLevelRight.getCurrentValue();
    }
    
    return 0.f;
    
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
	spec.numChannels = getTotalNumOutputChannels();

	lowPassFilter.prepare(spec);
	lowPassFilter.reset();

	highPassFilter.prepare(spec);
	highPassFilter.reset();

	auto smoothingTime = 0.3f;
	auto defaultValue = 0.f;
	smoothedLevelLeft.reset(sampleRate, smoothingTime);
	smoothedLevelRight.reset(sampleRate, smoothingTime);
	smoothedLevelLeft.setCurrentAndTargetValue(defaultValue);
	smoothedLevelRight.setCurrentAndTargetValue(defaultValue);
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
/**
 processBlock is the most important method in any JUCE plugin. This is where all the DSP is done. This handles both midi and audio input. The parameters buffer and midiMessage can be manipulated
 in accordance to what you want the output of your plugin to be. However keep in mind this method is on a high priority thread and it's important that the code within the method must follow some strict rules. Failure
 to abide by these rules will lead to glitchy audio and possibly slowing down the user's computer.
 The rules are:
 * 1) Keep the code short. Like code in any function you want to be efficient, however this method especially that is the case.
 * 2) Do not call async functions. processBlock has to be predictable. It's okay to call a method which might take some time as long as we know how much time exactly. You don't want to stop this function from running.
 * 3) Try to minimise the amount of object constructions within this method. Preferably you want object declared outside of the method and have their attributes changed from within. An example below is the smooth values.
 * 4) When calling methods such as getNumSamples() it's important to store the value in a constant and use that constant through the method. This is because such methods can change their return value over the course of the processBlock's runtime. It keeps things simple and prevents issues.
 */
void MercyAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
	juce::ScopedNoDenormals noDenormals;
    
    const auto numSamples = buffer.getNumSamples();
    
    /**
     Skip the next numSamples samples.
     This is identical to calling getNextValue numSamples times. It returns the new current value.
     */
    lowPassCutoff.skip(numSamples);
    lowPassReso.skip(numSamples);
    highPassCutoff.skip(numSamples);
    highPassReso.skip(numSamples);
    gain.skip(numSamples);
    smoothedLevelLeft.skip(numSamples);
    smoothedLevelRight.skip(numSamples);
    
	const auto totalNumInputChannels = getTotalNumInputChannels();
	const auto totalNumOutputChannels = getTotalNumOutputChannels();

    //get raw parameters
    const auto lpfCutoff = apvts.getRawParameterValue(ParamIDs::lpfCutoff)->load();
    const auto lpfReso = apvts.getRawParameterValue(ParamIDs::lpfReso)->load();
    const auto hpfCutoff = apvts.getRawParameterValue(ParamIDs::hpfCutoff)->load();
    const auto hpfReso = apvts.getRawParameterValue(ParamIDs::hpfReso)->load();
    const auto g = apvts.getRawParameterValue(ParamIDs::gain)->load();
    const auto bypass = apvts.getRawParameterValue(ParamIDs::bypass)->load();
    
    //smooth values
    lowPassCutoff.setTargetValue(lpfCutoff);
    lowPassReso.setTargetValue(lpfReso);
    highPassCutoff.setTargetValue(hpfCutoff);
    highPassReso.setTargetValue(hpfReso);
    gain.setTargetValue(g);

    const auto sampleRate = getSampleRate();
	*lowPassFilter.state = *juce::dsp::IIR::Coefficients<float>::makeLowPass(sampleRate, lowPassCutoff.getCurrentValue(), lowPassReso.getCurrentValue());
	*highPassFilter.state = *juce::dsp::IIR::Coefficients<float>::makeHighPass(sampleRate, highPassCutoff.getCurrentValue(), highPassReso.getCurrentValue());

	// In case we have more outputs than inputs, this code clears any output
	// channels that didn't contain input data, (because these aren't
	// guaranteed to be empty - they may contain garbage).
	// This is here to avoid people getting screaming feedback
	// when they first compile a plugin, but obviously you don't need to keep
	// this code if your algorithm always overwrites all the output channels.
	for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
		buffer.clear(i, 0, buffer.getNumSamples());

    //If bypass is true then none of the DSP code will run and thus buffer will remain unchanged
	if (!bypass) {
        //AudioBlock class helps to manipulate the buffer object
        /**
         It is a minimal and lightweight data-structure which contains a list of pointers to channels containing some kind of sample data.
         This class doesn't own any of the data which it points to, it's simply a view into data that is owned elsewhere. You can construct one from some raw data that you've allocated yourself, or give it a HeapBlock to use, or give it an AudioBuffer which it can refer to, but in all cases the user is responsible for making sure that the data doesn't get deleted while there's still an AudioBlock using it.          */
		juce::dsp::AudioBlock<float> audioBlock(buffer);
        //ProcessContextReplacing class helps to get information to be passed into a dsp module's process method
		juce::dsp::ProcessContextReplacing<float> ctx(audioBlock);

        //applying process to the ctx object makes the dsp module take effect
		highPassFilter.process(ctx);
		lowPassFilter.process(ctx);

        /**
         This is a very common for loop to find in this method. Here we could have easily used the applyGain method to the buffer, but here we see what is happening under the hood.
         We are iterating through the buffer using a nested for loop. The buffer is a 2D array where the first dimension represents the channel(i.e left or right speaker) of the audio and the second dimension
         represents the actual audio. This concept of audio being represented as numbers may seem confusing so for simplicity see it this way. Each number is an instruction to the speakers of the device to vibrate.
        These numbers are changed usually around 44100 times a second so when a speaker is being given instructions at this frequency the sound is created. This is an oversimplified explanation but it will suffice.
         Just keep in mind that the larger the number the louder the sound.
         */
		for (auto channel = 0; channel < buffer.getNumChannels(); channel++) {
			for (auto sample = 0; sample < buffer.getNumSamples(); sample++) {
                /**
                 To increase a signal by x decibels it's not correct to multiply the buffer value by x, but rather we need to covert x to a gain value. JUCE provides a decibelsToGain value, which helps because
                 humans are much better at interpreting a decibel value than a gain value. For shorthand +0dB is equivalent to multiplying by 1 in terms of gain. And +6dB is equivalent to 1.99526231(which is basiacally 2).
                 A gain multiplication of 0 will mute the audio.
                 */
				auto newSampleValue = buffer.getSample(channel, sample) * juce::Decibels::decibelsToGain(gain.getCurrentValue(), -6 * 8.f);
				buffer.setSample(channel, sample, newSampleValue);
			}
		}
	}

        const auto leftChannelIndex = 0;
        const auto rightChannelIndex = 1;
		auto startSample = 0;

    //Values used to display the decibel meters. Notice now we use the gainToDecibels value and not the decibelsToGain value.
    rmsLevelLeft = juce::Decibels::gainToDecibels(buffer.getRMSLevel(leftChannelIndex, startSample, numSamples));
    rmsLevelRight = juce::Decibels::gainToDecibels(buffer.getRMSLevel(rightChannelIndex, startSample, numSamples));
    
    if(rmsLevelLeft > smoothedLevelLeft.getCurrentValue()){
        smoothedLevelLeft.setCurrentAndTargetValue(rmsLevelLeft);
    }else{
        smoothedLevelLeft.setTargetValue(rmsLevelLeft);
    }
    
    if(rmsLevelRight > smoothedLevelRight.getCurrentValue()){
        smoothedLevelRight.setCurrentAndTargetValue(rmsLevelRight);
    }else{
        smoothedLevelRight.setTargetValue(rmsLevelRight);
    }

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
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}


void MercyAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
	// You should use this method to restore your parameters from this memory block,
	// whose contents will have been created by the getStateInformation() call.
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName (apvts.state.getType()))
            apvts.replaceState (juce::ValueTree::fromXml (*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
	return new MercyAudioProcessor();
}
