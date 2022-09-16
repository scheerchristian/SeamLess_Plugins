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

  SoundSource(int ID, float x, float y, float z);

  ~SoundSource() override;

  void paint (juce::Graphics&) override;
  void resized() override;
  void mouseDown (const juce::MouseEvent& e) override;
  void mouseDrag (const juce::MouseEvent& e) override;
  void moveX(double x);
  void moveY(double y);

  /// moves the center of the elipse
  void moveXY(float x, float y);
  void moveXYZ(float x, float y, float z);
    juce::Point<float> getPosition();
    int getSourceWidth();
    void updateCoordinates(float x, float y, float z);
    int getSourceID();

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
