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
                public juce::Button::Listener,
                public juce::ComboBox::Listener

{
public:

    LFOBox(SeamLess_ClientAudioProcessor &p, juce::AudioProcessorValueTreeState &apvts);
    /** @cond PRIVATE */
    ~LFOBox() override;

    void paint (juce::Graphics&) override;
    void resized() override;


    void sliderValueChanged (juce::Slider* slider) override;
    
    void buttonClicked(juce::Button* button) override;

    void comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged) override;
    /** @endcond */
    
    void connectXtoParameter(juce::RangedAudioParameter& p);
    void connectYtoParameter(juce::RangedAudioParameter& p);
    void connectZtoParameter(juce::RangedAudioParameter& p);

    void endLFOs();
    void startLFOs();
   

private:

    SeamLess_ClientAudioProcessor& audioProcessor;
    juce::AudioProcessorValueTreeState& treeState;

    juce::String names[12] = { "rate", "depth", "phase", "offset", "rate", "depth", "phase", "offset", "rate", "depth", "phase", "offset" };
    juce::Range<double> ranges[12] = { {0.0, 10.00}, {0.0, 100.0}, {-180.0 , 180.0}, {-10.0, 10.0}, {0.0, 10.00}, {0.0, 100.0}, {-180.0 , 180.0}, {-10.0, 10.0}, {0.0, 10.00}, {0.0, 100.0}, {-180.0 , 180.0}, {-10.0, 10.0} };
    juce::String suffixes[12] = { " Hz"," %", juce::CharPointer_UTF8("\xc2\xb0"), " m", " Hz"," %", juce::CharPointer_UTF8("\xc2\xb0"), " m", " Hz"," %", juce::CharPointer_UTF8("\xc2\xb0"), " m" };


    LFOSlider xRateSlider;
    LFOSlider xDepthSlider;
    LFOSlider xPhaseSlider;
    LFOSlider xOffsetSlider;

    LFOSlider yRateSlider;
    LFOSlider yDepthSlider;
    LFOSlider yPhaseSlider;
    LFOSlider yOffsetSlider;

    LFOSlider zRateSlider;
    LFOSlider zDepthSlider;
    LFOSlider zPhaseSlider;
    LFOSlider zOffsetSlider;
    
    LFOSlider* sliders[12] = { &xRateSlider, &xDepthSlider, &xPhaseSlider, &xOffsetSlider, &yRateSlider, &yDepthSlider, &yPhaseSlider, &yOffsetSlider, &zRateSlider, &zDepthSlider, &zPhaseSlider, &zOffsetSlider };
    LFOSlider* rateSliders[3] = { &xRateSlider, &yRateSlider, &zRateSlider };
    LFOSlider* depthSliders[3] = { &xDepthSlider, &yDepthSlider, &zDepthSlider };
    LFOSlider* phaseSliders[3] = { &xPhaseSlider, &yPhaseSlider, &zPhaseSlider };
    LFOSlider* offsetSliders[3] = { &xOffsetSlider, &yOffsetSlider, &zOffsetSlider };
    LFOSlider* xSliders[4] = { &xRateSlider, &xDepthSlider, &xPhaseSlider, &xOffsetSlider };
    LFOSlider* ySliders[4] = { &yRateSlider, &yDepthSlider, &yPhaseSlider, &yOffsetSlider };
    LFOSlider* zSliders[4] = { &zRateSlider, &zDepthSlider, &zPhaseSlider, &zOffsetSlider };


    juce::TextButton LFOStartButton;
    juce::ComboBox LFONumberBox;

    
    juce::TextButton* textbutton = &LFOStartButton;
    
    std::unique_ptr<juce::ParameterAttachment> xAttachment;
    std::unique_ptr<juce::ParameterAttachment> yAttachment;
    std::unique_ptr<juce::ParameterAttachment> zAttachment;

   
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LFOBox)
};


