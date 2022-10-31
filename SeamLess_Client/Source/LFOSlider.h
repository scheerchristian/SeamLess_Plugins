/*
  ==============================================================================

    LFOSlider.h
    Created: 12 Sep 2022 12:44:01pm
    Author:  Michael Witte

  ==============================================================================
*/

#pragma once
#include "PluginProcessor.h"

class LFOSlider : public juce::Slider, juce::Slider::Listener
{
public:
    LFOSlider(SeamLess_ClientAudioProcessor& p, juce::AudioProcessorValueTreeState& apvts, bool endless, juce::Slider::RotaryParameters rotaryParameters, juce::String id);
    /** @cond PRIVATE */
    ~LFOSlider() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    /** @endcond */
    
    void setText(juce::String s);

    void sliderDragStarted(juce::Slider* fader) override;
    void sliderDragEnded(juce::Slider*) override;
    void sliderValueChanged(juce::Slider*) override;

    void setSliderRange(juce::Range<double> newRange, double newInterval);
    void setSliderTextValueSuffix(juce::String newSuffix);
    void setSliderSkewFactor(double newSkewFactor);


private:

    juce::Label lfoNameLabel;

    juce::Slider slider;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lfoSliderAttachment;

    juce::String parameterID;

    juce::AudioProcessorValueTreeState* treeState;

    SeamLess_ClientAudioProcessor* audioProcessor;

    double currentValue;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LFOSlider)
};


