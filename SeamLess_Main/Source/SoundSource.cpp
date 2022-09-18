/*
==============================================================================

SoundSource.cpp
Created: 15 Feb 2021 4:05:52pm
Author:  anwaldt

==============================================================================
*/

#include <JuceHeader.h>
#include "SoundSource.h"

//==============================================================================
SoundSource::SoundSource(int ID, float x, float y, float z)
{
    
    updateCoordinates(x, y, z);
    sourceID = ID;
    // this makes the component ignore mouse events
    // and they reach the parent component
    setInterceptsMouseClicks(false,false);
    
}


SoundSource::~SoundSource()
{
}

void SoundSource::paint (juce::Graphics& g)
{
    if (sourceID > 0)
    {
        g.setColour(seamlessLightGrey);
        g.fillEllipse (0, 0, getWidth(),getHeight());
        g.setColour (juce::Colours::lightblue);
        g.fillEllipse(getWidth()*0.1, getWidth()*0.1, getWidth()*0.8, getHeight()*0.8);
        g.setColour(seamlessBlue);
        g.drawText(juce::String(sourceID), 0, 0, getWidth(), getHeight(), juce::Justification::centred);
    }
    
}

juce::Point<float> SoundSource::getPosition()
{
    return juce::Point<float>(xPos, yPos);
}
int SoundSource::getSourceWidth()
{
    return sourceWidth;
}

float SoundSource::zToWidth(float z){
    return 30+z;
}

void SoundSource::updateCoordinates(float x, float y, float z)
{
    xPos = x;
    yPos = y;
    sourceWidth = zToWidth(z);
}

int SoundSource::getSourceID()
{
    return sourceID;
}
