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

    impulse[0] = new float[1];
    impulse[1] = new float[1];
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
    
    impulseLength = reader->lengthInSamples;


    delete[] impulse[0];
    impulse[0] = new float[impulseLength];

    delete[] impulse[1];
    impulse[1] = new float[impulseLength];

    reader->read(impulse, 2, 0, impulseLength);
}
