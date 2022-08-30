/*
  ==============================================================================

    SendFaderBox.cpp
    Created: 31 Mar 2021 12:13:37am
    Author:  anwaldt

  ==============================================================================
*/
#define _USE_MATH_DEFINES

#include <JuceHeader.h>
#include "SendFaderBox.h"
#include "PluginProcessor.h"
#include <math.h>
//==============================================================================
SendFaderBox::SendFaderBox(SeamLess_ClientAudioProcessor& p, juce::AudioProcessorValueTreeState& apvts):
    audioProcessor(p), treeState(apvts),
    sendFaderHOA(p, apvts, "sendGainHOA", false, { 0, 1, true }),
    sendFaderWFS(p, apvts, "sendGainWFS", false, { 0, 1, true }),
    sendFaderREV(p, apvts, "sendGainREV", false, { 0, 1, true })
//    sendFaderLFE(p,apvts,"sendGainLFE")
{

    sendFaderWFS.setText("WFS");
    sendFaderHOA.setText("HOA");
    sendFaderREV.setText("REV");
//    sendFaderLFE.setText("LFE");

    addAndMakeVisible(sendFaderHOA);
    addAndMakeVisible(sendFaderWFS);
    addAndMakeVisible(sendFaderREV);
   // addAndMakeVisible(sendFaderLFE);

    // sendFaderWFS.addListener(this);

}

SendFaderBox::~SendFaderBox()
{
}

void SendFaderBox::paint (juce::Graphics& g)
{
    g.setColour (seamlessBlue);
    g.fillRoundedRectangle(getLocalBounds().toFloat(), 20);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    //g.drawText ("Send Levels", getWidth()/2-100, 20, 200, 20, juce::Justification::centred, true);
}


void SendFaderBox::resized()
{
    
    auto r = getLocalBounds().reduced(20, 20);
    
    auto sliderWidth = (r.getWidth()-40)/3 ;
    
    auto sendFaderHOASection = r.removeFromLeft(sliderWidth);
    sendFaderHOA.setBounds(sendFaderHOASection);

    r.removeFromLeft(20);

    auto sendFaderWFSSection = r.removeFromLeft(sliderWidth);
    sendFaderWFS.setBounds(sendFaderWFSSection);

    r.removeFromLeft(20);

    auto sendFaderREVSection = r.removeFromLeft(sliderWidth);
    sendFaderREV.setBounds(sendFaderREVSection);

    /*
    const int &maxFaderWidth = std::min<int>(getWidth()/6, 60);
    
        sendFaderHOA.setBounds(20, 60, maxFaderWidth, getHeight() - 90);
        sendFaderWFS.setBounds(20 + getWidth()/3 + 10 - maxFaderWidth/2, 60, maxFaderWidth, getHeight() - 90);
        sendFaderREV.setBounds(20 + getWidth()/3 + 20 + getWidth() / 3 + 10 - maxFaderWidth, 60, maxFaderWidth, getHeight() - 90);
    
//    sendFaderLFE.setBounds(370,60,60,460);
        */
}


void SendFaderBox::sliderValueChanged (juce::Slider* slider)
{
    //    if (slider == &xSlider)
    //        xSlider.setValue (xSlider.getValue(), juce::dontSendNotification);

    //    if (slider == &ySlider)
    //        ySlider.setValue (ySlider.getValue(), juce::dontSendNotification);

    //    if (slider == &zSlider)
    //        zSlider.setValue (zSlider.getValue(), juce::dontSendNotification);
}

