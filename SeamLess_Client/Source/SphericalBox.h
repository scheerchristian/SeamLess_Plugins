/*
  ==============================================================================

    SphericalBox.h
    Created: 19 Jun 2022 11:13:31pm
    Author:  Uni

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "../../Common/SeamLess.h"
#include "PluginProcessor.h"

class SphericalBox  : public juce::Component, juce::Slider::Listener
{
public:

    SphericalBox(SeamLess_ClientAudioProcessor &p, juce::AudioProcessorValueTreeState &apvts);
    ~SphericalBox() override;

    void paint (juce::Graphics&) override;
    void resized() override;


    void sliderValueChanged (juce::Slider* slider) override;

private:

    SeamLess_ClientAudioProcessor& audioProcessor;
    juce::AudioProcessorValueTreeState& treeState;

    juce::Slider rSlider;
    juce::Label rSliderLabel;
    juce::AudioProcessorValueTreeState::SliderAttachment rSliderAttachment;
    



    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SphericalBox)
};
