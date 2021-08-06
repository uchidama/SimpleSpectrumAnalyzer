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
class SimpleSpectrumAnalyzerAudioProcessorEditor  : public juce::AudioProcessorEditor, 
    private juce::Timer
{
public:
    SimpleSpectrumAnalyzerAudioProcessorEditor (SimpleSpectrumAnalyzerAudioProcessor&);
    ~SimpleSpectrumAnalyzerAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void timerCallback() override;
    
    void drawNextFrameOfSpectrum();    
    void drawFrame (juce::Graphics& g);
    
    static constexpr auto scopeSize = 512;
    
private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SimpleSpectrumAnalyzerAudioProcessor& audioProcessor;

    juce::dsp::FFT forwardFFT;
    juce::dsp::WindowingFunction<float> window;
    float scopeData [scopeSize];

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SimpleSpectrumAnalyzerAudioProcessorEditor)
};
