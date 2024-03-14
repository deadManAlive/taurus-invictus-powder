#pragma once

#include "PluginProcessor.h"

#include "FFTComponent.h"
#include "ControlPanel.h"

//==============================================================================
class AudioPluginAudioProcessorEditor final : public AudioProcessorEditor
{
public:
    explicit AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor&, AudioProcessorValueTreeState&);
    ~AudioPluginAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AudioPluginAudioProcessor& processorRef;

    FFTSpectrum fftSpectrum;
    ControlPanel controlPanel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginAudioProcessorEditor)
};
