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
    /** @cond PRIVATE */
    ~SphericalFader() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    /** @endcond*/
    
    void setText(juce::String s);

    void sliderDragStarted(juce::Slider* fader) override;
    void sliderDragEnded(juce::Slider*) override;
    void sliderValueChanged(juce::Slider*) override;

    void setSliderRange(juce::Range<double> newRange, double newInterval);
    void setSliderTextValueSuffix(juce::String newSuffix);

    juce::Slider slider;

    bool onDrag = false;

    /** Callback-function which is called when x parameter in the apvts change. */
    void connectXtoParameter(juce::RangedAudioParameter& p);
    /** Callback-function which is called when y parameter in the apvts change. */
    void connectYtoParameter(juce::RangedAudioParameter& p);
    /** Callback-function which is called when z parameter in the apvts change. */
    void connectZtoParameter(juce::RangedAudioParameter& p);
    


private:
    juce::Label sphericalNameLabel;
    SeamLess_ClientAudioProcessor& audioProcessor;
    juce::AudioProcessorValueTreeState& treeState;
    juce::String sliderType;
    std::unique_ptr<juce::ParameterAttachment> xAttachment;
    std::unique_ptr<juce::ParameterAttachment> yAttachment;
    std::unique_ptr<juce::ParameterAttachment> zAttachment;




    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SphericalFader)
};
