/*
  ==============================================================================

    SendFader.h
    Created: 31 Mar 2021 10:34:35am
    Author:  anwaldt

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/*
*/
class ReverbFader  : public juce::Slider
{
public:
    ReverbFader(SeamLess_MainAudioProcessor &p, juce::AudioProcessorValueTreeState& apvts, juce::String s);
    /** @cond PRIVATE */
    ~ReverbFader() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    /** @endcond */

    /**
     Sets a  string as the faders name label.
     @param s string for name label
     */
    void setText(juce::String s);

private:

    juce::Label sendNameLabel;

    juce::Slider slider;
    juce::AudioProcessorValueTreeState::SliderAttachment sliderAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ReverbFader)
};
