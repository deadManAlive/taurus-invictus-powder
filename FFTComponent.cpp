#include "FFTComponent.h"
#include "PluginProcessor.h"
#include "melatonin_perfetto/melatonin_perfetto.h"
#include <cmath>
#include <vector>

//==============================================================================
static std::vector<float> frequenciesGenerator(float input) {
    // Define the pattern
    std::vector<float> pattern = {20, 50, 100};

    // Generate the vector based on the pattern
    std::vector<float> result;
    for (float value : pattern) {
        while (value < input) {
            result.push_back(value);
            value *= 10;
        }
    }

    // Use std::sort to sort the vector in ascending order
    std::sort(result.begin(), result.end());

    return result;
}

static std::vector<float> gainGenerator() {
    static_assert((int)minDB % 12 == 0);
    std::vector<float> res;

    for (float i = minDB; i <= -12.f; i /= 2.f) {
        res.push_back((float)i);
    }
    res.push_back(0.f);

    return res;
}
//==============================================================================
FFTSpectrum::FFTSpectrum(AudioPluginAudioProcessor& p, AudioProcessorValueTreeState& apvts)
    : processorRef(p)
    , parameters(apvts)
    , scopeData{}
{   
    startTimerHz(60);
}

FFTSpectrum::~FFTSpectrum() {}

void FFTSpectrum::paint(juce::Graphics& g) {
    TRACE_COMPONENT();

    g.fillAll (Colours::black);

    drawBackgroundGrid(g);
    drawTextLabels(g);

    g.setOpacity(1.0f);
    g.setColour(Colours::whitesmoke);

    auto width = getLocalBounds().getWidth();
    auto height = getLocalBounds().getHeight();

    for (int i = 1; i < (scopeSize - 1); ++i) {
        g.drawLine({
            (float) jmap(i, 0, scopeSize - 1, 0, width),
            (float) height,
            (float) jmap(i, 0, scopeSize - 1, 0, width),
            jmap (scopeData[(size_t) i], 0.0f, 1.0f, (float) height, 0.0f)
        });
    }
}

void FFTSpectrum::resized() {}

void FFTSpectrum::timerCallback() {
    TRACE_COMPONENT();

    if (processorRef.getBufferFreeSpace() == 0) {
        processorRef.processFftData();

        const auto analyzerMaxDB = 1.f;
        const auto levelFactor = 1.f / (pow(10.f, analyzerMaxDB / 20.f));

        auto skewValue = (parameters.getRawParameterValue("skew"));


        // TODO: this is wrong (for this new graphing base).
        for (int i = 0; i < scopeSize; ++i) {
            auto skewedProportionX = 1.f - std::exp(std::log(1.f - (float) i / (float) scopeSize) * (*skewValue));
            auto fftDataIndex = juce::jlimit(0, (int)fftSize / 2, (int)(skewedProportionX * (float) fftSize * 0.5f));
            auto level = juce::jmap(
                juce::jlimit(
                    minDB, maxDB,
                    juce::Decibels::gainToDecibels(processorRef.getFftData(fftDataIndex)) - juce::Decibels::gainToDecibels((float) fftSize)
                ),
                minDB, maxDB, 0.f, levelFactor
            );

            scopeData[(size_t) i] = level;
        }

        repaint();
    }
}

juce::Rectangle<int> FFTSpectrum::getRenderArea() {
    auto bounds = getLocalBounds();

    bounds.removeFromTop(12);
    bounds.removeFromBottom(2);
    bounds.removeFromLeft(20);
    bounds.removeFromRight(20);

    return bounds;
}

juce::Rectangle<int> FFTSpectrum::getAnalysisArea() {
    auto bounds = getRenderArea();

    bounds.removeFromTop(4);
    bounds.removeFromBottom(4);

    return bounds;
}

std::vector<float> FFTSpectrum::getXs(const std::vector<float> &freqs, float left, float width)
{
    std::vector<float> xs;

    for( auto f : freqs )
    {
        auto normX = juce::mapFromLog10(f, 20.f, 20000.f);
        xs.push_back( left + width * normX );
    }
    
    return xs;
}

void FFTSpectrum::drawBackgroundGrid(juce::Graphics& g) {
    const auto nyquist_frequency = (float)processorRef.getSampleRate() / 2.f;
    const auto frequencies = frequenciesGenerator(nyquist_frequency);

    const auto renderArea = getRenderArea();
    const auto left = (float)renderArea.getX();
    const auto right = (float)renderArea.getRight();
    const auto top = (float)renderArea.getY();
    const auto bottom = (float)renderArea.getBottom();
    const auto width = (float)renderArea.getWidth();

    const auto xs = getXs(frequencies, left, width);

    g.setColour(Colours::dimgrey);

    for (auto x : xs) {
        g.drawVerticalLine((int)x, top, bottom);
    }

    auto gain = gainGenerator();
    
    g.setColour(Colours::darkgrey);
    for( auto gDb : gain )
    {
        auto y = jmap(gDb, minDB, maxDB, float(bottom), float(top));
        
        g.drawHorizontalLine((int)y, left, right);
    }
}

void FFTSpectrum::drawTextLabels(juce::Graphics &g)
{
    g.setColour(Colours::lightgrey);
    const int fontHeight = 10;
    g.setFont(fontHeight);
    
    auto renderArea = getAnalysisArea();
    auto left = (float)renderArea.getX();
    auto top = (float)renderArea.getY();
    auto bottom = (float)renderArea.getBottom();
    auto width = (float)renderArea.getWidth();
    
    const auto nyquist_frequency = (float)processorRef.getSampleRate() / 2.f;
    const auto freqs = frequenciesGenerator(nyquist_frequency);

    auto xs = getXs(freqs, left, width);
    
    for( size_t i = 0; i < freqs.size(); ++i )
    {
        auto f = freqs[i];
        auto x = xs[i];

        bool addK = false;
        String str;
        if( f > 999.f )
        {
            addK = true;
            f /= 1000.f;
        }

        str << f;
        if( addK )
            str << "k";
        str << "Hz";
        
        auto textWidth = g.getCurrentFont().getStringWidth(str);

        Rectangle<int> r;

        r.setSize(textWidth, fontHeight);
        r.setCentre((int)x, 0);
        r.setY(1);
        
        g.drawFittedText(str, r, juce::Justification::centred, 1);
    }
    
    auto gain = gainGenerator();

    for( auto gDb : gain )
    {
        auto y = jmap(gDb, minDB, maxDB, float(bottom), float(top));
        
        String str;
        if( gDb > 0 )
            str << "+";
        str << gDb;
        
        auto textWidth = g.getCurrentFont().getStringWidth(str);
        
        Rectangle<int> r;
        r.setSize(textWidth, fontHeight);
        r.setX(1);
        r.setCentre(r.getCentreX(), (int)y);
        
        g.setColour(Colours::lightgrey);
        
        g.drawFittedText(str, r, juce::Justification::centredLeft, 1);
    }
}