/*
==============================================================================

SoundSource.h
Created: 15 Feb 2021 4:05:52pm
Author:  anwaldt

==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../../Common/SeamLess.h"

//==============================================================================
/*
*/
class SoundSource  : public juce::Component
{
public:
    /** Creates a SoundSource.
        Its coordinates and ID get stored here as well.
     @param ID The ID of the source to be created
     @param x The inital x-position of the source
     @param y The inital y-position of the source
     @param z The inital z-position of the source
    */
    SoundSource(int ID, float x, float y, float z);
    ~SoundSource() override;

    void paint (juce::Graphics&) override;
    
    /**
     @returns the source ID of the SoundSource
     */
    int getSourceID();
    
    /**
     @returns Point of x and y position of the source.
     @see getSourceWidth
     */
    juce::Point<float> getPosition();
    
    /**
     @returns the width of the source, correlated to its z-position
     @see getPosition
     */
    int getSourceWidth();
    
    /**
     updates the coordinates which are stored in the SoundSource object.
     @param x x-coordinate of the new position
     @param y y-coordinate of the new position
     @param z z-coordinate of the new position
     */
    void updateCoordinates(float x, float y, float z);

private:

    juce::ComponentBoundsConstrainer constrainer;
    juce::ComponentDragger dragger;
    float xPos = 0;
    float yPos = 0;
    float sourceWidth = 1;
    float zToWidth(float z);
    int sourceID = 0;
  int width  = 30;
  int height = 30;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SoundSource)
};
