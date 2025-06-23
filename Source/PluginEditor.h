/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class ImpulseResponderAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::FilenameComponentListener
{
public:
    ImpulseResponderAudioProcessorEditor (ImpulseResponderAudioProcessor&);
    ~ImpulseResponderAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void filenameComponentChanged(juce::FilenameComponent* fileComponentThatHasChanged);

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    ImpulseResponderAudioProcessor& audioProcessor;
    juce::AudioFormatManager formatManager;

    juce::FilenameComponent fileSelector = juce::FilenameComponent("", juce::File(), true, false, false, "*.wav", {}, "Select Impulse");

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ImpulseResponderAudioProcessorEditor)
};
