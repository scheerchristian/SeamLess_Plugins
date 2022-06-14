/*
  ==============================================================================

    OutgoingConnectionComponent.h
    Created: 24 Mar 2021 10:31:46pm
    Author:  anwaldt

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "../../Common/SeamLess.h"
#include "PluginProcessor.h"

//==============================================================================
/*
*/
class OutgoingConnectionComponent  : public juce::Component, juce::Timer
{
public:
    OutgoingConnectionComponent(SeamLess_ClientAudioProcessor *a);
    ~OutgoingConnectionComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void timerCallback() override;
    void setOscTargetPortText(int newPort);

private:

    SeamLess_ClientAudioProcessor *audioProcessor;

    juce::Label oscTargetAddressText;
    juce::TextEditor oscTargetPortText;
    juce::Label titleLabel;

    juce::ToggleButton sendButton   {"Send OSC"};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OutgoingConnectionComponent)
};
