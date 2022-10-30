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
    incomingPortText.setJustificationType(juce::Justification::centred);
    
    addAndMakeVisible(receiveOscButton);
    receiveOscButton.setTooltip("Turns green when OSC messages are received.");

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
    g.drawText("Receiving OSC on Port", 10, getHeight()/2-5, 150, 10, juce::Justification::left, true);   // draw some placeholder text

}

void ConnectionBox::resized()
{

    incomingPortText.setBounds(145, getHeight()/2-8, 51, 17);
    receiveOscButton.setBounds(200, getHeight()/2-8, 17, 17);
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
