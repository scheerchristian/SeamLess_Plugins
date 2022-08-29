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
    setResizeLimits(675, 600, 7000, 8000);
    
//==== BOXES =========================================================================

    addAndMakeVisible(topView);
    addAndMakeVisible(sendBox);
    addAndMakeVisible(sphericalBox);
    sphericalBox.setVisible(false);
    
    addAndMakeVisible(connectionComponent);
    connectionComponent.setOscTargetPortText(audioProcessor.getOscTargetPort());
    addAndMakeVisible(settingComponent);
    
//==== ZSLIDER =======================================================================
    
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
    
    
    

    // ===========================================================================
    

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

    
    
    
//==== BUTTONS ========================================================================
    
    addAndMakeVisible(buttonSettings);
    buttonSettings.addListener(this);
    buttonSettings.setColour(juce::TextButton::buttonColourId,seamlessBlue);
    buttonSettings.setComponentID("settings");
    buttonSettings.setButtonText ("Settings");
    
    addAndMakeVisible(buttonSend);
    buttonSend.addListener(this);
    buttonSend.setColour(juce::TextButton::buttonColourId,seamlessBlue);
    buttonSend.setComponentID("send");
    buttonSend.setButtonText ("Send");
    
    addAndMakeVisible(buttonSpherical);
    buttonSpherical.addListener(this);
    buttonSpherical.setColour(juce::TextButton::buttonColourId,seamlessBlue);
    buttonSpherical.setComponentID("spherical");
    buttonSpherical.setButtonText ("Spherical");
    
    addAndMakeVisible(buttonLayout);
    buttonLayout.addListener(this);
    buttonLayout.setColour(juce::TextButton::buttonColourId,seamlessBlue);
    buttonLayout.setComponentID("layout");
    buttonLayout.setButtonText ("Studio");
    
    addAndMakeVisible(buttonGrid);
    buttonGrid.addListener(this);
    buttonGrid.setColour(juce::TextButton::buttonColourId,seamlessBlue);
    buttonGrid.setComponentID("grid");
    buttonGrid.setButtonText ("Grid OFF");

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
    landscape = true;
    const int &maxTopViewSize = std::min<int>((getWidth()-100)/1.6, getHeight()-40);

    zSlider.setBounds(0,40,100, getHeight()-210);
    topView.setBounds(100,20,maxTopViewSize, maxTopViewSize);
    juce::Component::setColour(juce::ComboBox::outlineColourId, juce::Colours::blue);
    
    sendBox.setBounds(     maxTopViewSize + 120, 20, getWidth() - 140 - maxTopViewSize, getHeight() - 320);
    sphericalBox.setBounds(maxTopViewSize + 120, 20, getWidth() - 140 - maxTopViewSize, getHeight() - 320);

    settingComponent.setBounds(   maxTopViewSize+120, getHeight()-280, getWidth()-maxTopViewSize-140, 120);
    connectionComponent.setBounds(maxTopViewSize+120, getHeight()-140, getWidth()-maxTopViewSize-140, 120);
    
    const int &buttonSpace = getWidth()-maxTopViewSize-130;
    buttonSettings.setBounds( getWidth()-buttonSpace,                      30, (buttonSpace-50)/3, 40);
    buttonSend.setBounds(     getWidth()-buttonSpace+1*(buttonSpace-20)/3, 30, (buttonSpace-50)/3, 40);
    buttonSpherical.setBounds(getWidth()-buttonSpace+2*(buttonSpace-20)/3, 30, (buttonSpace-50)/3, 40);
    buttonLayout.setBounds(20, getHeight()-80, 60, 60);
    buttonGrid.setBounds(20, getHeight()-160, 60, 60);
    
    
    //xSlider.setBounds(210, -10, 400, 120);
    //ySlider.setBounds(-10, 200, 120, 400);
    //rSlider.setBounds(1000, 40, 100, maxTopViewSize-20);


    
    const float &aspectRatio = (float)getWidth()/(float)getHeight();
    if (aspectRatio>1.7)
    {
        settingComponent.setBounds(maxTopViewSize+120, getHeight()-140, (getWidth()-maxTopViewSize-120)*0.5-10, 120);
        connectionComponent.setBounds(maxTopViewSize+120+(getWidth()-maxTopViewSize-120)*0.5, getHeight()-140, (getWidth()-maxTopViewSize-120)*0.5-20, 120);
        
        sendBox.setBounds(sendBox.getBounds().getX(), sendBox.getBounds().getY(), sendBox.getWidth(), getHeight()-180);
        sphericalBox.setBounds(sendBox.getBounds().getX(), sendBox.getBounds().getY(), sendBox.getWidth(), getHeight()-180);
    }
    if (aspectRatio<1.2)
    {
        landscape = false;
        sendBox.boxOrientation(landscape);
        settingComponent.setBounds(maxTopViewSize+120, getHeight()-280, (getWidth()-maxTopViewSize-140), 120);
        connectionComponent.setBounds(maxTopViewSize+120, getHeight()-140, (getWidth()-maxTopViewSize-140), 120);
        
        sendBox.setBounds(100, maxTopViewSize+40, maxTopViewSize, getHeight()-maxTopViewSize-60);
        sphericalBox.setBounds(100, maxTopViewSize+40, maxTopViewSize, getHeight()-maxTopViewSize-60);
        
        buttonSettings.setBounds(maxTopViewSize+120, 20,  getWidth()-maxTopViewSize-140, 40);
        buttonSend.setBounds(maxTopViewSize+120, 80,  getWidth()-maxTopViewSize-140, 40);
        buttonSpherical.setBounds(maxTopViewSize+120, 140, getWidth()-maxTopViewSize-140, 40);
        
        buttonSend.setColour(juce::TextButton::buttonColourId,seamlessBlue);
        buttonSpherical.setColour(juce::TextButton::buttonColourId,seamlessBlue);
        buttonSettings.setColour(juce::TextButton::buttonColourId,seamlessBlue);
        if (sendBox              .isVisible() == true) {buttonSend.setColour(     juce::TextButton::buttonColourId,juce::Colours::grey);}
        else if (sphericalBox    .isVisible() == true) {buttonSpherical.setColour(juce::TextButton::buttonColourId,juce::Colours::grey);}
        else if (settingComponent.isVisible() == true) {buttonSettings.setColour( juce::TextButton::buttonColourId,juce::Colours::grey);}
        else {buttonSend.setColour(                                               juce::TextButton::buttonColourId,juce::Colours::grey);}
    } else {
        buttonSend.setColour(juce::TextButton::buttonColourId,seamlessBlue);
        buttonSpherical.setColour(juce::TextButton::buttonColourId,seamlessBlue);
        buttonSettings.setColour(juce::TextButton::buttonColourId,seamlessBlue);
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
    if (button->getComponentID() == "layout")
    {
        if (button->getButtonText() == "Studio") {
            topView.changeLayout(false);
            buttonLayout.setButtonText("HuFo");
        } else {
            topView.changeLayout(true);
            buttonLayout.setButtonText("Studio");
        }
        
    } else
    if (button->getComponentID() == "grid")
    {
        if (button->getButtonText() == "Grid OFF")
        {
            topView.showGrid(true);
            buttonGrid.setButtonText("Grid \nON");
        } else {
            topView.showGrid(false);
            buttonGrid.setButtonText("Grid OFF");
        }
    } else {
        buttonSpherical.setColour(juce::ComboBox::outlineColourId, juce::Colours::grey);
        buttonSend.setColour(juce::ComboBox::outlineColourId, juce::Colours::grey);
        buttonSettings.setColour(juce::ComboBox::outlineColourId, juce::Colours::grey);
        button->setColour(juce::ComboBox::outlineColourId, seamlessBlue);
        
        //settingComponent.setVisible(false);
        //connectionComponent.setVisible(false);
        sendBox.setVisible(false);
        sphericalBox.setVisible(false);
        
        if (button->getComponentID() == "settings")
        {
            connectionComponent.setVisible(true);
            settingComponent.setVisible(true);
        } else
        if (button->getComponentID() == "send")
        {
            sendBox.setVisible(true);
            topView.changeGrid(true); //true for xyz
        } else
        if (button->getComponentID() == "spherical")
        {
            sphericalBox.setVisible(true);
            topView.changeGrid(false); //false for spherical grid
        }
    }
    if (landscape == false) {
        buttonSend.setColour(juce::TextButton::buttonColourId,seamlessBlue);
        buttonSpherical.setColour(juce::TextButton::buttonColourId,seamlessBlue);
        buttonSettings.setColour(juce::TextButton::buttonColourId,seamlessBlue);
        button->setColour(juce::TextButton::buttonColourId,juce::Colours::grey);
    }
}

//void SeamLess_ClientAudioProcessorEditor::setOscTargetAddressText(SeamLess_ClientAudioProcessorEditor* p, juce::String s)
//{
//    p->oscTargetAddressText.setText(s, juce::sendNotification);
//}
