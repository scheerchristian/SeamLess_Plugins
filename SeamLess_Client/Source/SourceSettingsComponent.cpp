/*
  ==============================================================================

    SourceSettingsComponent.cpp
    Created: 10 Apr 2021 10:58:19pm
    Author:  anwaldt

  ==============================================================================
*/

#include <JuceHeader.h>
#include "SourceSettingsComponent.h"

SourceSettingsComponent::SourceSettingsComponent(SeamLess_ClientAudioProcessor *a, juce::AudioProcessorValueTreeState &apvts)
{
    processor = a;

    addAndMakeVisible (inputLabel);
    inputLabel.setText ("Source Index:", juce::dontSendNotification);
    inputLabel.attachToComponent (&sourceIndText, true);
    inputLabel.setColour (juce::Label::textColourId, juce::Colours::black);
    inputLabel.setJustificationType (juce::Justification::right);

    addAndMakeVisible (sourceIndText);
    sourceIndText.setEditable (true);
    sourceIndText.setColour (juce::Label::backgroundColourId, seamlessBlue);

    addAndMakeVisible(nameLabel);
    nameLabel.setText("SeamLess Client", juce::NotificationType::dontSendNotification);
    nameLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    
    addAndMakeVisible (mainConnectionButton);
    mainConnectionButton.setButtonText ("Not Connected!");
    mainConnectionButton.setColour(juce::TextButton::buttonColourId,juce::Colours::red);

    sourceIndText.onTextChange = [this]
    {
        juce::String s = sourceIndText.getText();
        // sourceIndex.setValue( s.getIntValue());
        processor->setSourceIndex(s.getIntValue());
    };
    // xSliderAttachment     = new juce::AudioProcessorValueTreeState::SliderAttachment ();

    //*valueTreeState.getParameter("sourceIdx"),
    sourceIndText.setText(std::to_string(a->getSourceIndex()), juce::NotificationType::dontSendNotification);



    startTimer(100);

}

SourceSettingsComponent::~SourceSettingsComponent()
{
}

void SourceSettingsComponent::paint (juce::Graphics& g)
{
    g.setColour(juce::Colour (220,220,220));
    g.fillRoundedRectangle(0,0,getWidth(),getHeight(),30);

    g.setColour(seamlessBlue);
    g.drawRoundedRectangle(0,0,getWidth(),getHeight(),30,15);

    g.setColour (seamlessBlue);
    g.setFont (14.0f);



}

void SourceSettingsComponent::resized()
{
    nameLabel.setBounds(getWidth()*0.07, getHeight() *0.15, 200, 20);
    if(getWidth()<290)
    {
        nameLabel.setVisible(false);
        sourceIndText.setBounds(getWidth()*0.5+25, getHeight() *0.15, 50, 20);
    }
    else {
        nameLabel.setVisible(true);
        sourceIndText.setBounds(getWidth()*0.92-50, getHeight() *0.15, 50, 20);
    }
    mainConnectionButton.setBounds(getWidth()*0.1,50,getWidth()*0.8,50);
}


void SourceSettingsComponent::timerCallback()
{
    if(sourceIndText.isBeingEdited() == false)
        sourceIndText.setText(juce::String(processor->getSourceIndex()), juce::dontSendNotification);

    this->setConnectionFeedback(processor->getConnectedToMain());

}


void SourceSettingsComponent::setConnectionFeedback(bool state)
{
  if(state==true)
  {
    mainConnectionButton.setColour(juce::TextButton::buttonColourId,juce::Colours::green);
    mainConnectionButton.setButtonText ("Connected to Main Plugin!");
  }
  else if(state==false)
  {
      mainConnectionButton.setColour(juce::TextButton::buttonColourId,juce::Colours::red);
      mainConnectionButton.setButtonText ("Not Connected!");
  }
}
