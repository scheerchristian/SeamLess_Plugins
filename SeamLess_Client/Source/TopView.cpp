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
    addAndMakeVisible(coordinatesLabel);
    coordinatesLabel.setText("", juce::dontSendNotification);
    coordinatesLabel.setColour(juce::Label::textColourId, juce::Colours::darkgrey);
    coordinatesLabel.setVisible(false);
    coordinatesLabel.setFont(juce::Font(juce::Font::getDefaultMonospacedFontName(), 14.0f, 0));
}


TopView::~TopView()
{

}

void TopView::paint (juce::Graphics& g)
{
    
    g.setColour(seamlessBlue);
    g.fillRoundedRectangle(0,0,getWidth(),getHeight(),30);

    g.setColour(seamlessLightGrey);
    g.fillRoundedRectangle(5,5,getWidth()-10,getHeight()-10,25);

    //g.setColour(seamlessBlue);
    //g.drawRoundedRectangle(0,0,getWidth(),getHeight(),30,10);
    if (enableGrid == true){
        g.setColour(juce::Colours::silver);
         if (grid == "spherical")
         {
             
            for (int i = 1; i <= 13; i++) 
            {
                g.drawRoundedRectangle(juce::Rectangle<float>(convertMeterToPixel(-i, -i), convertMeterToPixel(i, i)), 100 * (i), 1);
            }
            
             g.drawLine(juce::Line<float>(convertMeterToPixel(0, 10), convertMeterToPixel(0, -10)), 0.8f);
             g.drawLine(juce::Line<float>(convertMeterToPixel(-10, 0), convertMeterToPixel(10, 0)), 0.8f);
             g.drawLine(juce::Line<float>(convertMeterToPixel(4.2264973081 - 10, 10), convertMeterToPixel(5.7735026919, -10)), 0.8f);
             g.drawLine(juce::Line<float>(convertMeterToPixel(4.2264973081 - 10, -10), convertMeterToPixel(5.7735026919, 10)), 0.8f);
             g.drawLine(juce::Line<float>(convertMeterToPixel(-10, 4.2264973081 - 10), convertMeterToPixel(10, 5.7735026919)), 0.8f);
             g.drawLine(juce::Line<float>(convertMeterToPixel(10, 4.2264973081 - 10), convertMeterToPixel(-10, 5.7735026919)), 0.8f);
         }
         else 
         {
            for (int i = -9; i<=9; i++) 
            {
                g.drawLine(juce::Line<float>(convertMeterToPixel(-10, i), convertMeterToPixel(10, i)));
                g.drawLine(juce::Line<float>(convertMeterToPixel(i, -10), convertMeterToPixel(i, 10)));
            }
        }
    g.setColour(seamlessBlue);
        
    g.drawRoundedRectangle(juce::Rectangle<float>(juce::Point<float>(3,3), juce::Point<float>(getWidth()-3,getHeight()-3)),25,5);
    }
    g.setColour(seamlessBlue);
    g.strokePath(polygonPath, juce::PathStrokeType(5.0f));
    if (layout == "Studio") {g.strokePath(TUStudioPath, juce::PathStrokeType(5.0f));}

    // draws the knob shadow
    g.setColour(juce::Colours::darkgrey);
    shadowOrigin.addEllipse((convertMeterToPixel(xPos,yPos).getX() - sourceWidthPx/2) + xPos - zPos/2,
                            (convertMeterToPixel(xPos,yPos).getY() - sourceWidthPx/2) - yPos - zPos/2,
                            sourceWidthPx + zPos,
                            sourceWidthPx + zPos);
    sourceShadow.drawForPath(g, shadowOrigin);
    shadowOrigin.clear();
}

void TopView::resized()
{
    source.setBounds(convertMeterToPixel(xPos,yPos).getX()-sourceWidthPx/2, convertMeterToPixel(xPos,yPos).getY()-sourceWidthPx/2, sourceWidthPx, sourceWidthPx);

    // draw the HUFO-Shape 
    polygonPath.clear();
    TUStudioPath.clear();
    polygonPixel[0] = convertMeterToPixel(polygonMeter[0].getX(), polygonMeter[0].getY());
    polygonPath.startNewSubPath(polygonPixel[0]);
    for (int i = 1; i <= 33; i++)
    {
        polygonPixel[i] = convertMeterToPixel(polygonMeter[i].getX(), polygonMeter[i].getY());
        polygonPath.lineTo(polygonPixel[i]);
    }
    polygonPath.closeSubPath();
    if (layout == "Studio") {
        polygonPath.clear();
        TUStudioPath.clear();
        TUStudioPixel[0] = convertMeterToPixel(TUStudioMeter[0].getX(), TUStudioMeter[0].getY());
        TUStudioPath.startNewSubPath(TUStudioPixel[0]);
        for (int i = 1; i <= 7; i++)
        {
            TUStudioPixel[i] = convertMeterToPixel(TUStudioMeter[i].getX(), TUStudioMeter[i].getY());
            TUStudioPath.lineTo(TUStudioPixel[i]);
        }
        TUStudioPath.closeSubPath();
    }
    coordinatesLabel.setBounds(20, 20, 80, 40);
    repaint();
}

void TopView::mouseDown(const juce::MouseEvent& e)
{
    auto pos = convertPixelToMeter(e.x, e.y);
    xAttachment->setValueAsCompleteGesture(pos.x);
    yAttachment->setValueAsCompleteGesture(pos.y);

    xAttachment->beginGesture();
    yAttachment->beginGesture();
    juce::String xcoord = juce::String(round((+20 * (float)e.getPosition().getX() / (float)getWidth() - 10) * 100) / 100) + " m";
    juce::String ycoord = juce::String(round((-20 * (float)e.getPosition().getY() / (float)getHeight() + 10) * 100) / 100) + " m";
    
    if (std::stof(xcoord.toStdString()) >= 0)
        xcoord = " "+ xcoord;
    if (std::stof(ycoord.toStdString()) >= 0)
        ycoord = " "+ ycoord;
    coordinatesLabel.setText("x= " + xcoord + " \ny= " + ycoord, juce::dontSendNotification);

    if (e.getPosition().getX() < 145 && e.getPosition().getY() < 90) 
    {
        coordinatesLabel.setBounds(getWidth() - 115, 20, 95, 40);
    }
    else { coordinatesLabel.setBounds(20, 20, 95, 40); }
    coordinatesLabel.setVisible(true);
}


void TopView::mouseDrag (const juce::MouseEvent& e)
{
    auto pos = convertPixelToMeter(e.x, e.y);
    xAttachment->setValueAsPartOfGesture(pos.x);
    yAttachment->setValueAsPartOfGesture(pos.y);
    juce::String xcoord = juce::String(round((+20*(float)e.getPosition().getX()/(float)getWidth()-10)*100)/100)+" m";
    juce::String ycoord = juce::String(round((-20*(float)e.getPosition().getY()/(float)getWidth()+10)*100)/100)+" m";
    if (std::stof(xcoord.toStdString()) >= 0)
        xcoord = " "+ xcoord;
    if (std::stof(ycoord.toStdString()) >= 0)
        ycoord = " "+ ycoord;
    coordinatesLabel.setText("x= " + xcoord + " \ny= " + ycoord, juce::dontSendNotification);
    
    if (e.getPosition().getX()<145 && e.getPosition().getY()<90){
        coordinatesLabel.setBounds(getWidth()-110, 20, 95, 40);
    } else {coordinatesLabel.setBounds(20, 20, 95, 40);}
}


void TopView::mouseUp(const juce::MouseEvent& e)
{
    xAttachment->endGesture();
    yAttachment->endGesture();
    coordinatesLabel.setVisible(false);
}

void TopView::connectXtoParameter(juce::RangedAudioParameter& p)
{
    xAttachment = std::make_unique<juce::ParameterAttachment>(p, [this](float newValue) 
        {
            xPos = newValue;
            auto pos = convertMeterToPixel(newValue, yPos).toInt();
            xPosPx = pos.x;
            source.setXPos(newValue);
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
            source.setYPos(newValue);
            resized();
        });
    yAttachment->sendInitialUpdate();
}


juce::Point<float> TopView::convertMeterToPixel(float xMeter, float yMeter)
{
    float xPixel = getLocalBounds().getWidth() * ((xMeter + 10)/20);
    float yPixel = (getLocalBounds().getHeight() * ((-yMeter + 10)/20));
    return juce::Point<float>(xPixel, yPixel);
}

juce::Point<double> TopView::convertPixelToMeter(int xPixel, int yPixel)
{   
    auto sad = getLocalBounds().getWidth();
    double xMeter = (double(xPixel) / getLocalBounds().getWidth()) * 20 - 10;
    double yMeter = -((double(yPixel) / getLocalBounds().getHeight()) * 20 - 10);
    return juce::Point<double>(xMeter, yMeter);
}

void TopView::setSourceWidthPx(int newWidth)
{
    sourceWidthPx = newWidth;
}

void TopView::setZPos(float newValue) { zPos = newValue; }


void TopView::changeLayout(bool HuFoSelected) 
{
    if (HuFoSelected == true) {layout = "HuFo";} else {layout = "Studio";}
    repaint();
    resized();
}

void TopView::showGrid(bool showGrid, bool xyzGrid) 
{
    if (showGrid == true) 
    {
        enableGrid = true;
        coordinatesLabel.setColour(juce::Label::backgroundColourId, juce::Colours::white);
        
    } 
    else 
    {
        enableGrid = false;
        coordinatesLabel.setColour(juce::Label::backgroundColourId, juce::Colour(000000));
    };
    
    if (xyzGrid == true) {grid = "xyz";} else {grid = "spherical";}
    repaint();
}



