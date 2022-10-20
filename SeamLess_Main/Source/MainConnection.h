/*
  ==============================================================================

    Connection.h
    Created: 19 Mar 2021 2:52:36pm
    Author:  anwaldt

  ==============================================================================
*/

#pragma once

class SeamLess_MainAudioProcessor;

#include <JuceHeader.h>
#include "PluginProcessor.h"

class MainConnection : public juce::InterprocessConnection
{
public:
    /**
     @brief creates a new Client Connection
     */
    MainConnection(juce::WaitableEvent& stop_signal, SeamLess_MainAudioProcessor *p);

    /**
     @brief gets triggered when a client is connected, prints out a info message 
     */
    void connectionMade();

    /**
    @brief If a MainConnection object loses its connection, it calls the processor's removeClosedConnections method for deleting all disconnected MainConnections
     */
    void connectionLost();

    /**
     @brief gets triggered if the plugin receives a message from client
     */
    void messageReceived(const juce::MemoryBlock& msg);
    
    /**
     @brief contains the latest received general message as a juce::String object
     */
    juce::String message;
    
    /**
     @brief contains the latest received source position message as a juce::String object
     */
    juce::String posmessage;

private:
    SeamLess_MainAudioProcessor *processor;
    juce::WaitableEvent& stop_signal_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainConnection);
};
