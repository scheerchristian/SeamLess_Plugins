/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

#include "ConnectionBox.h"
#include "ReverbFaderBox.h"
#include "OutgoingConnectionComponent.h"
#include "TopView.h"
//==============================================================================
/**
*/
class SeamLess_MainAudioProcessorEditor  : public juce::AudioProcessorEditor, juce::Timer
{
public:
    SeamLess_MainAudioProcessorEditor (SeamLess_MainAudioProcessor&, juce::AudioProcessorValueTreeState&);
    /** @cond PRIVATE */
    ~SeamLess_MainAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override; 
    void resized() override;
    /** @endcond */

private:

    SeamLess_MainAudioProcessor& audioProcessor;

    ConnectionBox oscConnectionBox;

    ReverbFaderBox reverbFaderBox;

    OutgoingConnectionComponent connectionComponent;
    
    TopView sourceViewer;
    
    juce::TextButton fullscreenSourceViewerButton;
    juce::TextButton layoutButton;
    juce::TextButton reInitButton;
    
    bool fullScreen = false;
    
    juce::Rectangle<float> sourceViewerSection;

    void timerCallback() override;
    juce::TooltipWindow tooltip;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SeamLess_MainAudioProcessorEditor)
};
