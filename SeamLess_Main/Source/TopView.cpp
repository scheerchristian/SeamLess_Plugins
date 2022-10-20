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

TopView::TopView(SeamLess_MainAudioProcessor *p): source(0,0,0,0)
{

    processor  = p;
    background = juce::ImageCache::getFromMemory (BinaryData::top_view_png, BinaryData::top_view_pngSize);
    
}


TopView::~TopView()
{

}


void TopView::paint (juce::Graphics& g)
{
    g.setColour(seamlessLightGrey);
    g.fillRoundedRectangle(0,0,getWidth(),getHeight(),getWidth()*0.08);

    //g.setColour(seamlessBlue);
    //g.drawRoundedRectangle(0,0,getWidth(),getHeight(),30,15);


    g.setColour(seamlessBlue);
    g.strokePath(polygonPath, juce::PathStrokeType(getWidth()*0.01));
    if (layout == "Studio") {g.strokePath(TUStudioPath, juce::PathStrokeType(getWidth()*0.01));}
    //g.drawLine(juce::Line<float>(convertMeterToPixel(3.5, 10), convertMeterToPixel(16.5, 10)));
    // g.drawImageAt(background.rescaled(700,400,juce::Graphics::mediumResamplingQuality), 0, 0);
}

void TopView::resized()
{
    if (sourceVector.size() != 0) {
        for(auto const& s: sourceVector) {
            s->setBounds(convertMeterToPixel(s->getPosition().getX()+10,0).getX()-(s->getSourceWidth())/2, convertMeterToPixel(0,-s->getPosition().getY()+10).getY()-(s->getSourceWidth())/2, (s->getSourceWidth()), (s->getSourceWidth()));
        }
    }
    polygonPath.clear();
    TUStudioPath.clear();
    polygonPixel[0] = convertMeterToPixel(polygonMeter[0].getX() + 10, polygonMeter[0].getY()+ 10);
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
        for (int i = 1; i <= 7; i++)
        {
            TUStudioPixel[i] = convertMeterToPixel(TUStudioMeter[i].getX() + 10, TUStudioMeter[i].getY() + 10);
            TUStudioPath.lineTo(TUStudioPixel[i]);
        }
        TUStudioPath.closeSubPath();
    }

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

void TopView::changeLayout(bool HuFoSelected)
{
    if (HuFoSelected == true)
        layout = "HuFo";
    else
        layout = "Studio";
    repaint();
    resized();
}

void TopView::moveSource (int sourceID, float x, float y, float z)
{
    auto source = std::find(registeredSources.begin(), registeredSources.end(), sourceID);
    
    if (source != registeredSources.end())
    {
        // only move already registered source
        auto s = sourceVector[source - registeredSources.begin()];
        s->updateCoordinates(x, y, z);
    }
    else
    {
        // add new sourceID if not yet registered
        SoundSource* s = new SoundSource(sourceID, x,y,z);
        addAndMakeVisible(s);
        s->setComponentID("source"+juce::String(sourceID));
        sourceVector.push_back(s);
        registeredSources.push_back(sourceID);
    }
    resized();
}
void TopView::reInitViewer()
{
    removeAllChildren();
    deleteAllChildren();
    sourceVector.clear();
    registeredSources.clear();
}
