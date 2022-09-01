#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"
#include <cmath>

//==============================================================================
/*
*/
class SphericalFader : public juce::Slider, juce::Slider::Listener
{


public:
    SphericalFader(SeamLess_ClientAudioProcessor& p, juce::AudioProcessorValueTreeState& apvts, bool endless, juce::Slider::RotaryParameters rotaryParameters, juce::String type);
    ~SphericalFader() override;

    void paint(juce::Graphics&) override;
    void resized() override;

    void setText(juce::String s);

    void sliderDragStarted(juce::Slider* fader) override;
    void sliderDragEnded(juce::Slider*) override;
    void sliderValueChanged(juce::Slider*) override;

    void setSliderRange(juce::Range<double> newRange, double newInterval);
    void setSliderTextValueSuffix(juce::String newSuffix);

    juce::Slider slider;

    bool onDrag = false;

    void connectXtoParameter(juce::RangedAudioParameter& p);
    void connectYtoParameter(juce::RangedAudioParameter& p);
    void connectZtoParameter(juce::RangedAudioParameter& p);

private:
    juce::Label sphericalNameLabel;
    SeamLess_ClientAudioProcessor& audioProcessor;
    juce::AudioProcessorValueTreeState& treeState;
    juce::String sliderType;
    std::unique_ptr<juce::ParameterAttachment> xAttachment;
    std::unique_ptr<juce::ParameterAttachment> yAttachment;
    std::unique_ptr<juce::ParameterAttachment> zAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SphericalFader)
};
