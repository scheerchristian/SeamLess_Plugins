/*
  ==============================================================================

    ClientConnection.h
    Created: 19 Mar 2021 4:14:08pm
    Author:  anwaldt

  ==============================================================================
*/


#pragma once

class SeamLess_ClientAudioProcessor;

#include <JuceHeader.h>
#include "PluginProcessor.h"

class ClientConnection : public juce::InterprocessConnection
{
public:

    ClientConnection(SeamLess_ClientAudioProcessor &p);

    bool mainConnection();

    void reconnect();

    void sendMessageToMain(juce::String msg);

private:

    SeamLess_ClientAudioProcessor* processor;

    void connectionMade() override;
    void connectionLost() override;
    void messageReceived(const juce::MemoryBlock& msg) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ClientConnection);
};
