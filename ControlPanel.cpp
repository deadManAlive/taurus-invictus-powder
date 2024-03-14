#include "ControlPanel.h"

ControlPanel::ControlPanel(AudioPluginAudioProcessor& p, AudioProcessorValueTreeState& apvts)
    : processorRef(p)
    , parameters(apvts)
    , skewSlider(Slider::LinearHorizontal, Slider::TextBoxBelow)
    , smoothingSlider(Slider::LinearHorizontal, Slider::TextBoxBelow)
{
    addAndMakeVisible(skewSlider);
    skewAttachment.reset(new SliderAttachment(parameters, "skew", skewSlider));

    addAndMakeVisible(smoothingSlider);
    smoothingAttachment.reset(new SliderAttachment(parameters, "smoothing", smoothingSlider));
}

ControlPanel::~ControlPanel() {}

void ControlPanel::paint(juce::Graphics& g) {
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}

void ControlPanel::resized() {
    juce::Grid grid;

    using Track = juce::Grid::TrackInfo;
    using Fr = juce::Grid::Fr;

    grid.templateRows = {Track(Fr(1)), Track(Fr(1))};
    grid.templateColumns = {Track(Fr(1))};

    grid.items = {
        juce::GridItem(skewSlider),
        juce::GridItem(smoothingSlider)
    };

    grid.performLayout(getLocalBounds());
}