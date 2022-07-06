#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/*
*/
class SphericalFader : public juce::Slider, juce::Slider::Listener
{
public:
    SphericalFader(SeamLess_ClientAudioProcessor& p, juce::AudioProcessorValueTreeState& apvts, bool endless, juce::Slider::RotaryParameters rotaryParameters);
    ~SphericalFader() override;

    void paint(juce::Graphics&) override;
    void resized() override;

    void setText(juce::String s);

    void sliderDragStarted(juce::Slider* fader) override;
    void sliderDragEnded(juce::Slider*) override;
    void sliderValueChanged(juce::Slider*) override;

    void setSliderRange(juce::Range<double> newRange, double newInterval);
    void setSliderTextValueSuffix(juce::String newSuffix);

private:

    juce::Label sphericalNameLabel;

    juce::Slider slider;

    double currentValue;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SphericalFader)
};
