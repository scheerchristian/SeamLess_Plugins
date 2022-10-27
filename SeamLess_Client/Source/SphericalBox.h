/*
  ==============================================================================

    SphericalBox.h
    Created: 19 Jun 2022 11:13:31pm
    Author:  Uni

  ==============================================================================
*/
#pragma once
#include <JuceHeader.h>
#include "../../Common/SeamLess.h"
#include "PluginProcessor.h"
#include "SendFader.h"
#include "SphericalFader.h"
#include <math.h>



class SphericalBox  : public juce::Component
{
public:

    SphericalBox(SeamLess_ClientAudioProcessor &p, juce::AudioProcessorValueTreeState &apvts);
    ~SphericalBox() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void connectXtoParameter(juce::RangedAudioParameter& p);
    void connectYtoParameter(juce::RangedAudioParameter& p);
    void connectZtoParameter(juce::RangedAudioParameter& p);

    void updateSphericalSliders(float x, float y, float z, bool alsoUpdateRadius);

private:

    SeamLess_ClientAudioProcessor& audioProcessor;
    juce::AudioProcessorValueTreeState& treeState;

    SphericalFader radiusSlider;
    SphericalFader azimuthSlider;
    SphericalFader elevationSlider;

    // parameterAttachments to keep radius, azimuth and elavation in sync with x, y and z
    std::unique_ptr<juce::ParameterAttachment> xAttachment;
    std::unique_ptr<juce::ParameterAttachment> yAttachment;
    std::unique_ptr<juce::ParameterAttachment> zAttachment;

    juce::String names[3] = { "radius", "azimuth", "elevation" };
    SphericalFader* faders[3] = { &radiusSlider, &azimuthSlider, &elevationSlider };
    juce::Range<double> ranges[3] = { {0.0, 17.32}, {-180, 180}, {-90, 90} };
    juce::String suffixes[3] = { " m",juce::CharPointer_UTF8("\xc2\xb0"), juce::CharPointer_UTF8("\xc2\xb0") };

    //juce::AudioProcessorValueTreeState::SliderAttachment radiusSliderAttachment;
   
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SphericalBox)
};
