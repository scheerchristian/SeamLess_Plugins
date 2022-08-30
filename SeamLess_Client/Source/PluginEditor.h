/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "TopView.h"
#include "OutgoingConnectionComponent.h"
#include "SendFaderBox.h"
#include "SourceSettingsComponent.h"
#include "SphericalBox.h"


//==============================================================================

/**
*/
class SeamLess_ClientAudioProcessorEditor  : public juce::AudioProcessorEditor,
        public juce::Slider::Listener
{
public:
    SeamLess_ClientAudioProcessorEditor (SeamLess_ClientAudioProcessor&, juce::AudioProcessorValueTreeState&);
    ~SeamLess_ClientAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    //==============================================================================
    void connectZToParameter(juce::RangedAudioParameter& p);

    void sliderValueChanged(juce::Slider* slider) override;

private:

    SeamLess_ClientAudioProcessor& audioProcessor;
    juce::AudioProcessorValueTreeState& treeState;

    SendFaderBox sendBox;
    
    SphericalBox sphericalBox;

    OutgoingConnectionComponent connectionComponent;

    SourceSettingsComponent settingComponent;

    TopView topView;

    juce::Slider zSlider;

    juce::Label zSliderLabel;


    // the zSlider gets a SliderAttachment to keep sync with its corresponding parameter in the apvts ...
    juce::AudioProcessorValueTreeState::SliderAttachment zSliderAttachment;
    // ... which also needs a ParameterAttachment which calls a callback-function in which we can update the knob inside topView, as its size depends on our z-Position
    std::unique_ptr<juce::ParameterAttachment> zAttachment; // Attachment to connect the z-Value with the RangedAudioParameter in apvts in the PluginProcessor


    
    //========================= deprecated =====================================================
    // juce::Slider rSlider;
    // juce::Label rSliderLabel;
    // void sliderValueChanged(juce::Slider* slider) override;
    //    juce::Slider xSlider;
    //    juce::Slider ySlider;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SeamLess_ClientAudioProcessorEditor)
};
