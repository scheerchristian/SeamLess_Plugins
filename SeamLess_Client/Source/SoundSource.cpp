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
SoundSource::SoundSource()
{

    // this makes the component ignore mouse events
    // and they reach the parent component
    setInterceptsMouseClicks(false,false);

}


SoundSource::~SoundSource()
{
}

void SoundSource::paint (juce::Graphics& g)
{
  //auto area = getLocalBounds().reduced (2);
    if (yPos >= 0)      
        ballgrad = juce::ColourGradient(juce::Colours::whitesmoke, getWidth()*((-xPos + 10) / 20), getWidth() * ((yPos + 10) / 20), seamlessBlue, getWidth() / 2, 0, true);
    else
        ballgrad = juce::ColourGradient(juce::Colours::whitesmoke, getWidth()*((-xPos + 10) / 20), getWidth() * ((yPos + 10) / 20), seamlessBlue, getWidth() / 2, getWidth(), true);
    
    //g.setGradientFill(shadowgrad);
    //g.setOpacity(shadowOpacity);
    //g.fillEllipse (xPos+(width-shadowWidth)/2+5, yPos+(width-shadowWidth)/2+5, shadowWidth,shadowWidth);
    g.setGradientFill(ballgrad);
    g.fillEllipse (0, 0, getWidth(), getWidth());
}


void SoundSource::setXPos(float newValue) { xPos = newValue;    }
void SoundSource::setYPos(float newValue) { yPos = newValue;    }
