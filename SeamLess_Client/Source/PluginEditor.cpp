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
      //buttonBox(),
      connectionComponent(&p),
      settingComponent(&p,apvts),
      sendBox(p,apvts),
      sphericalBox(p, apvts)
{
    
    setSize (1070, 780);
    setResizable(true, true);
    setResizeLimits(675, 600, 7000, 8000);
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

    addAndMakeVisible(button1);
    button1.addListener(this);
    button1.setColour(juce::TextButton::buttonColourId,juce::Colours::grey);
    button1.setComponentID("settings");
    button1.setButtonText ("Settings");
    
    addAndMakeVisible(button2);
    button2.addListener(this);
    button2.setColour(juce::TextButton::buttonColourId,juce::Colours::grey);
    button2.setComponentID("send");
    button2.setButtonText ("Send");
    
    addAndMakeVisible(button3);
    button3.addListener(this);
    button3.setColour(juce::TextButton::buttonColourId,seamlessBlue);
    button3.setComponentID("spherical");
    button3.setButtonText ("Spherical");
    
    sphericalBox.setVisible(false);
    
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
    auto r = getLocalBounds();
    
    const int &maxTopViewSize = std::min<int>((getWidth()-100)/1.5, getHeight()-40);
    //const int &maxTopViewSize = std::min<int>((getWidth()-100)/1.5, getHeight()-180);

    topView.setBounds(100,20,maxTopViewSize,maxTopViewSize);
    
    zSlider.setBounds(0, 40, 100, maxTopViewSize-20);
    
    sendBox.setBounds(maxTopViewSize + 120, 80, getWidth() - 140 - maxTopViewSize, getHeight() - 380);
    //sendBox.setBounds(maxTopViewSize + 120, 20, getWidth() - 140 - maxTopViewSize, maxTopViewSize / 2 - 10);
    
    sphericalBox.setBounds(maxTopViewSize + 120, 80, getWidth() - 140 - maxTopViewSize, getHeight() - 380);
    
    connectionComponent.setBounds(maxTopViewSize+120, getHeight()-140, getWidth()-maxTopViewSize-140, 120);
    //connectionComponent.setBounds(40+maxTopViewSize*0.55, getHeight()-140, maxTopViewSize*0.45+60, 120);

    settingComponent.setBounds(maxTopViewSize+120, getHeight()-280, getWidth()-maxTopViewSize-140, 120);
    //settingComponent.setBounds(20, getHeight()-140, maxTopViewSize*0.55, 120);
   
    button1.setBounds(maxTopViewSize+120, 20, (getWidth()-maxTopViewSize-120)/3-20, 40);
    button2.setBounds(maxTopViewSize+120+(getWidth()-maxTopViewSize-120)/3, 20, (getWidth()-maxTopViewSize-120)/3-20, 40);
    button3.setBounds(maxTopViewSize+120+2*(getWidth()-maxTopViewSize-120)/3, 20, (getWidth()-maxTopViewSize-120)/3-20, 40);
    
    
    
    //xSlider.setBounds(210, -10, 400, 120);
    //ySlider.setBounds(-10, 200, 120, 400);
    
    //rSlider.setBounds(1000, 40, 100, maxTopViewSize-20);


    
    const float &aspectRatio = (float)getWidth()/(float)getHeight();
    if (aspectRatio>1.6){
    const int &maxTopViewSize = std::min<int>((getWidth()-100)/1.5, getHeight()-40);
        topView.setBounds(100,20,maxTopViewSize,maxTopViewSize);
        settingComponent.setBounds(maxTopViewSize+120, getHeight()-140, (getWidth()-maxTopViewSize-120)*0.5-10, 120);
        connectionComponent.setBounds(maxTopViewSize+120+(getWidth()-maxTopViewSize-120)*0.5, getHeight()-140, (getWidth()-maxTopViewSize-120)*0.5-20, 120);
        sendBox.setBounds(     maxTopViewSize + 120, 80, getWidth() - 140 - maxTopViewSize, getHeight()-240);
        sphericalBox.setBounds(maxTopViewSize + 120, 80, getWidth() - 140 - maxTopViewSize, getHeight()-240);
        zSlider.setBounds(0, 40, 100, maxTopViewSize-20);
        button1.setBounds(maxTopViewSize+120,                                     20, (getWidth()-maxTopViewSize-120)/3-20, 40);
        button2.setBounds(maxTopViewSize+120+  (getWidth()-maxTopViewSize-120)/3, 20, (getWidth()-maxTopViewSize-120)/3-20, 40);
        button3.setBounds(maxTopViewSize+120+2*(getWidth()-maxTopViewSize-120)/3, 20, (getWidth()-maxTopViewSize-120)/3-20, 40);
    }
    if (aspectRatio<1.2){
    const int &maxTopViewSize = std::min<int>((getWidth()-100)/1.5, getHeight()-40);
        topView.setBounds(100,20,maxTopViewSize,maxTopViewSize);
        settingComponent.setBounds(maxTopViewSize+120, getHeight()-280, (getWidth()-maxTopViewSize-140), 120);
        connectionComponent.setBounds(maxTopViewSize+120, getHeight()-140, (getWidth()-maxTopViewSize-140), 120);
        sendBox.setBounds(100, maxTopViewSize+40, maxTopViewSize, getHeight()-maxTopViewSize-60);
        sphericalBox.setBounds(100, maxTopViewSize+40, maxTopViewSize, getHeight()-maxTopViewSize-60);
        zSlider.setBounds(0, 40, 100, maxTopViewSize-20);
        button1.setBounds(maxTopViewSize+120, 20,  getWidth()-maxTopViewSize-140, 40);
        button2.setBounds(maxTopViewSize+120, 80,  getWidth()-maxTopViewSize-140, 40);
        button3.setBounds(maxTopViewSize+120, 140, getWidth()-maxTopViewSize-140, 40);

    }
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
void SeamLess_ClientAudioProcessorEditor::buttonClicked (juce::Button* button)
{
    if (button->getComponentID() == "settings")
    {
        if (settingComponent.isVisible() == true)
        {
            settingComponent.setVisible(false);
            connectionComponent.setVisible(false);
            button1.setColour(juce::TextButton::buttonColourId,seamlessBlue);
            
        } else {
        settingComponent.setVisible(true);
        connectionComponent.setVisible(true);
        button1.setColour(juce::TextButton::buttonColourId,juce::Colours::grey);
            }
    
    } else
    if (button->getComponentID() == "send")
    {
        sendBox.setVisible(true);
        sphericalBox.setVisible(false);

        button2.setColour(juce::TextButton::buttonColourId,juce::Colours::grey);
        button3.setColour(juce::TextButton::buttonColourId,seamlessBlue);
        
    } else
    if (button->getComponentID() == "spherical")
    {
        sendBox.setVisible(false);
        sphericalBox.setVisible(true);
        button2.setColour(juce::TextButton::buttonColourId,seamlessBlue);
        button3.setColour(juce::TextButton::buttonColourId,juce::Colours::grey);
    }
}

//void SeamLess_ClientAudioProcessorEditor::setOscTargetAddressText(SeamLess_ClientAudioProcessorEditor* p, juce::String s)
//{
//    p->oscTargetAddressText.setText(s, juce::sendNotification);
//}
