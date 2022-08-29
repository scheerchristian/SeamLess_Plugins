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
                                             public juce::Slider::Listener,
                                             public juce::Button::Listener
{
public:
    SeamLess_ClientAudioProcessorEditor (SeamLess_ClientAudioProcessor&, juce::AudioProcessorValueTreeState&);
    ~SeamLess_ClientAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    // static void setOscTargetAddressText(SeamLess_ClientAudioProcessorEditor *p, juce::String a);

    private:

    SeamLess_ClientAudioProcessor& audioProcessor;
    juce::AudioProcessorValueTreeState& treeState;

    SendFaderBox sendBox;
    
    SphericalBox sphericalBox;

    OutgoingConnectionComponent connectionComponent;

    SourceSettingsComponent settingComponent;

    TopView topView;
    
    

    //    juce::Slider xSlider;
    //    juce::Slider ySlider;

    juce::Slider zSlider;

    juce::Label zSliderLabel;
    
    juce::TextButton buttonSettings;
    juce::TextButton buttonSend;
    juce::TextButton buttonSpherical;
    juce::TextButton buttonLayout;
    juce::TextButton buttonGrid;
    
    bool landscape;

    // juce::Slider rSlider;
    
    // juce::Label rSliderLabel;

    void sliderValueChanged(juce::Slider* slider) override;
    void buttonClicked(juce::Button* button) override;

    //juce::AudioProcessorValueTreeState::SliderAttachment xSliderAttachment;
    //juce::AudioProcessorValueTreeState::SliderAttachment ySliderAttachment;
    juce::AudioProcessorValueTreeState::SliderAttachment zSliderAttachment;
    // juce::AudioProcessorValueTreeState::SliderAttachment rSliderAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SeamLess_ClientAudioProcessorEditor)

};
