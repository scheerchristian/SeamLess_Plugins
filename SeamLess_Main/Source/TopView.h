/*
  ==============================================================================

    TopView.h
    Created: 23 Mar 2021 2:23:16pm
    Author:  anwaldt

  ==============================================================================
*/

#pragma once

class SoundSource;

#include <JuceHeader.h>
#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

#include "SoundSource.h"

//==============================================================================
/*
*/
class TopView  : public juce::Component, juce::Timer
{
public:
    TopView(SeamLess_MainAudioProcessor *p);
    ~TopView() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    
    /**
     Changes the displayed room layout in the Source Viewer.
     @param HuFoSelected true for Humbold Forum Layout, false for TU Studio
     */
    void changeLayout(bool HuFoSelected);
    
    /**
     Converts two coordinates in meters to the associated pixels in the Source Viewer
     @param xMeter x-Coordinate in meters
     @param yMeter y-Coordinate in meters
     */
    juce::Point<float> convertMeterToPixel(float xMeter, float yMeter);
    
    /**
     Converts two coordinates in pixels in the Source Viewer to the associated "real" coordinates in meters
     @param xPixel x-Coordinate in pixel
     @param yPixel y-Coordinate in pixel
     @see convertPixelToMeter
     */
    juce::Point<double> convertPixelToMeter(int xPixel, int yPixel);
    
    /**
     Moves a source in the Source Viewer
     @param sourceID SourceID of the source to be moved
     @param x new x-coordinate of the source
     @param y new y-coordinate of the source
     @param z new z-coordinate of the source
     @see convertMeterToPixel
     */
    void moveSource (int sourceID, float x, float y, float z);
    
    /**
     Reinitializes the Source Viewer. All sources get removed, and all connected sources are added afterwards. This helps to remove sources which aren't valid anymore.
     */
    void reInitViewer();
private:

    SeamLess_MainAudioProcessor *processor;
    SoundSource source;

    juce::Image background;

    juce::ComponentDragger dragger;

    bool isUpdating = true;

    juce::Path polygonPath;
    juce::Point<float> polygonMeter[34] = { juce::Point<float>(-6.051, 2.112), juce::Point<float>(-5.392, 2.593), juce::Point<float>(-4.695, 3.018), juce::Point<float>(-3.964, 3.382), juce::Point<float>(-3.205, 3.684), juce::Point<float>(-2.424, 3.921), juce::Point<float>(-1.625, 4.091), juce::Point<float>(-0.816, 4.194), juce::Point<float>(-0.0f, 4.228), juce::Point<float>(0.816, 4.194), juce::Point<float>(1.626, 4.091), juce::Point<float>(2.424, 3.921), juce::Point<float>(3.206, 3.684), juce::Point<float>(3.964, 3.382), juce::Point<float>(4.695, 3.017), juce::Point<float>(5.392, 2.593), juce::Point<float>(6.054, 2.107), juce::Point<float>(6.051, -2.112), juce::Point<float>(5.392, -2.593), juce::Point<float>(4.695, -3.018), juce::Point<float>(3.964, -3.382), juce::Point<float>(3.205, -3.684), juce::Point<float>(2.424, -3.921), juce::Point<float>(1.625, -4.091), juce::Point<float>(0.816, -4.194), juce::Point<float>(0.0f, -4.228), juce::Point<float>(-0.816, -4.194), juce::Point<float>(-1.626, -4.091), juce::Point<float>(-2.424	, -3.921), juce::Point<float>(-3.206, -3.684), juce::Point<float>(-3.964, -3.382), juce::Point<float>(-4.695, -3.017), juce::Point<float>(-5.392, -2.593), juce::Point<float>(-6.054, -2.107) };
    juce::Point<float> polygonPixel[34];
    
    juce::Path TUStudioPath;
    juce::Point<float> TUStudioMeter[8] = {
        juce::Point<float>(-1.620, 3.023), juce::Point<float>(1.620, 3.023), juce::Point<float>(2.430, 1.620), juce::Point<float>(2.430, -1.620),
        juce::Point<float>(1.620, -3.023), juce::Point<float>(-1.620, -3.023), juce::Point<float>(-2.430, -1.620), juce::Point<float>(-2.430,1.620) };
    juce::Point<float> TUStudioPixel[8];
    
    juce::String layout;
    std::vector<SoundSource*> sourceVector;
    std::vector<int> registeredSources;
    ///
    /// \brief timerCallback
    /// set source position
    /// scaled between 0 and 1
    virtual void timerCallback() override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TopView)

};
