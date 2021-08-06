/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SimpleSpectrumAnalyzerAudioProcessorEditor::SimpleSpectrumAnalyzerAudioProcessorEditor (SimpleSpectrumAnalyzerAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p),
      forwardFFT (p.fftOrder),
      window (p.fftSize, juce::dsp::WindowingFunction<float>::hann)
    
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.    
    setOpaque (true);
    startTimerHz (30);
    setSize (700, 500);
}

SimpleSpectrumAnalyzerAudioProcessorEditor::~SimpleSpectrumAnalyzerAudioProcessorEditor()
{
}

void SimpleSpectrumAnalyzerAudioProcessorEditor::timerCallback()
{
    auto flg = audioProcessor.getFFTReady();
    if (flg)
    {
        drawNextFrameOfSpectrum();
        audioProcessor.setFFTReady(false);
        repaint();
    }
}

void SimpleSpectrumAnalyzerAudioProcessorEditor::drawNextFrameOfSpectrum()
{
    //[3-1]窓関数の指定、引数をアクセサとProcessorのfftSizeで与えます。
    window.multiplyWithWindowingTable (audioProcessor.getFFTDataPtr(), audioProcessor.fftSize);
    //[3-2]周波数データへの変換処理、Processor側のデータfftDataはアクセサで与えます。
    forwardFFT.performFrequencyOnlyForwardTransform (audioProcessor.getFFTDataPtr());

    auto mindB = -100.0f;
    auto maxdB =    0.0f;

    //[3-3]スケーリング処理です。AudioProcessor.fftSizeに、fftDataの中身はアクセサに変更しました。
    for (int i = 0; i < scopeSize; ++i)
    {
        auto skewedProportionX = 1.0f - std::exp (std::log (1.0f - (float) i / (float) scopeSize) * 0.2f);
        auto fftDataIndex = juce::jlimit (0, audioProcessor.fftSize / 2, (int) (skewedProportionX * (float) audioProcessor.fftSize * 0.5f));
        //fftData[fftDataIndex]はaudioProcessor.getFFTDataSample(fftDataIndex)へ変更。
        auto level = juce::jmap (juce::jlimit (mindB, maxdB, juce::Decibels::gainToDecibels (audioProcessor.getFFTDataSample(fftDataIndex))
                                                           - juce::Decibels::gainToDecibels ((float) audioProcessor.fftSize)),
                                 mindB, maxdB, 0.0f, 1.0f);

        scopeData[i] = level;
    }
}

void SimpleSpectrumAnalyzerAudioProcessorEditor::drawFrame (juce::Graphics& g)
{
    for (int i = 1; i < scopeSize; ++i)
    {
        auto width  = getLocalBounds().getWidth();
        auto height = getLocalBounds().getHeight();

        g.drawLine ({ (float) juce::jmap (i - 1, 0, scopeSize - 1, 0, width),
                              juce::jmap (scopeData[i - 1], 0.0f, 1.0f, (float) height, 0.0f),
                      (float) juce::jmap (i,     0, scopeSize - 1, 0, width),
                              juce::jmap (scopeData[i],     0.0f, 1.0f, (float) height, 0.0f) });
    }
}

//==============================================================================
void SimpleSpectrumAnalyzerAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    /*
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
    */
    
    g.fillAll (juce::Colours::black);

    g.setOpacity (1.0f);
    g.setColour (juce::Colours::white);
    drawFrame (g);
}

void SimpleSpectrumAnalyzerAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
