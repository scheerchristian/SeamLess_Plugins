/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "../../Common/SeamLess.h"

//==============================================================================
SeamLess_MainAudioProcessorEditor::SeamLess_MainAudioProcessorEditor (SeamLess_MainAudioProcessor& p, juce::AudioProcessorValueTreeState& apvts)
    : AudioProcessorEditor (&p), audioProcessor (p), oscConnectionBox(p), reverbFaderBox(p,apvts), connectionComponent(&p), sourceViewer(&p)
{
    setSize (930,520);
    setResizable(true, true);
    setResizeLimits(930, 260, 2000, 1000);
    addAndMakeVisible(oscConnectionBox);

    addAndMakeVisible(reverbFaderBox);
    addAndMakeVisible(connectionComponent);
    addAndMakeVisible(sourceViewer);
    addAndMakeVisible(layoutButton);
    layoutButton.setButtonText("HuFo");
    layoutButton.onClick = [this] {
        if (layoutButton.getButtonText() == "HuFo")
        {
            sourceViewer.changeLayout(false);
            layoutButton.setButtonText("Studio");
        }
        else
        {
            sourceViewer.changeLayout(true);
            layoutButton.setButtonText("HuFo");
        }
    };
    addAndMakeVisible(fullSizeSourceViewButton);
    fullSizeSourceViewButton.setButtonText("bigg");
    fullSizeSourceViewButton.onClick = [this] {
        fullScreen = !fullScreen;
        resized();
    };
    startTimer(100);
}

SeamLess_MainAudioProcessorEditor::~SeamLess_MainAudioProcessorEditor()
{
}

//==============================================================================
void SeamLess_MainAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.setColour(seamlessBlue);
    g.fillRoundedRectangle(getLocalBounds().toFloat(), 20);

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("SeamLess Main Plugin", 10, 0, getWidth()*0.25, 40,
    juce::Justification::centred, true);
}


void SeamLess_MainAudioProcessorEditor::resized()
{
    const auto box = getLocalBounds().reduced(20, 20);

    auto r = box;
    r.removeFromTop(30);
    auto reverbFaderSection = r.removeFromRight(box.getWidth() * 2 / 3);
    reverbFaderBox.setBounds(reverbFaderSection);

    auto oscConnectionSection = r.removeFromTop(95);
    oscConnectionSection = oscConnectionSection.removeFromLeft(270);
    oscConnectionBox.setBounds(oscConnectionSection);

    r.removeFromTop(10);

    auto connectionSection = r.removeFromTop(95);
    connectionSection = connectionSection.removeFromLeft(270);
    connectionComponent.setBounds(connectionSection);
    
    r.removeFromTop(10);

    if (r.getHeight() <= r.getWidth())
        sourceViewer.setBounds(15, r.getY(), r.getHeight(), r.getHeight() + 5);
    else
        sourceViewer.setBounds(15, r.getY(), r.getWidth(), r.getWidth() + 5);
    
    oscConnectionBox.setBounds(10, 40, getWidth()*0.25, 60);
    connectionComponent.setBounds(10, 110, getWidth()*0.25, 100);
    reverbFaderBox.setBounds(getWidth()*0.275, 10, getWidth()*0.72, getHeight()-20);
    const int &maxSourceViewerSize = std::min<int>((getWidth()*0.25+10), getHeight()-oscConnectionBox.getHeight()-connectionComponent.getHeight()-85);
    if (fullScreen == false)
    {
        sourceViewer.setBounds(5, getHeight()-maxSourceViewerSize-10, maxSourceViewerSize, maxSourceViewerSize);
        layoutButton.setVisible(false);
        reverbFaderBox.setVisible(true);
        fullSizeSourceViewButton.setButtonText("bigg");
        
    }
    else
    {
        const int &maxSourceViewerSize = std::min<int>(getWidth(), getHeight());
        sourceViewer.setBounds(reverbFaderBox.getBounds().getX(), 0, maxSourceViewerSize, maxSourceViewerSize);
        layoutButton.setBounds(fullSizeSourceViewButton.getBounds().getX(), fullSizeSourceViewButton.getBounds().getY()+30, 60, 60);
        layoutButton.setVisible(true);
        reverbFaderBox.setVisible(false);
        fullSizeSourceViewButton.setButtonText("smol");
        
    }
    fullSizeSourceViewButton.setBounds(10, getHeight()-maxSourceViewerSize-30, 60, 20);
    
    
}


void SeamLess_MainAudioProcessorEditor::timerCallback()
{
    juce::StringArray msgtokens = audioProcessor.getIncomingMessages();
    sourceViewer.moveSource(msgtokens[0].getIntValue(), msgtokens[1].getFloatValue(), msgtokens[2].getFloatValue(), msgtokens[3].getFloatValue());
}
