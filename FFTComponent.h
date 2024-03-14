#pragma once

#include <JuceHeader.h>
#include <vector>

#include "PluginProcessor.h"

static constexpr float minDB = -96;
static constexpr float maxDB = 0;

class FFTSpectrum : public juce::Component, public juce::Timer
{
public:
    //==============================================================================
    FFTSpectrum(AudioPluginAudioProcessor& p, AudioProcessorValueTreeState& apvts);
    ~FFTSpectrum() override;
    void paint(juce::Graphics& g) override;
    void resized() override;
    void timerCallback() override;
private:
    //==============================================================================
    AudioPluginAudioProcessor& processorRef;

    //==============================================================================
    static constexpr auto fftOrder = AudioPluginAudioProcessor::fftOrder;
    static constexpr auto fftSize = AudioPluginAudioProcessor::fftSize;
    static constexpr auto scopeSize = AudioPluginAudioProcessor::scopeSize;

    //==============================================================================
    AudioProcessorValueTreeState& parameters;

    //==============================================================================
    std::array<float, scopeSize> scopeData;

    //==============================================================================
    juce::Rectangle<int> getRenderArea();
    juce::Rectangle<int> getAnalysisArea(); 
    std::vector<float> getXs(const std::vector<float>&, float, float);

    //==============================================================================
    void drawBackgroundGrid(juce::Graphics& g);
    void drawTextLabels(juce::Graphics& g);

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FFTSpectrum)
};