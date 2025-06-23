/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

using juce::dsp::Convolution;

//==============================================================================
ImpulseResponderAudioProcessor::ImpulseResponderAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    impulse[0] = new float[1] {0.f};
    impulse[1] = new float[1] {0.f};

    samples[0] = new float[1] {0.f};
    samples[1] = new float[1] {0.f};
}

ImpulseResponderAudioProcessor::~ImpulseResponderAudioProcessor()
{
}

//==============================================================================
const juce::String ImpulseResponderAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ImpulseResponderAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ImpulseResponderAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ImpulseResponderAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ImpulseResponderAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ImpulseResponderAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int ImpulseResponderAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ImpulseResponderAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String ImpulseResponderAudioProcessor::getProgramName (int index)
{
    return {};
}

void ImpulseResponderAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void ImpulseResponderAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    //samplesPerBuffer = samplesPerBlock;
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void ImpulseResponderAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ImpulseResponderAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
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

void ImpulseResponderAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    float* const* newSamples = buffer.getArrayOfWritePointers();

    
    //if (convolution.getCurrentIRSize() > 0) {
    //    convolution.process(buffer);
    //}

    
    for (int sample = 0; sample < buffer.getNumSamples(); sample++) {
        for (int channel = 0; channel < 2; channel++)
        {
            float sum = 0;
            for (int impulseSample = 0; impulseSample < impulseLength; impulseSample++) {
                sum += impulse[channel][impulseSample] * samples[channel][(impulseSample + counter) % impulseLength];
            }

            samples[channel][counter] = newSamples[channel][sample];
            newSamples[channel][sample] = sum;
        }

        counter = (counter + 1) % impulseLength;
    }
}

//==============================================================================
bool ImpulseResponderAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* ImpulseResponderAudioProcessor::createEditor()
{
    return new ImpulseResponderAudioProcessorEditor (*this);
}

//==============================================================================
void ImpulseResponderAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void ImpulseResponderAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

/*void ImpulseResponderAudioProcessor::initConvolution(juce::File& file)
{
    
    convolution.loadImpulseResponse(file, Convolution::Stereo::yes, Convolution::Trim::yes, 0, Convolution::Normalise::yes);
    juce::dsp::ProcessSpec processSpec;
    processSpec.sampleRate = getSampleRate();
    processSpec.numChannels = 2;
    processSpec.maximumBlockSize = samplesPerBuffer;
    convolution.prepare(processSpec);
}*/

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ImpulseResponderAudioProcessor();
}
