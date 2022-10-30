/*
==============================================================================

This file contains the basic framework code for a JUCE plugin processor.

==============================================================================
*/

#pragma once

// forward declaration
class ClientConnection;

#include <JuceHeader.h>

#include "../../Common/SeamLess.h"
#include "ClientConnection.h"

//==============================================================================
/**
*/

class SeamLess_ClientAudioProcessor : public juce::AudioProcessor,
    juce::AudioProcessorValueTreeState::Listener, juce::HighResolutionTimer
{
public:
    //==============================================================================
    SeamLess_ClientAudioProcessor();
    /** @cond PRIVATE */
    ~SeamLess_ClientAudioProcessor() override;

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

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
    /** @endcond */
    
    float getXPos();
    float getYPos();
    float getZPos();

    void setXPos(float in);
    void setYPos(float in);
    void setZPos(float in);
    /*
    void xPosSend();
    void yPosSend();
    void zPosSend();
    */
    void xyzPosSend();

    /**
     @brief Send OSC messages for the send gains
    */
    void sendGainSend();

    //==============================================================================

    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;
    juce::AudioProcessorValueTreeState::ParameterLayout createParameters();


    void setSendGain(int sendIndex, float in);

    void setSourceIndex(int i);
    int getSourceIndex();

    void setOscTargetAddress(juce::String address);
    void setOscTargetPort(int port);

    juce::String  getOscTargetAddress();
    int getOscTargetPort();

    int getSettingsMode();
    void setSettingsMode(int newValue);
    enum availableSettingsModes { send, spherical, lfo };

    int getShapeState();
    void setShapeState(int newValue);
    enum availableShapes { HuFo, Studio };

    int getGridState();
    void setGridState(int newValue);
    enum availableGrids { gridOff, gridXYZ, gridSpheric };

    int getSelectedLFO();
    void setSelectedLFO(int newState);
    enum lfosToSelect { x = 1, y = 2, z = 3 };

    /** @brief returns state of connection to main */
    bool getConnectedToMain();
    
    /** @brief sets current state of connection to main */
    void setConnectedToMain(bool b);
    
    /** @brief returns state of sending OSC to main */
    bool getSendState();
    
    /** @brief sets state of sending OSC to main */
    void setSendState(bool s);

    bool getPlayState();

    /** @brief returns state of send button */
    bool getSendButtonState();
    
    /** @brief sets state of send Button */
    void setSendButtonState(bool newValue);

    virtual void parameterChanged(const juce::String& id, float val) override;
    juce::AudioProcessorValueTreeState& getState();

    /** @brief Retrys to connect to the main plugin if no connection is established. */
    void reconnectToMainPlugin();

    //LFOs
    static constexpr size_t xLFOUpdateRate = 100;
    size_t xLFOUpdateCounter = xLFOUpdateRate;
    std::unique_ptr<juce::dsp::Oscillator<float>> xLFO;
    std::unique_ptr<juce::dsp::Oscillator<float>> yLFO;
    std::unique_ptr<juce::dsp::Oscillator<float>> zLFO;
    void prepareLFOs();
    void refreshLFOs();
    void startLFOs();
    void endLFOs();
    std::array<float, 12> getLfoSettings();
    
    float xLFOOut;
    float yLFOOut;
    float zLFOOut;

    void connectXtoParameter(juce::RangedAudioParameter& p);
    void connectYtoParameter(juce::RangedAudioParameter& p);
    void connectZtoParameter(juce::RangedAudioParameter& p);
    void connectLfosToParameters();

    bool getAzimuthChangedWhileRadiusWasZero();
    void setAzimuthChangedWhileRadiusWasZero(bool newState);

    bool getElevationChangedWhileRadiusWasZero();
    void setElevationChangedWhileRadiusWasZero(bool newState);
    
    bool getCriticalRadius();
    void setCriticalRadius(bool newState);

    void setSphericalCoordinates(float radius, float azimuth, float elevation);


    float getCurrentRadius();
    float getCurrentAzimuth();
    float getCurrentElevation();
    void setCurrentAzimuth(float newValue);
    void setCurrentElevation(float newValue);
    void setCurrentRadius(float newValue);

private:

    std::unique_ptr<juce::ParameterAttachment> xAttachment;
    std::unique_ptr<juce::ParameterAttachment> yAttachment;
    std::unique_ptr<juce::ParameterAttachment> zAttachment;

    std::unique_ptr<juce::ParameterAttachment> xLfoRateAttachment;
    std::unique_ptr<juce::ParameterAttachment> xLfoDepthAttachment;
    std::unique_ptr<juce::ParameterAttachment> xLfoPhaseAttachment;
    std::unique_ptr<juce::ParameterAttachment> xLfoOffsetAttachment;

    std::unique_ptr<juce::ParameterAttachment> yLfoRateAttachment;
    std::unique_ptr<juce::ParameterAttachment> yLfoDepthAttachment;
    std::unique_ptr<juce::ParameterAttachment> yLfoPhaseAttachment;
    std::unique_ptr<juce::ParameterAttachment> yLfoOffsetAttachment;

    std::unique_ptr<juce::ParameterAttachment> zLfoRateAttachment;
    std::unique_ptr<juce::ParameterAttachment> zLfoDepthAttachment;
    std::unique_ptr<juce::ParameterAttachment> zLfoPhaseAttachment;
    std::unique_ptr<juce::ParameterAttachment> zLfoOffsetAttachment;

    //std::array<juce::String, 12> lfoSliderNames = {"xLfoRate", "xLfoDepth", "xLfoPhase", "xLfoOffset", "yLfoRate", "yLfoDepth", "yLfoPhase", "yLfoOffset", "zLfoRate", "zLfoDepth", "zLfoPhase", "zLfoOffset" };


    // for the inter com
    const int port_nr = 52713;


   
    bool connectedToMain = false;

    /*
    juce::dsp::ProcessorChain<juce::dsp::Oscillator<float>, juce::dsp::Oscillator<float>, juce::dsp::Oscillator<float>> processorChainLFO;
    enum
    {
        xLFOIndex,
        yLFOIndex,
        zLFOIndex
    };
    */

    // Values to be stored 
    juce::Value oscTargetAddress;
    juce::Value oscTargetPort;
    juce::Value sourceIdx;
    juce::Value settingsMode;
    juce::Value sendButtonState;
    juce::Value shapeState;
    juce::Value gridState;
    juce::Value selectedLFO;


    // used to detect play-state
    juce::AudioPlayHead::PositionInfo playInfo;

    // manual send state (used by all instances):
    bool isSending;
    // play state depending (used by all instances):
    bool playSending;

    /// \brief sender1
    /// @todo Make it non static!
    static juce::OSCSender sender1;

    // Use atomic parameters for thread-safety
    std::atomic<float>* xPos = nullptr;
    std::atomic<float>* yPos = nullptr;
    std::atomic<float>* zPos = nullptr;

//==============================================================================
// the following variables are just to keep the difficutl relation between x,y,z
// and rad, azi, ele in synch. They are ONLY used in the sliderValueChanged() 
// virtual function. To get information about the current state of the source
// always check the current value in the apvts!
//==============================================================================

    float currentAzimuth;
    float currentElevation;
    float currentRadius;

    float newX;
    float newY;
    float newZ;

    /*
    float backupAzimuth;    // store angles in case that radius becomes 0 to be able to go back to the same angles
    float backupElevation;
    */

    bool criticalRadius = false;    // becomes true, when the radius slider gets pulled to a value > 10
    bool azimuthChangedWhileRadiusWasZero = false; // naming is our main skill
    bool elevationChangedWhileRadiusWasZero = false;


    std::atomic<float>* sendGainWFS = nullptr;
    std::atomic<float>* sendGainHOA = nullptr;
    std::atomic<float>* sendGainREV = nullptr;
    std::atomic<float>* sendGainLFE = nullptr;


    //juce::AudioParameterInt* sourceIdx;

    // The AudioProcessorValueTreeState connects parameters to the GUI
    // and manages serialization
    juce::AudioProcessorValueTreeState parameters;
    juce::ValueTree settings;


    std::unique_ptr<ClientConnection> client;    // no longer needed due to parameterAttachments

    virtual void hiResTimerCallback() override;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SeamLess_ClientAudioProcessor)


};
