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
class TopView  : public juce::Component
{
public:
    /**
     @brief Creates a TopView object.*/
    TopView(SeamLess_ClientAudioProcessor *p, juce::AudioProcessorValueTreeState& apvts);
    /** @cond PRIVATE */
    ~TopView() override;

    std::unique_ptr<juce::ParameterAttachment> xAttachment;
    std::unique_ptr<juce::ParameterAttachment> yAttachment;


    void paint (juce::Graphics&) override;
    
    /**
     All elements belonging to the TopView component are placed onto the canvas here. Relative coordinates are needed for scalability.
     */
    void resized() override;
    /** @endcond */
    
    /**
     @brief Triggered when mouse click is registered.
     
     Used to start a gesture and show the label displaying the coordinates.
     @see mouseDrag
     @see mouseUp
     */
    void mouseDown(const juce::MouseEvent& e) override;

    /**
     @brief Triggered when mouse is dragged after a click.
     
     Used to record a gesture and update the label displaying the coordinates
     @see mouseDown
     @see mouseUp
     */
    void mouseDrag(const juce::MouseEvent& e) override;

    /**
     @brief Triggered when mouse click is finished.
     
     Used to stop the ongoing gesture and hide the label displaying the coordinates.
     @see mouseDown
     @see mouseDrag
     */
    void mouseUp(const juce::MouseEvent& e) override;
    
    void connectXtoParameter(juce::RangedAudioParameter& p);
    void connectYtoParameter(juce::RangedAudioParameter& p);

    /**
     Converts two coordinates in meters to the associated pixels in the Source Viewer.
     @param xMeter x-Coordinate in meters
     @param yMeter y-Coordinate in meters
     @see convertPixelToMeter
     */
    juce::Point<float> convertMeterToPixel(float xMeter, float yMeter);
    
    /**
     Converts two coordinates in pixels in the Source Viewer to the associated "real" coordinates in meters.
     @param xPixel x-Coordinate in pixel
     @param yPixel y-Coordinate in pixel
     @see convertMeterToPixel
     */
    juce::Point<double> convertPixelToMeter(int xPixel, int yPixel);

    // setter and getter functions
    /**
     Sets the source's width.
     @param newWidth width in pixel
     @see setZPos
     */
    void setSourceWidthPx(int newWidth);
    
    /**
     Sets the source's z position.
     @param newValue z position in meter
     @see setSourceWidthPx
     */
    void setZPos(float newValue);
    
    /**
     Changes the displayed room layout.
     @param HuFoSelected true for Humbold Forum Layout, false for TU Studio
     */
    void changeLayout(bool HuFoSelected);
    
    /**
     Enables or disables the cartesian or spherical grid.
     
     @param showGrid must be true for grids to be displayed
     @param xyzGrid true for cartesian grid, false for spherical grid
     */
    void showGrid(bool showGrid, bool xyzGrid);
    
private:

    SeamLess_ClientAudioProcessor *processor;
    juce::AudioProcessorValueTreeState& treeState;
    SoundSource source;

    juce::Image background;

    juce::ComponentDragger dragger;

    bool isUpdating = true;

    //Path for HU-Forum graphical shape
    juce::Path polygonPath;
    juce::Point<float> polygonMeter[34] = { juce::Point<float>(-6.051, 2.112), juce::Point<float>(-5.392, 2.593), juce::Point<float>(-4.695, 3.018), juce::Point<float>(-3.964, 3.382), juce::Point<float>(-3.205, 3.684), juce::Point<float>(-2.424, 3.921), juce::Point<float>(-1.625, 4.091), juce::Point<float>(-0.816, 4.194), juce::Point<float>(-0.0f, 4.228), juce::Point<float>(0.816, 4.194), juce::Point<float>(1.626, 4.091), juce::Point<float>(2.424, 3.921), juce::Point<float>(3.206, 3.684), juce::Point<float>(3.964, 3.382), juce::Point<float>(4.695, 3.017), juce::Point<float>(5.392, 2.593), juce::Point<float>(6.054, 2.107), juce::Point<float>(6.051, -2.112), juce::Point<float>(5.392, -2.593), juce::Point<float>(4.695, -3.018), juce::Point<float>(3.964, -3.382), juce::Point<float>(3.205, -3.684), juce::Point<float>(2.424, -3.921), juce::Point<float>(1.625, -4.091), juce::Point<float>(0.816, -4.194), juce::Point<float>(0.0f, -4.228), juce::Point<float>(-0.816, -4.194), juce::Point<float>(-1.626, -4.091), juce::Point<float>(-2.424	, -3.921), juce::Point<float>(-3.206, -3.684), juce::Point<float>(-3.964, -3.382), juce::Point<float>(-4.695, -3.017), juce::Point<float>(-5.392, -2.593), juce::Point<float>(-6.054, -2.107) };
    juce::Point<float> polygonPixel[34];
        
    juce::Path TUStudioPath;
    juce::Point<float> TUStudioMeter[8] = {
        juce::Point<float>(-1.620, 3.023), juce::Point<float>(1.620, 3.023), juce::Point<float>(2.430, 1.620), juce::Point<float>(2.430, -1.620),
        juce::Point<float>(1.620, -3.023), juce::Point<float>(-1.620, -3.023), juce::Point<float>(-2.430, -1.620), juce::Point<float>(-2.430,1.620)
    };
    
    juce::Path shadowOrigin;
    juce::DropShadow sourceShadow;


    juce::Point<float> TUStudioPixel[8];
    
    juce::String layout;
    juce::String grid;

    float xPos;
    float yPos;
    float zPos;

    int xPosPx;
    int yPosPx;
    int zPosPx;

    int sourceWidthPx;

    
    bool enableGrid;
    juce::Label coordinatesLabel;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TopView)

};
