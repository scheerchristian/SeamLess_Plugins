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
    : AudioProcessorEditor (&p), audioProcessor (p), oscConnectionBox(p), reverbFaderBox(p,apvts), connectionComponent(&p)
{
    setSize (1000,400);
    setResizable(true, true);
    addAndMakeVisible(oscConnectionBox);

    addAndMakeVisible(reverbFaderBox);
    addAndMakeVisible(connectionComponent);
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
    g.drawText ("SeamLess Main Plugin", 20, 20, 200, 20,
    juce::Justification::left, true);
}


void SeamLess_MainAudioProcessorEditor::resized()
{
    auto box = getLocalBounds().reduced(40, 40);
    auto r = box;
    auto reverbFaderSection = r.removeFromRight(r.getWidth() * 4 / 5);
    reverbFaderBox.setBounds(reverbFaderSection);

    auto oscConnectionSection = r.removeFromTop(r.getHeight() / 4);
    oscConnectionSection = oscConnectionSection.removeFromLeft(300);
    oscConnectionBox.setBounds(oscConnectionSection);

    r.removeFromTop(box.getHeight() / 6);

    auto connectionSection = r.removeFromTop(r.getHeight() / 4);
    connectionSection = connectionSection.removeFromLeft(300);
    connectionComponent.setBounds(connectionSection);
}


void SeamLess_MainAudioProcessorEditor::timerCallback()
{

}
