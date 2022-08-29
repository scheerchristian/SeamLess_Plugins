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
    
    auto ballgrad = juce::ColourGradient(juce::Colours::whitesmoke, xPos+width/3, yPos+width/3, juce::Colours::black, xPos+width, yPos+width, true);
    auto shadowgrad = juce::ColourGradient(juce::Colours::darkgrey, xPos+(width-shadowWidth)/2+shadowWidth/2+7, yPos+(width-shadowWidth)/2+shadowWidth/2+7, juce::Colours::white, xPos+(width-shadowWidth)/2+shadowWidth+7, yPos+(width-shadowWidth)/2+shadowWidth+7, true);
    
    g.setColour (juce::Colours::silver);
    g.setGradientFill(shadowgrad);
    g.setOpacity(shadowOpacity);
    g.fillEllipse (xPos+(width-shadowWidth)/2+5, yPos+(width-shadowWidth)/2+5, shadowWidth,shadowWidth);
    g.setColour (juce::Colours::lightblue);
    g.setGradientFill(ballgrad);
    g.fillEllipse (xPos, yPos, width,width);
}

void SoundSource::resized (){
    
}


void SoundSource::mouseDown (const juce::MouseEvent& e){}

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
}
