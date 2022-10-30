#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"
#include <cmath>

//==============================================================================
/*
*/
class SphericalFader : public juce::Slider, juce::Slider::Listener
{


public:
    SphericalFader(SeamLess_ClientAudioProcessor& p, juce::AudioProcessorValueTreeState& apvts, bool endless, juce::Slider::RotaryParameters rotaryParameters, juce::String type);
    ~SphericalFader() override;

    void paint(juce::Graphics&) override;
    void resized() override;

    void setText(juce::String s);

    void sliderDragStarted(juce::Slider* fader) override;
    void sliderDragEnded(juce::Slider*) override;
    void sliderValueChanged(juce::Slider*) override;

    void setSliderRange(juce::Range<double> newRange, double newInterval);
    void setSliderTextValueSuffix(juce::String newSuffix);

    juce::Slider slider;

    bool onDrag = false;

    void connectXtoParameter(juce::RangedAudioParameter& p);
    void connectYtoParameter(juce::RangedAudioParameter& p);
    void connectZtoParameter(juce::RangedAudioParameter& p);

    bool isRadiusCritical();
    void setRadiusCritical(bool newValue);

    float getCurrentRadius();
    float getCurrentAzimuth();
    float getCurrentElevation();


private:
    juce::Label sphericalNameLabel;
    SeamLess_ClientAudioProcessor& audioProcessor;
    juce::AudioProcessorValueTreeState& treeState;
    juce::String sliderType;
    std::unique_ptr<juce::ParameterAttachment> xAttachment;
    std::unique_ptr<juce::ParameterAttachment> yAttachment;
    std::unique_ptr<juce::ParameterAttachment> zAttachment;


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
    
    float backupAzimuth;    // store angles in case that radius becomes 0 to be able to go back to the same angles
    float backupElevation;

    bool criticalRadius = false;    // becomes true, when the radius slider gets pulled to a value > 10

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SphericalFader)
};
