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
#include "SendFader.h"
#include "SphericalFader.h"
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

    SphericalFader rSlider;
    SphericalFader azimutSlider;
    SphericalFader elevationSlider;

    juce::String names[3] = { "radius", "azimuth", "elevation" };
    SphericalFader* faders[3] = { &rSlider, &azimutSlider, &elevationSlider };
    juce::Range<double> ranges[3] = { {0.0, 14.14}, {0.0, 360.0}, {0.0, 360.0} };
    juce::String suffixes[3] = { " m",juce::CharPointer_UTF8("\xc2\xb0"), juce::CharPointer_UTF8("\xc2\xb0") };

    //juce::AudioProcessorValueTreeState::SliderAttachment rSliderAttachment;
   
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SphericalBox)
};
