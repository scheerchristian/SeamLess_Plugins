/*
  ==============================================================================

    LFOBox.h
    Created: 12 Sep 2022 12:42:28pm
    Author:  Michael Witte

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "../../Common/SeamLess.h"
#include "PluginProcessor.h"
#include "LFOSlider.h"

class LFOBox  : public juce::Component, juce::Slider::Listener, juce::Button::Listener
{
public:

    LFOBox(SeamLess_ClientAudioProcessor &p, juce::AudioProcessorValueTreeState &apvts);
    ~LFOBox() override;

    void paint (juce::Graphics&) override;
    void resized() override;


    void sliderValueChanged (juce::Slider* slider) override;
    
    void buttonClicked(juce::Button* button) override;
    

private:

    SeamLess_ClientAudioProcessor& audioProcessor;
    juce::AudioProcessorValueTreeState& treeState;

    LFOSlider rateSlider;
    LFOSlider depthSlider;
    LFOSlider phaseSlider;
    LFOSlider offsetSlider;
    
    juce::TextButton LFOStartButton;

    juce::String names[4] = { "rate", "depth", "phase", "offset" };
    LFOSlider* slider[4] = { &rateSlider, &depthSlider, &phaseSlider, &offsetSlider };
    juce::Range<double> ranges[4] = { {0.0, 10.00}, {0.0, 100.0}, {-180.0 , 180.0}, {-10.0, 10.0} };
    juce::String suffixes[4] = { " Hz"," %", juce::CharPointer_UTF8("\xc2\xb0"), " m" };
    
    juce::TextButton* textbutton = &LFOStartButton;
    

    
   
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LFOBox)
};


