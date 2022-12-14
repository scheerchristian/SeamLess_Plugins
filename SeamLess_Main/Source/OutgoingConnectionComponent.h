/*
  ==============================================================================

    OutgoingConnectionComponent.h
    Created: 24 Mar 2021 10:31:46pm
    Author:  anwaldt

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "../../Common/SeamLess.h"

//==============================================================================
/*
*/
class OutgoingConnectionComponent  : public juce::Component, juce::Timer
{
public:
    OutgoingConnectionComponent(SeamLess_MainAudioProcessor *a);
    /** @cond PRIVATE */
    ~OutgoingConnectionComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void timerCallback() override;
    /**@endcond*/
private:

    SeamLess_MainAudioProcessor *audioProcessor;

    juce::Label oscTargetAddressText;
    juce::TextEditor oscTargetPortText;
    bool portTextFocused = false;
    juce::ToggleButton sendButton   {"Send OSC"};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OutgoingConnectionComponent)
};
