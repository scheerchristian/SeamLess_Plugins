/*
  ==============================================================================

    LFOBox.h
    Created: 12 Sep 2022 12:42:28pm
    Author:  Michael Witte

  ==============================================================================
*/

#pragma once
#define _USE_MATH_DEFINES
#include <JuceHeader.h>
#include "../../Common/SeamLess.h"
#include "PluginProcessor.h"
#include "LFOSlider.h"
#include <math.h>

class LFOBox  : public juce::Component, 
                public juce::Slider::Listener, 
                public juce::Button::Listener

{
public:

    LFOBox(SeamLess_ClientAudioProcessor &p, juce::AudioProcessorValueTreeState &apvts);
    ~LFOBox() override;

    void paint (juce::Graphics&) override;
    void resized() override;


    void sliderValueChanged (juce::Slider* slider) override;
    
    void buttonClicked(juce::Button* button) override;
    
    void connectXtoParameter(juce::RangedAudioParameter& p);
    void connectYtoParameter(juce::RangedAudioParameter& p);



private:

    SeamLess_ClientAudioProcessor& audioProcessor;
    juce::AudioProcessorValueTreeState& treeState;

    LFOSlider rateSlider;
    LFOSlider depthSlider;
    LFOSlider phaseSlider;
    LFOSlider offsetXSlider;
    LFOSlider offsetYSlider;

    
    juce::TextButton LFOStartButton;

    juce::String names[5] = { "rate", "depth", "phase", "offset X", "offset Y"};
    LFOSlider* slider[5] = { &rateSlider, &depthSlider, &phaseSlider, &offsetXSlider, &offsetYSlider };
    juce::Range<double> ranges[5] = { {0.0, 2.00}, {0.0, 100.0}, {-180.0 , 180.0}, {-10.0, 10.0}, {-10.0, 10.0} };
    juce::String suffixes[5] = { " Hz"," %", juce::CharPointer_UTF8("\xc2\xb0"), " m", " m"};
    
    juce::TextButton* textbutton = &LFOStartButton;
    
    std::unique_ptr<juce::ParameterAttachment> xAttachment;
    std::unique_ptr<juce::ParameterAttachment> yAttachment;
    
   
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LFOBox)
};


