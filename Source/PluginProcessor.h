/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class SimpleSpectrumAnalyzerAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    SimpleSpectrumAnalyzerAudioProcessor();
    ~SimpleSpectrumAnalyzerAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

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
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    //==============================================================================
    void pushNextSampleIntoFifo (float sample) noexcept;
    
    static constexpr auto fftOrder  = 11;
    static constexpr auto fftSize   = 1 << fftOrder;
    
    bool getFFTReady(){
	    return nextFFTBlockReady;
	}
	
	void setFFTReady(bool b) {
	    nextFFTBlockReady = b;
	}
	
	float* getFFTDataPtr() {
	    return fftData.data();
	}
	
	float getFFTDataSample(int sample) {
	    return fftData[sample];
	}
	
private:

    std::array<float, fftSize> fifo;
    std::array<float, fftSize * 2> fftData;
    int fifoIndex = 0;
    bool nextFFTBlockReady = false;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SimpleSpectrumAnalyzerAudioProcessor)
};
