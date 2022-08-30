/*
  ==============================================================================

    SphericalBox.cpp
    Created: 19 Jun 2022 11:13:40pm
    Author:  Uni

  ==============================================================================
*/
#define _USE_MATH_DEFINES

#include "SphericalBox.h"
#include <math.h>

//==============================================================================
SphericalBox::SphericalBox(SeamLess_ClientAudioProcessor& p, juce::AudioProcessorValueTreeState& apvts) :
    audioProcessor(p), treeState(apvts),
    rSlider(p, apvts, false, { 0, 1, true }),
    azimutSlider(p, apvts, true, { 0, 2*M_PI, false }),
    elevationSlider(p, apvts, true, { 0, 2 * M_PI, false })
{
    for (int i = 0; i < 3; i++)
    {
        faders[i]->setText(names[i]);
        addAndMakeVisible(*faders[i]);
        faders[i]->setSliderRange(ranges[i], 0.01);
        faders[i]->setSliderTextValueSuffix(suffixes[i]);
    }
    
 
}

SphericalBox::~SphericalBox()
{
}

void SphericalBox::paint (juce::Graphics& g)
{
    g.setColour (seamlessBlue);
    g.fillRoundedRectangle(getLocalBounds().toFloat(), 20);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    //g.drawText ("Spherical Coordinates", getWidth()/2-100, 20, 200, 20, juce::Justification::centred, true);
}


void SphericalBox::resized()
{
    auto r = getLocalBounds().reduced(20, 20);

    auto sliderWidth = (r.getWidth() - 40) / 3;

    //r.removeFromTop(40);

    auto sendFaderFOASection = r.removeFromLeft(sliderWidth);
    rSlider.setBounds(sendFaderFOASection);

    r.removeFromLeft(20);

    auto sendFaderWFSSection = r.removeFromLeft(sliderWidth);
    azimutSlider.setBounds(sendFaderWFSSection);

    r.removeFromLeft(20);

    auto sendFaderREVSection = r.removeFromLeft(sliderWidth);
    elevationSlider.setBounds(sendFaderREVSection);

}


void SphericalBox::sliderValueChanged (juce::Slider* slider)
{
}
