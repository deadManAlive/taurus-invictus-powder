#pragma once

#include <JuceHeader.h>
#include <array>

#include <LockFreeBuffer.h>

//==============================================================================
class AudioPluginAudioProcessor final : public juce::AudioProcessor
{
public:
    //==============================================================================
    AudioPluginAudioProcessor();
    ~AudioPluginAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    using AudioProcessor::processBlock;

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
    static constexpr int fftOrder = 11;
    static constexpr int fftSize = 1 << fftOrder;
    static constexpr int scopeSize = 1 << 6;

    //==============================================================================
    void processFftData();
    float getFftData(int index) const;

    //==============================================================================
    int getBufferFreeSpace() const;

    //==============================================================================
    AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
private:
    //==============================================================================
    std::atomic<float>* skewValue = nullptr;
    std::atomic<float>* smoothingConstantValue = nullptr;
    
    //==============================================================================
    juce::dsp::FFT forwardFFT;
    juce::dsp::WindowingFunction<float> window;
    taurus::LockFreeBufferFixed<float, 4 * fftSize> lockFreeBuffer;

    //==============================================================================
    using FftContainerType = std::array<float, 2 * fftSize>;
    FftContainerType fftData;
    FftContainerType smoothedBlock;
    juce::AudioBuffer<float> sumBuffer;

    //==============================================================================
    juce::AudioProcessorValueTreeState apvts;

    //==============================================================================
    #if PERFETTO
        std::unique_ptr<perfetto::TracingSession> tracingSession;
    #endif

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginAudioProcessor)
};
