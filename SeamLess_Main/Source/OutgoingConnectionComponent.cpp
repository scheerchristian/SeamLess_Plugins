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
OutgoingConnectionComponent::OutgoingConnectionComponent(SeamLess_MainAudioProcessor *a)
{
    audioProcessor = a;

    addAndMakeVisible(oscTargetAddressText);
    oscTargetAddressText.setText(audioProcessor->getOscTargetAddress(), juce::dontSendNotification);
    oscTargetAddressText.setColour (juce::Label::textColourId, juce::Colours::white);
    oscTargetAddressText.setColour (juce::Label::backgroundColourId, seamlessBlue);
    oscTargetAddressText.setJustificationType (juce::Justification::right);
    oscTargetAddressText.setTooltip("The target's IP-Adress.");
    oscTargetAddressText.setEditable (true);
    oscTargetAddressText.onTextChange = [this]
    {
        juce::IPAddress ip(oscTargetAddressText.getText());
        juce::String s = oscTargetAddressText.getText();
        juce::String ss = s.substring(0, 1);
        if (s == "localhost")
            audioProcessor->setOscTargetAddress("127.0.0.1");
        else if (ss == ".")
        {
            juce::StringArray sa;
            sa.addTokens(juce::IPAddress::getLocalAddress().toString(), ".", "/");
            audioProcessor->setOscTargetAddress(juce::IPAddress::getLocalAddress().toString().trimCharactersAtEnd(sa[3])+s.trimCharactersAtStart("."));
        }
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
    };
    

    addAndMakeVisible(oscTargetPortText);
    oscTargetPortText.setText(juce::String(audioProcessor->getOscTargetPort()), juce::dontSendNotification);
    oscTargetPortText.setColour (juce::TextEditor::textColourId, juce::Colours::white);
    oscTargetPortText.setColour (juce::TextEditor::backgroundColourId, seamlessBlue);
    oscTargetPortText.setSelectAllWhenFocused(true);
    oscTargetPortText.setJustification (juce::Justification::centredRight);
    oscTargetPortText.setTooltip("The target's UDP-Port");
    //oscTargetPortText.setEditable (true);
    oscTargetPortText.setInputRestrictions(5, "0123456789");
    
    oscTargetPortText.onReturnKey = [this]
    {
        audioProcessor->setOscTargetPort(oscTargetPortText.getText().getIntValue());
        oscTargetPortText.setEnabled(false);
        oscTargetPortText.setEnabled(true);

    };
    oscTargetPortText.onFocusLost = [this]
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

    g.setColour (juce::Colours::grey);
    g.fillRoundedRectangle(getLocalBounds().toFloat(), 20);   // draw an outline around the component

    g.setColour (seamlessBlue);
    g.setFont (14.0f);
    g.drawText ("Outgoing Connection (Main Plugin)", 0, 10, getWidth(), 10, juce::Justification::centred, true);   // draw some placeholder text


}

void OutgoingConnectionComponent::resized()
{
    sendButton.setBounds(30, 30, 80, 20);
    oscTargetAddressText.setBounds(30, 60, 120, 20);
    oscTargetPortText.setBounds(180, 60, 80, 20);
    sendButton.setBounds(getWidth()*0.05, getHeight()*0.35, 80, 20);
    oscTargetAddressText.setBounds(getWidth()*0.05, getHeight()*0.65, getWidth()*0.55, 20);
    oscTargetPortText.setBounds(getWidth()*0.65, getHeight()*0.65, getWidth()*0.3, 20);
}


void OutgoingConnectionComponent::timerCallback()
{
    // update text boxes only if they are not being edited
    if(oscTargetAddressText.isBeingEdited() == false)
    {
        oscTargetAddressText.setText(audioProcessor->getOscTargetAddress(), juce::dontSendNotification);
    }

    sendButton.setToggleState(audioProcessor->getSendState(), juce::dontSendNotification);

}


