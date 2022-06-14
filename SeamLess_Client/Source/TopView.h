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
    TopView(SeamLess_ClientAudioProcessor *p);
    ~TopView() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    /// \brief mouseDown
    /// \param e
    ///
    /// Calls changePosition() when the mouse is dragged.
    void mouseDown(const juce::MouseEvent& e) override;

    /// \brief mouseDrag
    /// \param e
    ///
    /// Calls changePosition() when the mouse is dragged.
    void mouseDrag(const juce::MouseEvent& e) override;

    /// \brief changePosition
    /// \param p
    /// Sets the X and Y position parameters of the
    /// processor.
    void changePosition(juce::Point <int> p);

    void mouseUp(const juce::MouseEvent& e) override;


    juce::Point<float> convertMeterToPixel(float xMeter, float yMeter);
    juce::Point<double> convertPixelToMeter(int xPixel, int yPixel);

private:

    SeamLess_ClientAudioProcessor *processor;
    SoundSource source;

    juce::Image background;

    juce::ComponentDragger dragger;

    bool isUpdating = true;

    juce::Path polygonPath;
    juce::Point<float> polygonMeter[34] = { juce::Point<float>(-6.051, 2.112), juce::Point<float>(-5.392, 2.593), juce::Point<float>(-4.695, 3.018), juce::Point<float>(-3.964, 3.382), juce::Point<float>(-3.205, 3.684), juce::Point<float>(-2.424, 3.921), juce::Point<float>(-1.625, 4.091), juce::Point<float>(-0.816, 4.194), juce::Point<float>(-0.0f, 4.228), juce::Point<float>(0.816, 4.194), juce::Point<float>(1.626, 4.091), juce::Point<float>(2.424, 3.921), juce::Point<float>(3.206, 3.684), juce::Point<float>(3.964, 3.382), juce::Point<float>(4.695, 3.017), juce::Point<float>(5.392, 2.593), juce::Point<float>(6.054, 2.107), juce::Point<float>(6.051, -2.112), juce::Point<float>(5.392, -2.593), juce::Point<float>(4.695, -3.018), juce::Point<float>(3.964, -3.382), juce::Point<float>(3.205, -3.684), juce::Point<float>(2.424, -3.921), juce::Point<float>(1.625, -4.091), juce::Point<float>(0.816, -4.194), juce::Point<float>(0.0f, -4.228), juce::Point<float>(-0.816, -4.194), juce::Point<float>(-1.626, -4.091), juce::Point<float>(-2.424	, -3.921), juce::Point<float>(-3.206, -3.684), juce::Point<float>(-3.964, -3.382), juce::Point<float>(-4.695, -3.017), juce::Point<float>(-5.392, -2.593), juce::Point<float>(-6.054, -2.107) };
    juce::Point<float> polygonPixel[34];
    ///
    /// \brief timerCallback
    /// set source position
    /// scaled between 0 and 1
    virtual void timerCallback() override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TopView)

};
