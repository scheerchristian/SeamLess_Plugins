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

  SoundSource();

  ~SoundSource() override;

  void paint (juce::Graphics&) override;
  void resized() override;
  void mouseDown (const juce::MouseEvent& e) override;
  void mouseDrag (const juce::MouseEvent& e) override;

  void setXPos(float newValue);
  void setYPos(float newValue);
  /*
  void moveX(double x);
  void moveY(double y);

  /// moves the center of the elipse
  void moveXY(float x, float y);
  void moveXYZ(float x, float y, float z);
  */
private:

  juce::ComponentBoundsConstrainer constrainer;
  juce::ComponentDragger dragger;
  juce::ColourGradient ballgrad;
  
  float xPos;
  float yPos;
/*
    int width;//  = 30;
    int shadowWidth = 40;
*/
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SoundSource)
};
