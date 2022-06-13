/*
  ==============================================================================

    OutgoingConnectionComponent.cpp
    Created: 24 Mar 2021 10:31:46pm
    Author:  anwaldt

  ==============================================================================
*/

#include <JuceHeader.h>
#include "OutgoingConnectionComponent.h"

//==============================================================================
OutgoingConnectionComponent::OutgoingConnectionComponent(SeamLess_ClientAudioProcessor *a)
{
    audioProcessor = a;


    addAndMakeVisible(oscTargetAddressText);
    oscTargetAddressText.setText("loco", juce::dontSendNotification);
    oscTargetAddressText.setColour (juce::Label::textColourId, juce::Colours::black);
    oscTargetAddressText.setColour (juce::Label::backgroundColourId, juce::Colours::grey);
    oscTargetAddressText.setJustificationType (juce::Justification::right);
    oscTargetAddressText.setEditable (true);
    oscTargetAddressText.onTextChange = [this]
    {

        juce::IPAddress ip(oscTargetAddressText.getText());
        juce::String s = oscTargetAddressText.getText();
        if (s == "localhost")
            audioProcessor->setOscTargetAddress("127.0.0.1");
        else if (ip.isNull())   // if entered ip adress is invalid
        {
            juce::String messageString("IP-Adress is invalid. Please enter a valid adress. Adress is now being set to the default value 127.0.0.1");
            
            juce::AlertWindow::showAsync(juce::MessageBoxOptions()
                .withIconType(juce::MessageBoxIconType::WarningIcon)
                .withTitle("Invalid IP-Adress")
                .withMessage(messageString)
                .withButton("OK"),
                nullptr);
            audioProcessor->setOscTargetAddress("127.0.0.1");
        }
        else
            audioProcessor->setOscTargetAddress(s);

        // setOscTargetAddressText(this, s);

    };


    addAndMakeVisible(oscTargetPortText);
    oscTargetPortText.setText("mot", juce::dontSendNotification);
    oscTargetPortText.setColour (juce::Label::textColourId, juce::Colours::black);
    oscTargetPortText.setColour (juce::Label::backgroundColourId, juce::Colours::grey);
    //oscTargetPortText.setJustificationType (juce::Justification::right);
    //oscTargetPortText.setEditable (true);
    oscTargetPortText.setInputRestrictions(5, "0123456789");
    
    oscTargetPortText.onTextChange = [this]
    {
        audioProcessor->setOscTargetPort(oscTargetPortText.getText().getIntValue());
    };



    addAndMakeVisible(sendButton);
    sendButton.onClick = [this]
    {
        audioProcessor->setSendState(sendButton.getToggleStateValue().getValue());
    };

    startTimer(100);
}

OutgoingConnectionComponent::~OutgoingConnectionComponent()
{

}

void OutgoingConnectionComponent::paint (juce::Graphics& g)
{


    g.setColour (seamlessBlue);
    g.fillRoundedRectangle(getLocalBounds().toFloat(), 20);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("Outgoing Connection (All Client Plugins)", 20, 20, 200, 20,
                juce::Justification::left, true);   // draw some placeholder text

}

void OutgoingConnectionComponent::resized()
{
    oscTargetAddressText.setBounds(40, 80, 120, 20);
    oscTargetPortText.setBounds(180, 80, 80, 20);
    sendButton.setBounds(40,50,80,20);
}


void OutgoingConnectionComponent::timerCallback()
{
    // update text boxes only if they are not being edited
    if(oscTargetAddressText.isBeingEdited() == false)
        oscTargetAddressText.setText(audioProcessor->getOscTargetAddress(), juce::dontSendNotification);

    //if(oscTargetPortText.isBeingEdited() == false)
        //oscTargetPortText.setText(juce::String(audioProcessor->getOscTargetPort()), juce::dontSendNotification);

    sendButton.setToggleState(audioProcessor->getSendState(), juce::dontSendNotification);

}

void OutgoingConnectionComponent::setOscTargetPortText(int newPort)
{
    oscTargetPortText.setText(std::to_string(newPort));
}
