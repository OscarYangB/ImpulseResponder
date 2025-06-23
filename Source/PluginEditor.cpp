/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ImpulseResponderAudioProcessorEditor::ImpulseResponderAudioProcessorEditor (ImpulseResponderAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);

    addAndMakeVisible(fileSelector);
    fileSelector.addListener(this);

    formatManager.registerBasicFormats();
}

ImpulseResponderAudioProcessorEditor::~ImpulseResponderAudioProcessorEditor()
{
}

//==============================================================================
void ImpulseResponderAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);

    fileSelector.setBounds(0, 0, 100, 50);
}

void ImpulseResponderAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

void ImpulseResponderAudioProcessorEditor::filenameComponentChanged(juce::FilenameComponent* fileComponentThatHasChanged)
{
    juce::File file = fileComponentThatHasChanged->getCurrentFile();
    juce::AudioFormatReader* reader = formatManager.createReaderFor(file);
    
    
    ImpulseResponderAudioProcessor* impulseProcessor = (ImpulseResponderAudioProcessor*)getAudioProcessor();
    
    impulseProcessor->impulseLength = reader->lengthInSamples;


    delete[] impulseProcessor->impulse[0];
    impulseProcessor->impulse[0] = new float[impulseProcessor->impulseLength] {0.f};
    delete[] impulseProcessor->samples[0];
    impulseProcessor->samples[0] = new float[impulseProcessor->impulseLength] {0.f};

    delete[] impulseProcessor->impulse[1];
    impulseProcessor->impulse[1] = new float[impulseProcessor->impulseLength] {0.f};
    delete impulseProcessor->samples[1];
    impulseProcessor->samples[1] = new float[impulseProcessor->impulseLength] {0.f};

    reader->read(impulseProcessor->impulse, 2, 0, impulseProcessor->impulseLength);
}
