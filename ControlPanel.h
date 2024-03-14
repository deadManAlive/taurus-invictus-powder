#pragma once

#include <JuceHeader.h>
#include <memory>

#include "PluginProcessor.h"

class ControlPanel : public juce::Component
{
public:
    ControlPanel(AudioPluginAudioProcessor& p, AudioProcessorValueTreeState& apvts);
    ~ControlPanel() override;
    void paint(juce::Graphics& g) override;
    void resized() override;
private:
    using SliderAttachment = AudioProcessorValueTreeState::SliderAttachment;

    AudioPluginAudioProcessor& processorRef;
    AudioProcessorValueTreeState& parameters;

    Slider skewSlider;
    std::unique_ptr<SliderAttachment> skewAttachment;

    Slider smoothingSlider;
    std::unique_ptr<SliderAttachment> smoothingAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ControlPanel)
};