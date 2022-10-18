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

/*

void SoundSource::mouseDrag (const juce::MouseEvent& e){}

void SoundSource::moveX(double x){}

void SoundSource::moveY(double y){}

void SoundSource::moveXY(float x, float y){}

void SoundSource::moveXYZ(float x, float y, float z)
{
    xPos  = x*getWidth()      - (int) ((float) width/2.0);
    yPos  = (1-y)*getHeight() - (int) ((float) width/2.0);
    
    width = getWidth()*0.0015*(20+z*15);
    shadowWidth  = width*1.2*(z+1);
    shadowOpacity = 1-(float)z*0.7;
    repaint();
   


    //this->setBounds(xPos, yPos, width, height);
}
*/
