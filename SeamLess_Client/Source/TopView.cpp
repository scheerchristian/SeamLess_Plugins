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
    addAndMakeVisible(coordinatesLabel);
    coordinatesLabel.setText("", juce::dontSendNotification);
    coordinatesLabel.setColour(juce::Label::textColourId, juce::Colours::darkgrey);
    coordinatesLabel.setVisible(false);
    //coordinatesLabel.setColour(juce::Label::backgroundColourId, juce::Colours::lightgrey);
    //coordinatesLabel.setColour(juce::Label::outlineColourId, juce::Colours::grey);
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
             g.drawLine(juce::Line<float>(convertMeterToPixel(4.2264973081, 20), convertMeterToPixel(15.7735026919, 0)), 0.8f);
             g.drawLine(juce::Line<float>(convertMeterToPixel(4.2264973081, 0), convertMeterToPixel(15.7735026919, 20)), 0.8f);
             g.drawLine(juce::Line<float>(convertMeterToPixel(0, 4.2264973081), convertMeterToPixel(20, 15.7735026919)), 0.8f);
             g.drawLine(juce::Line<float>(convertMeterToPixel(20, 4.2264973081), convertMeterToPixel(0, 15.7735026919)), 0.8f);
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
    if (layout == "Studio") {g.strokePath(TUStudioPath, juce::PathStrokeType(5.0f));}
   
    
    
}

void TopView::resized()
{
    source.setBounds(xPosPx - sourceWidthPx/2, yPosPx - sourceWidthPx/2, sourceWidthPx, sourceWidthPx);

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
        TUStudioPixel[0] = convertMeterToPixel(TUStudioMeter[0].getX() + 10, TUStudioMeter[0].getY() + 10);
        TUStudioPath.startNewSubPath(TUStudioPixel[0]);
        for (int i = 1; i <= 7; i++)
        {
            TUStudioPixel[i] = convertMeterToPixel(TUStudioMeter[i].getX() + 10, TUStudioMeter[i].getY() + 10);
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
    } else {coordinatesLabel.setBounds(20, 20, 95, 40);}
    coordinatesLabel.setVisible(true);
}


void TopView::mouseDrag (const juce::MouseEvent& e)
{
    auto pos = convertPixelToMeter(e.x, e.y);
    xAttachment->setValueAsPartOfGesture(pos.x);
    yAttachment->setValueAsPartOfGesture(pos.y);
    juce::String xcoord = juce::String(round((+20*(float)e.getPosition().getX()/(float)getWidth()-10)*100)/100)+" m";
    juce::String ycoord = juce::String(round((-20*(float)e.getPosition().getY()/(float)getWidth()+10)*100)/100)+" m";
    coordinatesLabel.setText("x= "+xcoord+" \ny= "+ycoord, juce::dontSendNotification);
    if (e.getPosition().getX()<145 and e.getPosition().getY()<90){
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
{    auto sad = getLocalBounds().getWidth();
    double xMeter = (double(xPixel) / getLocalBounds().getWidth()) * 20 - 10;
    double yMeter = (double(yPixel) / getLocalBounds().getHeight()) * 20 - 10;
    return juce::Point<double>(xMeter, yMeter);{
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
void TopView::changeLayout(bool HuFoSelected) {
    if (HuFoSelected == true) {layout = "HuFo";} else {layout = "Studio";}
    repaint();
    resized();
}

void TopView::showGrid(bool showGrid, bool xyzGrid) {
    if (showGrid == true) {
        enableGrid = true;
        coordinatesLabel.setColour(juce::Label::backgroundColourId, juce::Colours::white);
        
    } else {
        enableGrid = false;
        coordinatesLabel.setColour(juce::Label::backgroundColourId, juce::Colour(000000));
    };
    
    if (xyzGrid == true) {grid = "xyz";} else {grid = "spherical";}
    repaint();
}
