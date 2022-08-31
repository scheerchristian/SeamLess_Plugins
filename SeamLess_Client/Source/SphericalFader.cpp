#define _USE_MATH_DEFINES
#include <JuceHeader.h>
#include "SphericalFader.h"
#include "trigonometricHelpers.h"


//==============================================================================
SphericalFader::SphericalFader(SeamLess_ClientAudioProcessor& p, juce::AudioProcessorValueTreeState& apvts, bool endless, juce::Slider::RotaryParameters rotaryParameters, juce::String type) 
    :audioProcessor(p), treeState(apvts), sliderType(type)
   
{
    slider.setSliderStyle(juce::Slider::Rotary);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 90, 30);
    slider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::black);
    slider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::grey);
    slider.setNumDecimalPlacesToDisplay(2);
   
    if (endless == true)
        slider.setRotaryParameters(rotaryParameters);
    addAndMakeVisible(slider);

    // SphericalNameLabel.attachToComponent (&slider, false);
    sphericalNameLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    sphericalNameLabel.setJustificationType(juce::Justification::centred);
    sphericalNameLabel.setText("SND", juce::dontSendNotification);
    sphericalNameLabel.attachToComponent(&slider, false);

    slider.addListener(this);
    addAndMakeVisible(sphericalNameLabel);


}

SphericalFader::~SphericalFader()
{
}

void SphericalFader::paint(juce::Graphics& g)
{
    g.setColour(juce::Colours::grey);
    g.fillRoundedRectangle(getLocalBounds().toFloat(), 10);   // draw an outline around the component
}

void SphericalFader::resized()
{
    slider.setBounds(0,30,getWidth(),getHeight()*0.85);
    //slider.setBounds(getWidth() * -0.1, 40, getWidth() * 1.2, getHeight() * 0.85);
}

void SphericalFader::setText(juce::String s)
{
    sphericalNameLabel.setText(s, juce::dontSendNotification);
}

void SphericalFader::sliderDragStarted(juce::Slider* fader)
{
    onDrag = true;
}

void SphericalFader::sliderDragEnded(juce::Slider* fader)
{
    onDrag = false;
}

void SphericalFader::sliderValueChanged(juce::Slider* fader)
{
    //onDrag = true;
    auto currentX = treeState.getParameterAsValue("xPos").getValue().toString().getFloatValue();
    auto currentY = treeState.getParameterAsValue("yPos").getValue().toString().getFloatValue();
    auto currentZ = treeState.getParameterAsValue("zPos").getValue().toString().getFloatValue();

    auto currentAzimuth = azimuth_from_cartesian(currentX, currentY);
    auto currentElevation = elevation_from_cartesian(currentX, currentY, currentZ);
    auto currentRadius = radius_from_cartesian(currentX, currentY, currentZ);

    if (sliderType == "radius")
        currentRadius = fader->getValue();
    else if (sliderType == "azimuth")
        currentAzimuth = degree_to_radian(fader->getValue());
    else if (sliderType == "elevation")
        currentElevation = degree_to_radian(fader->getValue());

    auto newX = x_from_spherical(currentRadius, currentElevation, currentAzimuth);
    auto newY = y_from_spherical(currentRadius, currentElevation, currentAzimuth);
    auto newZ = z_from_spherical(currentRadius, currentElevation);

    treeState.getParameter("xPos")->setValueNotifyingHost(newX);
    treeState.getParameter("yPos")->setValueNotifyingHost(newY);
    treeState.getParameter("zPos")->setValueNotifyingHost(newZ);
    //onDrag = false;
}

void SphericalFader::setSliderRange(juce::Range<double> newRange, double newInterval)
{
    slider.setRange(newRange, newInterval);
}

void SphericalFader::setSliderTextValueSuffix(juce::String newSuffix)
{
    slider.setTextValueSuffix(newSuffix);
}
