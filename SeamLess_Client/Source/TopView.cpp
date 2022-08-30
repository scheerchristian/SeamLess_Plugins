/*
  ==============================================================================

    TopView.cpp
    Created: 23 Mar 2021 2:23:16pm
    Author:  anwaldt

  ==============================================================================
*/

#include <JuceHeader.h>
#include "TopView.h"

//==============================================================================

TopView::TopView(SeamLess_ClientAudioProcessor *p, juce::AudioProcessorValueTreeState& apvts): source(), treeState(apvts)
{

    processor  = p;
    background = juce::ImageCache::getFromMemory (BinaryData::top_view_png, BinaryData::top_view_pngSize);

    addAndMakeVisible(source);
    // startTimer(50); // timer no longer needed because off parameter attachments
}


TopView::~TopView()
{

}

void TopView::paint (juce::Graphics& g)
{
    g.setColour(juce::Colour (220,220,220));
    g.fillRoundedRectangle(0,0,getWidth(),getHeight(),30);

    g.setColour(seamlessBlue);
    g.drawRoundedRectangle(0,0,getWidth(),getHeight(),30,15);


    g.setColour(seamlessBlue);
    g.strokePath(polygonPath, juce::PathStrokeType(5.0f));
}

void TopView::resized()
{
    source.setBounds(xPosPx - sourceWidthPx/2, yPosPx - sourceWidthPx/2, sourceWidthPx, sourceWidthPx);

    // draw the HUFO-Shape 
    polygonPath.clear();
    polygonPixel[0] = convertMeterToPixel(polygonMeter[0].getX(), polygonMeter[0].getY());
    polygonPath.startNewSubPath(polygonPixel[0]);
    for (int i = 1; i <= 33; i++)
    {
        polygonPixel[i] = convertMeterToPixel(polygonMeter[i].getX(), polygonMeter[i].getY());
        polygonPath.lineTo(polygonPixel[i]);
    }
    polygonPath.closeSubPath();
    repaint();
}

void TopView::mouseDown(const juce::MouseEvent& e)
{
    auto pos = convertPixelToMeter(e.x, e.y);
    xAttachment->setValueAsCompleteGesture(pos.x);
    yAttachment->setValueAsCompleteGesture(pos.y);

    xAttachment->beginGesture();
    yAttachment->beginGesture();
}


void TopView::mouseDrag (const juce::MouseEvent& e)
{
    auto pos = convertPixelToMeter(e.x, e.y);
    xAttachment->setValueAsPartOfGesture(pos.x);
    yAttachment->setValueAsPartOfGesture(pos.y);
}


void TopView::mouseUp(const juce::MouseEvent& e)
{
    xAttachment->endGesture();
    yAttachment->endGesture();
}

void TopView::connectXtoParameter(juce::RangedAudioParameter& p)
{
    xAttachment = std::make_unique<juce::ParameterAttachment>(p, [this](float newValue) 
        {
            xPos = newValue;
            auto pos = convertMeterToPixel(newValue, yPos).toInt();
            xPosPx = pos.x;
            resized();
        });
    xAttachment->sendInitialUpdate();
}

void TopView::connectYtoParameter(juce::RangedAudioParameter& p)
{
    yAttachment = std::make_unique<juce::ParameterAttachment>(p, [this](float newValue)
        {
            yPos = newValue;
            auto pos = convertMeterToPixel(newValue, yPos).toInt();
            yPosPx = pos.y;
            resized();
        });
    yAttachment->sendInitialUpdate();
}


juce::Point<float> TopView::convertMeterToPixel(float xMeter, float yMeter)
{
    float xPixel = getLocalBounds().getWidth() * ((xMeter + 10)/20);
    float yPixel = getLocalBounds().getHeight() * ((yMeter + 10)/20);
    return juce::Point<float>(xPixel, yPixel);
}

juce::Point<double> TopView::convertPixelToMeter(int xPixel, int yPixel)
{
    auto sad = getLocalBounds().getWidth();
    double xMeter = (double(xPixel) / getLocalBounds().getWidth()) * 20 - 10;
    double yMeter = (double(yPixel) / getLocalBounds().getHeight()) * 20 - 10;
    return juce::Point<double>(xMeter, yMeter);
}

void TopView::setSourceWidthPx(int newWidth)
{
    sourceWidthPx = newWidth;
}

// timer no longer needed because of parameter attachments
/*
void TopView::timerCallback()
{
    if(isUpdating == true)
    {
        /// @todo: scaling is hard coded
        float x = processor->getXPos() / 20.0 + 0.5;
        float y = processor->getYPos() / 20.0 + 0.5;
        float z = processor->getZPos() / 20.0 + 0.5;
        source.moveXYZ(x,y,z);
    }
}
*/
