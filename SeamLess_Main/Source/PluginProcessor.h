/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

class MainConnection;

#include <JuceHeader.h>
#include "MainConnection.h"

//==============================================================================
/**
*/
class SeamLess_MainAudioProcessor  :
        public juce::AudioProcessor,
        juce::AudioProcessorValueTreeState::Listener,
        juce::InterprocessConnectionServer,
        private juce::OSCReceiver,
        juce::OSCReceiver::ListenerWithOSCAddress<juce::OSCReceiver::MessageLoopCallback>,
        juce::HighResolutionTimer
{
public:
    //==============================================================================
    SeamLess_MainAudioProcessor();
    /** @cond PRIVATE */

    ~SeamLess_MainAudioProcessor() override;

    virtual void parameterChanged(const juce::String & id, float val) override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
#endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    
    
    /**
     Restore plugin settings from last session.
     @param data
     @param sizeInBytes
     */
    void setStateInformation (const void* data, int sizeInBytes) override;
    /** @endcond */
    //==============================================================================
    /** @brief Changes the incoming OSC port and prints a message if the connection was successful. */
    void setIncomingPort(int p);
    
    /** @brief Returns the currently used port for listening for incoming OSC messages. */
    int getIncomingPort();

    
    /** @brief Check for unconnected main connection objects and remove them from the vector. */
    void removeClosedConnections();

    /** @brief Returns the parameters stored in the AudioProcessorValueTreeState.*/
    juce::AudioProcessorValueTreeState& getState();
    juce::ValueTree settings;
    void referUnautomatableParameters();


    void revGainSend();
    void revSizeSend();
    void revColorSend();

    /** @brief Returns the current state of receiving OSC.*/
    bool getReceivingState();
    
    /**
     @brief Enables or disables OSC receiving.
     */
    void setReceivingState(bool s);

    /** @brief Sets the OSC target adress to connect to. */
    void setOscTargetAddress(juce::String address);
    
    /** @brief Sets the OSC target port to connect to. */
    void setOscTargetPort(int port);
    
    /** @brief Returns state of sending OSC. */
    bool getSendState();
    
    /** @brief Sets state of sending OSC */
    void setSendState(bool s);
    
    /** @brief Returns the OSC target adress. */
    juce::String getOscTargetAddress();
    
    /** @brief Returns the OSC target port. */
    int getOscTargetPort();
    
    /**
    @param messageType
    @returns the most recent message of the specified type
     */
    juce::StringArray getIncomingMessages(juce::String messageType);
private:

    /// \brief incomingPort
    /// Port for incoming OSC connections to main plugin.
    juce::Value incomingPort;

    // The AudioProcessorValueTreeState connects parameters to the GUI
    // and manages serialization
    juce::AudioProcessorValueTreeState parameters;

    // IP address and port are used by all instances
    juce::Value oscTargetAddress;
    juce::Value oscTargetPort;
    juce::OSCSender oscSender;

    juce::AudioPlayHead::CurrentPositionInfo playInfo;

    bool isSending   = true;
    bool playSending = true;

    std::atomic<float>* revGain   = nullptr;
    std::atomic<float>* revFreq1  = nullptr;
    std::atomic<float>* revFreq2  = nullptr;
    std::atomic<float>* revRdel   = nullptr;
    std::atomic<float>* revRgxyz  = nullptr;
    std::atomic<float>* revT60dc  = nullptr;
    std::atomic<float>* revT60m   = nullptr;
    std::atomic<float>* revLpFreq = nullptr;
    std::atomic<float>* revLpRs   = nullptr;
    std::atomic<float>* revLpDb   = nullptr;

    

    /**
     Passes incoming OSC messages to all client instances.
     @param message
     @todo: Change from string to a more efficient interprocess protocol!
     */
    void oscMessageReceived (const juce::OSCMessage& message) override;

    ///
    /// @brief receiving
    /// used to indicate whether any OSC message has reached the plugin
    bool receiving;

    /// Interconnection Stuff ====================================================================
    juce::WaitableEvent stop_signal;

    std::vector<MainConnection*> connections;

    juce::InterprocessConnection* createConnectionObject() override;

    ///
    /// \brief hiResTimerCallback
    /// Send all OSC commands with a timer call.
    void hiResTimerCallback() override;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SeamLess_MainAudioProcessor)
};
