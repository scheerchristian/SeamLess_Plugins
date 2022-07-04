/*
  ==============================================================================

    SphericalBox.cpp
    Created: 19 Jun 2022 11:13:40pm
    Author:  Uni

  ==============================================================================
*/

#include "SphericalBox.h"

//==============================================================================
SphericalBox::SphericalBox(SeamLess_ClientAudioProcessor& p, juce::AudioProcessorValueTreeState& apvts):
    audioProcessor(p), treeState(apvts),
    
    rSliderAttachment(p.getState(),"radius",this->rSlider)


{
    
    

    
    rSlider.setSliderStyle (juce::Slider::LinearVertical);
    rSlider.setRange (0, 10, 0.01);
    rSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 90, 30);
    rSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::black);
    rSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::white);
    rSlider.setTextValueSuffix(" m");
    
    rSlider.addListener(this);
    addAndMakeVisible (&rSlider);
    
    addAndMakeVisible(rSliderLabel);
    rSliderLabel.setText("Radius", juce::dontSendNotification);
    rSliderLabel.setJustificationType(juce::Justification::centred);
    rSliderLabel.setColour (juce::Label::textColourId, juce::Colours::white);
    rSliderLabel.attachToComponent(&rSlider,false);


    addAndMakeVisible(rSlider);

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
    g.drawText ("Spherical Coordinates", getWidth()/2-100, 20, 200, 20, juce::Justification::centred, true);
}


void SphericalBox::resized()
{
    const int &maxFaderWidth = std::min<int>(getWidth()/6, 60);
    
        rSlider.setBounds(getWidth()/8,                60,maxFaderWidth,getHeight()-90);

        
}


void SphericalBox::sliderValueChanged (juce::Slider* slider)
{
    
              rSlider.setValue (rSlider.getValue(), juce::dontSendNotification);
   
}
