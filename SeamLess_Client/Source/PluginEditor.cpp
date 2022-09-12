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
      zSliderAttachment(p.getState(),"zPos",this->zSlider),
      treeState(apvts),
      topView(&p, apvts),
      connectionComponent(&p),
      settingComponent(&p,apvts),
      sendBox(p,apvts),
      sphericalBox(p, apvts),
      lfoBox(p, apvts)
{    
    //connect the parameterAttachments and initialize their callback lambda-functions
    topView.connectXtoParameter(*treeState.getParameter("xPos"));
    topView.connectYtoParameter(*treeState.getParameter("yPos"));
    connectZToParameter(*treeState.getParameter("zPos"));
    
    setSize (1000, 600);
    setResizable(true, true);
    setResizeLimits(675, 600, 7000, 8000);
    
//==== BOXES =========================================================================

    addAndMakeVisible(topView);
    addAndMakeVisible(sendBox);
    addAndMakeVisible(sphericalBox);
    addAndMakeVisible(lfoBox);
    sphericalBox.setVisible(false);
    sendBox.setVisible(false);
    
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
    zSliderLabel.attachToComponent(&zSlider, false);
    
//==== BUTTONS ========================================================================
    
    addAndMakeVisible(buttonSettings);
    buttonSettings.addListener(this);
    buttonSettings.setColour(juce::TextButton::buttonColourId,seamlessBlue);
    buttonSettings.setColour(juce::ComboBox::outlineColourId,seamlessBlue);
    buttonSettings.setComponentID("settings");
    buttonSettings.setButtonText ("Settings");
    
    addAndMakeVisible(buttonSend);
    buttonSend.addListener(this);
    buttonSend.setColour(juce::TextButton::buttonColourId,juce::Colours::grey);
    buttonSend.setColour(juce::ComboBox::outlineColourId,seamlessBlue);
    buttonSend.setComponentID("send");
    buttonSend.setButtonText ("Send");
    
    addAndMakeVisible(buttonSpherical);
    buttonSpherical.addListener(this);
    buttonSpherical.setColour(juce::TextButton::buttonColourId,seamlessBlue);
    buttonSpherical.setColour(juce::ComboBox::outlineColourId,seamlessBlue);
    buttonSpherical.setComponentID("spherical");
    buttonSpherical.setButtonText ("Spherical");
    
    addAndMakeVisible(buttonLayout);
    buttonLayout.addListener(this);
    buttonLayout.setColour(juce::TextButton::buttonColourId,seamlessBlue);
    buttonLayout.setComponentID("layout");
    buttonLayout.setButtonText ("HuFo");
    
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
    
    const int &maxTopViewSize = std::min<int>((getWidth()-100)/1.6, getHeight()-40);

    zSlider.setBounds(0,40,100, getHeight()-195);
    topView.setBounds(100,20,maxTopViewSize, maxTopViewSize);
    juce::Component::setColour(juce::ComboBox::outlineColourId, juce::Colours::blue);
    
    sendBox.setBounds(     maxTopViewSize + 120, 70, getWidth() - 140 - maxTopViewSize, getHeight() - 350);
    sphericalBox.setBounds(maxTopViewSize + 120, 70, getWidth() - 140 - maxTopViewSize, getHeight() - 350);

    settingComponent.setBounds(   maxTopViewSize+120, getHeight()-270, getWidth()-maxTopViewSize-140, 120);
    connectionComponent.setBounds(maxTopViewSize+120, getHeight()-140, getWidth()-maxTopViewSize-140, 120);
    lfoBox.setBounds(   maxTopViewSize + 120, 20 + maxTopViewSize / 2 + 10, getWidth() - 40 - maxTopViewSize, maxTopViewSize / 2 - 10);
    
    buttonSettings.setBounds(sendBox.getBounds().getX(),    20, sendBox.getWidth()/3, 40);
    buttonSend.setBounds(buttonSettings.getBounds().getX()+sendBox.getWidth()/3, 20, sendBox.getWidth()/3, 40);
    buttonSpherical.setBounds(buttonSend.getBounds().getX()+sendBox.getWidth()/3, 20, sendBox.getWidth()/3, 40);
    buttonLayout.setBounds(20, getHeight()-80, 60, 60);
    buttonGrid.setBounds(20, getHeight()-150, 60, 60);


    if (sendBox              .isVisible() == true) {buttonSend.setColour(     juce::TextButton::buttonColourId,juce::Colours::grey);}
    else if (sphericalBox    .isVisible() == true) {buttonSpherical.setColour(juce::TextButton::buttonColourId,juce::Colours::grey);}
    else if (settingComponent.isVisible() == true) {buttonSettings.setColour( juce::TextButton::buttonColourId,juce::Colours::grey);}
    else {buttonSend.setColour(                                               juce::TextButton::buttonColourId,juce::Colours::grey);}

    
    const float &aspectRatio = (float)getWidth()/(float)getHeight();
    if (aspectRatio<1.3)
    {
        settingComponent.setBounds(maxTopViewSize+120, getHeight()-280, (getWidth()-maxTopViewSize-140), 120);
        connectionComponent.setBounds(maxTopViewSize+120, getHeight()-140, (getWidth()-maxTopViewSize-140), 120);
        
        sendBox.setBounds(100, maxTopViewSize+30, maxTopViewSize, getHeight()-maxTopViewSize-50);
        sphericalBox.setBounds(100, maxTopViewSize+30, maxTopViewSize, getHeight()-maxTopViewSize-50);
        
        buttonSettings.setBounds(maxTopViewSize+120, 20,  getWidth()-maxTopViewSize-140, 40);
        buttonSend.setBounds(maxTopViewSize+120, 80,  getWidth()-maxTopViewSize-140, 40);
        buttonSpherical.setBounds(maxTopViewSize+120, 140, getWidth()-maxTopViewSize-140, 40);
        
        //buttonSend.setColour(juce::TextButton::buttonColourId,seamlessBlue);
        //buttonSpherical.setColour(juce::TextButton::buttonColourId,seamlessBlue);
        //buttonSettings.setColour(juce::TextButton::buttonColourId,seamlessBlue);
    }
}
void SeamLess_ClientAudioProcessorEditor::connectZToParameter(juce::RangedAudioParameter& p)
{
    // lambda callback function of z-Parameter
    zAttachment = std::make_unique<juce::ParameterAttachment>(p, [this](float newValue)
        {
            auto x = treeState.getParameterAsValue("xPos").toString().getFloatValue();
            auto y = treeState.getParameterAsValue("yPos").toString().getFloatValue();
            // update the width of the slider-knob inside topView
            topView.setSourceWidthPx(newValue + 20);    
            topView.resized();
            topView.setZPos(newValue);

        });
    zAttachment->sendInitialUpdate();   //  update everything when loading the plugin
}
void SeamLess_ClientAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
}

void SeamLess_ClientAudioProcessorEditor::buttonClicked (juce::Button* button)
{
    // SPECIAL BUTTONS === GRID & LAYOUT
    if (button->getComponentID() == "layout")
    {
        if (button->getButtonText() == "Studio") 
        {
            topView.changeLayout(true);
            buttonLayout.setButtonText("HuFo");
        } else {
            topView.changeLayout(false);
            buttonLayout.setButtonText("Studio");
        }
        
    } else
    if (button->getComponentID() == "grid")
    {
        if (button->getButtonText() == "Grid OFF")
        {
            topView.showGrid(true, true);
            buttonGrid.setButtonText("Grid \nON \nxyz");
        } 
        else if (button->getButtonText() == "Grid \nON \nxyz")
        {
            topView.showGrid(true, false);
            buttonGrid.setButtonText(juce::CharPointer_UTF8("Grid \nON \n r \xcf\x86 \xce\xb8\t"));
        } 
        else 
        {
            topView.showGrid(false, false);
            buttonGrid.setButtonText("Grid OFF");
        }
    } 
    else 
    { // ALL OTHER BUTTONS
        buttonSpherical.setColour(juce::TextButton::buttonColourId, seamlessBlue);
        buttonSend.setColour(juce::TextButton::buttonColourId, seamlessBlue);
        buttonSettings.setColour(juce::TextButton::buttonColourId, seamlessBlue);
        button->setColour(juce::TextButton::buttonColourId, juce::Colours::grey);
        
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
        } else
        if (button->getComponentID() == "spherical")
        {
            sphericalBox.setVisible(true);
        }
    }
}
