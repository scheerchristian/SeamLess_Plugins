/*
==============================================================================

This file contains the basic framework code for a JUCE plugin editor.

==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SeamLess_ClientAudioProcessorEditor::SeamLess_ClientAudioProcessorEditor
(SeamLess_ClientAudioProcessor& p, juce::AudioProcessorValueTreeState& apvts)
    : AudioProcessorEditor (&p), audioProcessor (p),
      //xSliderAttachment(p.getState(),"xPos",this->xSlider),
      //ySliderAttachment(p.getState(),"yPos",this->ySlider),
      //rSliderAttachment(p.getState(),"radius",this->rSlider),

      zSliderAttachment(p.getState(),"zPos",this->zSlider),
      treeState(apvts),
      topView(&p),
      connectionComponent(&p),
      settingComponent(&p,apvts),
      sendBox(p,apvts),
      sphericalBox(p, apvts)
{

    setSize (1070, 780);
    setResizable(true, true);
    setResizeLimits(675, 550, 7000, 8000);
    addAndMakeVisible(settingComponent);

    addAndMakeVisible(sendBox);
    addAndMakeVisible(sphericalBox);

    // ===========================================================================
    addAndMakeVisible(topView);

    // ===========================================================================

    //    xSlider.setRange (-10, 10, 0.01);
    //    xSlider.setTextBoxStyle (juce::Slider::TextBoxRight, true, 60, 30);
    //    xSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::black);
    //    xSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::white);

    //    xSlider.setTextValueSuffix(" m");

    //    addAndMakeVisible (&xSlider);
    //    xSlider.addListener(this);

    // ===========================================================================

    //    ySlider.setSliderStyle (juce::Slider::LinearVertical);
    //    ySlider.setRange (-10, 10, 0.01);
    //    ySlider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 90, 30);
    //    ySlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::black);
    //    ySlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::white);
    //    ySlider.setTextValueSuffix(" m");

    //    ySlider.addListener(this);
    //    addAndMakeVisible (&ySlider);

    // ===========================================================================
    
    /*
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
    rSliderLabel.setColour (juce::Label::textColourId, seamlessBlue);
    rSliderLabel.attachToComponent(&rSlider,false);
    */

    zSlider.setSliderStyle (juce::Slider::LinearVertical);
    zSlider.setRange (-10, 10, 0.01);
    zSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 120, 30);
    zSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::black);
    zSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::white);
    zSlider.setTextValueSuffix(" m");

    zSlider.addListener(this);
    addAndMakeVisible (&zSlider);

    addAndMakeVisible(zSliderLabel);
    zSliderLabel.setText("Z Position", juce::dontSendNotification);
    zSliderLabel.setJustificationType(juce::Justification::centred);
    zSliderLabel.setColour (juce::Label::textColourId, seamlessBlue);
    zSliderLabel.attachToComponent(&zSlider,false);

    addAndMakeVisible(connectionComponent);
    connectionComponent.setOscTargetPortText(audioProcessor.getOscTargetPort());

}

SeamLess_ClientAudioProcessorEditor::~SeamLess_ClientAudioProcessorEditor()
{
}

//==============================================================================
void SeamLess_ClientAudioProcessorEditor::paint (juce::Graphics& g)
{
    // fill the whole window white
    g.fillAll (juce::Colours::white);

    // set the current drawing colour to black
    g.setColour (juce::Colours::black);

    // set the font size and draw text to the screen
    g.setFont (15.0f);

    // g.drawFittedText ("SeamLess Source Control", 0, 0, getWidth(), 30, juce::Justification::centred, 1);



}

// ALL components need to be resized() for appearing in the GUI
void SeamLess_ClientAudioProcessorEditor::resized()
{
    sendBox.setBounds(getWidth()-320,20,300,getHeight()-180);
    
    sphericalBox.setBounds(getWidth()-320,20,300,getHeight()-180);
    
    const int &maxTopViewSize = std::min<int>((getWidth()-100)/1.5, getHeight()-180);
   
    topView.setBounds(100,20,maxTopViewSize,maxTopViewSize);
    
    zSlider.setBounds(0, 40, 100, maxTopViewSize-20);
    
    sendBox.setBounds(maxTopViewSize+120,20,getWidth()-140-maxTopViewSize,maxTopViewSize);
    
    sphericalBox.setBounds(maxTopViewSize+120,20,getWidth()-140-maxTopViewSize,maxTopViewSize);
    

    
    
   
    //xSlider.setBounds(210, -10, 400, 120);
    //ySlider.setBounds(-10, 200, 120, 400);
    
    //rSlider.setBounds(1000, 40, 100, maxTopViewSize-20);

    connectionComponent.setBounds(40+maxTopViewSize*0.55, getHeight()-140, maxTopViewSize*0.45+60, 120);

    settingComponent.setBounds(20, getHeight()-140, maxTopViewSize*0.55, 120);

}


void SeamLess_ClientAudioProcessorEditor::sliderValueChanged (juce::Slider* slider)
{
    //    if (slider == &xSlider)
    //        xSlider.setValue (xSlider.getValue(), juce::dontSendNotification);

    //    if (slider == &ySlider)
    //        ySlider.setValue (ySlider.getValue(), juce::dontSendNotification);
    // rSlider.setValue (rSlider.getValue(), juce::dontSendNotification);

    //  if (slider == &zSlider)
    zSlider.setValue (zSlider.getValue(), juce::dontSendNotification);
}


//void SeamLess_ClientAudioProcessorEditor::setOscTargetAddressText(SeamLess_ClientAudioProcessorEditor* p, juce::String s)
//{
//    p->oscTargetAddressText.setText(s, juce::sendNotification);
//}
