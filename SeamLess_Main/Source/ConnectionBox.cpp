/*
  ==============================================================================

    ConnectionBox.cpp
    Created: 15 Jul 2021 12:02:09pm
    Author:  anwaldt

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ConnectionBox.h"

//==============================================================================
ConnectionBox::ConnectionBox(SeamLess_MainAudioProcessor &p) : audioProcessor(p)
{
    addAndMakeVisible(incomingPortText);
    incomingPortText.setText("-", juce::dontSendNotification);
    incomingPortText.setEditable (true);
    incomingPortText.setColour(juce::Label::textColourId, juce::Colours::white);
    incomingPortText.setColour (juce::Label::backgroundColourId, seamlessBlue);

    addAndMakeVisible(receiveOscButton);
    receiveOscButton.setButtonText("Receiving OSC");

    incomingPortText.onTextChange = [this]
    {
        audioProcessor.setIncomingPort(incomingPortText.getText().getIntValue());
    };

    startTimer(100);

}

ConnectionBox::~ConnectionBox()
{
}

void ConnectionBox::paint (juce::Graphics& g)
{

    g.setColour (juce::Colours::grey);
    g.fillRoundedRectangle(getLocalBounds().toFloat(), 20);   // draw an outline around the component

    g.setColour(seamlessBlue);
    g.setFont(14.0f);
    g.drawText("Port for receiving OSC", 10, 10, 250, 10,
        juce::Justification::left, true);   // draw some placeholder text

}

void ConnectionBox::resized()
{

    incomingPortLabel.setBounds(10, 20, 220, 20);
    incomingPortText.setBounds(30, 60, 120, 20);
    receiveOscButton.setBounds(180, 30, 70, 50);   
}



void ConnectionBox::timerCallback()
{

    if(incomingPortText.isBeingEdited() == false)

        incomingPortText.setText(juce::String(audioProcessor.getIncomingPort()), juce::dontSendNotification);

    bool rs = audioProcessor.getReceivingState();

    if(rs==true)
    {
        receiveOscButton.setColour(juce::TextButton::buttonColourId,juce::Colours::green);
        audioProcessor.setReceivingState(false);
    }
    else
    {
        receiveOscButton.setColour(juce::TextButton::buttonColourId,juce::Colours::grey);
    }
}
