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
    /**
     @brief Creates a component containing information about the outgoing connection.
     */
    OutgoingConnectionComponent(SeamLess_ClientAudioProcessor *a);
    ~OutgoingConnectionComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    /**
     Syncs sendButton checkbox state and OSC target port with the audioProcessor's values regularly.
     */
    void timerCallback() override;
    
    /**
     Updates the TextBox containing the OSC target port
     @param newPort Port to be displayed
     */
    void setOscTargetPortText(int newPort);

private:

    SeamLess_ClientAudioProcessor *audioProcessor;

    juce::Label oscTargetAddressText;
    juce::TextEditor oscTargetPortText;
    juce::Label titleLabel;

    juce::ToggleButton sendButton   {"Send OSC"};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OutgoingConnectionComponent)
};
