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
    
    addAndMakeVisible(titleLabel);
    titleLabel.setText("Outgoing Connection (All Client Plugins)", juce::NotificationType::dontSendNotification);

    addAndMakeVisible(oscTargetPortText);
    oscTargetPortText.setText("mot", juce::dontSendNotification);
    oscTargetPortText.setColour (juce::TextEditor::textColourId, juce::Colours::black);
    oscTargetPortText.setColour (juce::TextEditor::backgroundColourId, juce::Colours::grey);
    oscTargetPortText.setSelectAllWhenFocused(true);
    oscTargetPortText.setJustification(juce::Justification::centredRight);
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
    

}

void OutgoingConnectionComponent::resized()
{
    if(getWidth()<297)
    {
        titleLabel.setBounds(getWidth()/2-70, 20, 140, 20);
        titleLabel.setText("Outgoing Connection", juce::NotificationType::dontSendNotification);
    }
    else
    {
        titleLabel.setBounds(getWidth()/2-125, 20, 250, 20);
        titleLabel.setText("Outgoing Connection (All Client Plugins)", juce::NotificationType::dontSendNotification);
    }
    oscTargetAddressText.setBounds(getWidth()*0.1, 80, getWidth()*0.45, 20);
    oscTargetPortText.setBounds(getWidth()*0.55+10, 80, getWidth()*0.3, 20);
    sendButton.setBounds(getWidth()*0.1,50,80,20);
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
