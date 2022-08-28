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

TopView::TopView(SeamLess_ClientAudioProcessor *p): source()
{

    processor  = p;
    background = juce::ImageCache::getFromMemory (BinaryData::top_view_png, BinaryData::top_view_pngSize);

    addAndMakeVisible(source);
    startTimer(50);
    
}


TopView::~TopView()
{

}


void TopView::paint (juce::Graphics& g)
{
    
    g.setColour(seamlessBlue);
    g.fillRoundedRectangle(0,0,getWidth(),getHeight(),30);

    g.setColour(juce::Colour (220,220,220));
    g.fillRoundedRectangle(5,5,getWidth()-10,getHeight()-10,25);

    //g.setColour(seamlessBlue);
    //g.drawRoundedRectangle(0,0,getWidth(),getHeight(),30,10);
    if (enableGrid == true){
        g.setColour(juce::Colours::grey);
         if (grid == "spherical"){
            for (int i = 1; i <= 13; i++) {
                g.drawRoundedRectangle(juce::Rectangle<float>(convertMeterToPixel(10-i, 10-i), convertMeterToPixel(10+i, 10+i)), 100*i, 1);
            }
             g.drawLine(juce::Line<float>(convertMeterToPixel(10, 0), convertMeterToPixel(10, 20)), 0.8f);
             g.drawLine(juce::Line<float>(convertMeterToPixel(0, 10), convertMeterToPixel(20, 10)), 0.8f);
             
             g.drawLine(juce::Line<float>(convertMeterToPixel(4.23, 20), convertMeterToPixel(15.77, 0)), 0.8f);
             g.drawLine(juce::Line<float>(convertMeterToPixel(4.23, 0), convertMeterToPixel(15.77, 20)), 0.8f);
             
             g.drawLine(juce::Line<float>(convertMeterToPixel(0, 4.23), convertMeterToPixel(20, 15.77)), 0.8f);
             g.drawLine(juce::Line<float>(convertMeterToPixel(20, 4.23), convertMeterToPixel(0, 15.77)), 0.8f);
        } else {
            for (int i = 1; i<=19; i++) {
                g.drawLine(juce::Line<float>(convertMeterToPixel(0, i), convertMeterToPixel(20, i)));
                g.drawLine(juce::Line<float>(convertMeterToPixel(i, 0), convertMeterToPixel(i, 20)));
            }
        }
    g.setColour(seamlessBlue);
        
    g.drawRoundedRectangle(juce::Rectangle<float>(juce::Point<float>(3,3), juce::Point<float>(getWidth()-3,getHeight()-3)),25,5);
    }
    g.setColour(seamlessBlue);
    g.strokePath(polygonPath, juce::PathStrokeType(5.0f));
    g.strokePath(TUStudioPath, juce::PathStrokeType(5.0f));
   
    
    
    //g.drawLine(juce::Line<float>(convertMeterToPixel(3.5, 10), convertMeterToPixel(16.5, 10)));
    // g.drawImageAt(background.rescaled(700,400,juce::Graphics::mediumResamplingQuality), 0, 0);
}

void TopView::resized()
{
    source.setBounds(0,0,getWidth(),getHeight() );
    polygonPath.clear();
    TUStudioPath.clear();
    polygonPixel[0] = convertMeterToPixel(polygonMeter[0].getX() + 10, polygonMeter[0].getY() + 10);
    polygonPath.startNewSubPath(polygonPixel[0]);
    for (int i = 1; i <= 33; i++)
    {
        polygonPixel[i] = convertMeterToPixel(polygonMeter[i].getX() + 10, polygonMeter[i].getY() + 10);
        polygonPath.lineTo(polygonPixel[i]);
    }
    polygonPath.closeSubPath();
    if (layout == "Studio") {
        polygonPath.clear();
        TUStudioPath.clear();
        TUStudioPixel[0] = convertMeterToPixel(TUStudioMeter[0].getX() + 10, TUStudioMeter[0].getY() + 10);
        TUStudioPath.startNewSubPath(TUStudioPixel[0]);
        for (int i = 1; i <= 3; i++)
        {
            TUStudioPixel[i] = convertMeterToPixel(TUStudioMeter[i].getX() + 10, TUStudioMeter[i].getY() + 10);
            TUStudioPath.lineTo(TUStudioPixel[i]);
        }
        TUStudioPath.closeSubPath();
    } 
}

void TopView::mouseDown(const juce::MouseEvent& e)
{
    changePosition(e.getPosition());
}


void TopView::mouseDrag (const juce::MouseEvent& e)
{
    changePosition(e.getPosition());
}

void TopView::changePosition(juce::Point <int> p)
{

    int x = p.getX();
    int y = p.getY()+0.5*getHeight();

    float xPos = -20*(0.5-(float) x / (float) getWidth());
    float yPos =  20*(0.5-(float) y / (float) getHeight())+10;

    processor->setXPos(xPos);
    processor->setYPos(yPos);
}


void TopView::mouseUp(const juce::MouseEvent& e)
{
}

juce::Point<float> TopView::convertMeterToPixel(float xMeter, float yMeter)
{
    float xPixel = getLocalBounds().getWidth() * (xMeter/20);
    float yPixel = getLocalBounds().getHeight() * (yMeter/20);
    return juce::Point<float>(xPixel, yPixel);
}

juce::Point<double> TopView::convertPixelToMeter(int xPixel, int yPixel)
{
    double xMeter = (xPixel / getLocalBounds().getWidth()) * 20;
    double yMeter = (yPixel / getLocalBounds().getHeight()) * 20;
    return juce::Point<double>(xMeter, yMeter);
}


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

void TopView::changeLayout(bool HuFoSelected) {
    if (HuFoSelected == true) {layout = "HuFo";} else {layout = "Studio";}
}

void TopView::changeGrid(bool xyzGrid) {
    if (xyzGrid == true) {grid = "xyz";} else {grid = "spherical";}
    
}
void TopView::showGrid(bool showGrid) {
    if (showGrid == true) {enableGrid = true;} else {enableGrid = false;};
    resized();
}
