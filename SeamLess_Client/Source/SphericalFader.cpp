/*
  ==============================================================================

    SphericalFader.cpp
    Created: 31 Mar 2021 10:34:35am
    Author:  anwaldt

  ==============================================================================
*/

#include <JuceHeader.h>
#include "SphericalFader.h"

//==============================================================================
SphericalFader::SphericalFader(SeamLess_ClientAudioProcessor& p, juce::AudioProcessorValueTreeState& apvts, bool endless, juce::Slider::RotaryParameters rotaryParameters)
   
{

    slider.setSliderStyle(juce::Slider::Rotary);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 90, 30);
    slider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::black);
    slider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::grey);
    slider.setNumDecimalPlacesToDisplay(2);
 
    if (endless == true)
        slider.setRotaryParameters(rotaryParameters);
    addAndMakeVisible(slider);

    // SphericalNameLabel.attachToComponent (&slider, false);
    sphericalNameLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    sphericalNameLabel.setJustificationType(juce::Justification::centred);
    sphericalNameLabel.setText("SND", juce::dontSendNotification);
    sphericalNameLabel.attachToComponent(&slider, false);

    addAndMakeVisible(sphericalNameLabel);

}

SphericalFader::~SphericalFader()
{
}

void SphericalFader::paint(juce::Graphics& g)
{
    g.setColour(juce::Colours::grey);
    g.fillRoundedRectangle(getLocalBounds().toFloat(), 10);   // draw an outline around the component
}

void SphericalFader::resized()
{
    slider.setBounds(0,30,getWidth(),getHeight()*0.85);
    //slider.setBounds(getWidth() * -0.1, 40, getWidth() * 1.2, getHeight() * 0.85);
}

void SphericalFader::setText(juce::String s)
{
    sphericalNameLabel.setText(s, juce::dontSendNotification);
}

void SphericalFader::sliderDragStarted(juce::Slider* fader)
{
}

void SphericalFader::sliderDragEnded(juce::Slider* fader)
{

}

void SphericalFader::sliderValueChanged(juce::Slider* fader)
{
}

void SphericalFader::setSliderRange(juce::Range<double> newRange, double newInterval)
{
    slider.setRange(newRange, newInterval);
}

void SphericalFader::setSliderTextValueSuffix(juce::String newSuffix)
{
    slider.setTextValueSuffix(newSuffix);
}
