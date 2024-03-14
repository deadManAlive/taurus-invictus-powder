#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor& p, AudioProcessorValueTreeState& apvts)
    : AudioProcessorEditor (&p)
    , processorRef (p)
    , fftSpectrum (p, apvts)
    , controlPanel (p, apvts)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    addAndMakeVisible(fftSpectrum);
    addAndMakeVisible(controlPanel);

    double ratio = 32.0/9.0;
    int min_height = 400;
    int max_height = 1600;
    int default_size = 600;
    setResizeLimits(min_height, (int)(min_height/ratio), max_height, (int)(max_height/ratio));
    getConstrainer()->setFixedAspectRatio(ratio);
    setSize(default_size, (int)(default_size/ratio));
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
}

//==============================================================================
void AudioPluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (Colours::black);

}

void AudioPluginAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    Grid grid;
    
    using Track = Grid::TrackInfo;
    using Fr = Grid::Fr;
    using Px = Grid::Px;

    grid.setGap(Px{8});

    grid.templateRows = {Track(Fr(1))};
    grid.templateColumns = {Track(Fr(1)), Track(Fr(3))};

    grid.items = {
        GridItem (controlPanel),
        GridItem (fftSpectrum)
    };

    grid.performLayout (getLocalBounds());
}
