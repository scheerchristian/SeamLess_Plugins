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
    //idLabel.setText(juce::String(ID), juce::dontSendNotification);
    //addAndMakeVisible(idLabel);
}


SoundSource::~SoundSource()
{
}

void SoundSource::paint (juce::Graphics& g)
{
    if (sourceID > 0)
    {
  //auto area = getLocalBounds().reduced (2);
  g.setColour (juce::Colours::lightblue);
  g.fillEllipse (0, 0, getWidth(),getHeight());
    g.setColour(seamlessBlue);
    g.drawText(juce::String(sourceID), 0, 0, getWidth(), getHeight(), juce::Justification::centred);
    }
    
}

void SoundSource::resized ()
{
    
}


void SoundSource::mouseDown (const juce::MouseEvent& e)
{
}


void SoundSource::mouseDrag (const juce::MouseEvent& e)
{
}


void SoundSource::moveX(double x)
{
  setTopLeftPosition(x*10+300,this->getY());
}


void SoundSource::moveY(double y)
{
  setTopLeftPosition(this->getX(),y*10+400);
}


void SoundSource::moveXY(float x, float y)
{

    // repaint(xPos,yPos, 40,40);
    xPos = x*getWidth()  - (int) ((float) width/ 2.0);
    yPos = (1-y)*getHeight() - (int) ((float) height/2.0);

    // repaint(xPos,yPos, 40,40);
    repaint();

}


void SoundSource::moveXYZ(float x, float y, float z)
{

    // shift and enlarge area to repaint
    int shift = 5;
    int pad   = 10;

    // helper values for clearing the area at the old position
    // (maybe obsolete)
    int tmpWidth  = width;
    int tmpHeight = height;

    width  = 0;
    height = 0;

    repaint(xPos-shift,yPos-shift, tmpWidth+pad,tmpHeight+pad);


    xPos  = x*getWidth()      - (int) ((float) tmpWidth/ 2.0);
    yPos  = (1-y)*getHeight() - (int) ((float) tmpWidth/2.0);

    width  = 5+z*50;
    height = 5+z*50;

    // repaint(xPos,yPos, 40,40);
    repaint(xPos-shift,yPos-shift, width+pad,height+pad);

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
    return 2*z+30;
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
