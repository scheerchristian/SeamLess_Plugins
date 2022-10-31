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
    /** @brief creates a new connection to the main plugin. */
    ClientConnection(SeamLess_ClientAudioProcessor &p);

    /** @brief Returns the current connection status. */
    bool mainConnection();

    /**
     Sends a message to the connected master plugin, if a connection is established.
     
     The Message should be a juce::String, which does not include the character "§".
     
     As of 10/2022, the only used message type is "pos", for communicating the source positions to the main plugin's source viewer.
     
     Other message types can be added if needed.
     
     @param messageType Type of Message
     @param message Message to be send
     */
    void sendMessageToMain(juce::String messageType, juce::String message);

private:

    SeamLess_ClientAudioProcessor* processor;

    void connectionMade() override;
    void connectionLost() override;
    void messageReceived(const juce::MemoryBlock& msg) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ClientConnection);
};
