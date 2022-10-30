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
    slider.setColour(juce::Slider::textBoxOutlineColourId, transparent);
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
    connectXtoParameter(*treeState.getParameter("xPos"));
    connectYtoParameter(*treeState.getParameter("yPos"));
    connectZtoParameter(*treeState.getParameter("zPos"));
}

SphericalFader::~SphericalFader()
{
}

void SphericalFader::paint(juce::Graphics& g)
{
    g.setColour(seamlessGrey);
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
    xAttachment->beginGesture();
    yAttachment->beginGesture();
    zAttachment->beginGesture();
}

void SphericalFader::sliderDragEnded(juce::Slider* fader)
{
    onDrag = false;
    xAttachment->endGesture();
    yAttachment->endGesture();
    zAttachment->endGesture();
}

void SphericalFader::sliderValueChanged(juce::Slider* fader)
{
    onDrag = true;

    float radius = audioProcessor.getCurrentRadius();
    float azimuth = audioProcessor.getCurrentAzimuth();
    float elevation = audioProcessor.getCurrentElevation();

    if (sliderType == "azimuth")
    {
        azimuth = fader->getValue();
        audioProcessor.setCurrentAzimuth(azimuth);
    }
    else if (sliderType == "elevation")
    {
        elevation = fader->getValue();
        audioProcessor.setCurrentElevation(elevation);
    }
    float newX = x_from_spherical(radius, degree_to_radian(elevation), degree_to_radian(azimuth));
    float newY = -y_from_spherical(radius, degree_to_radian(elevation), degree_to_radian(azimuth));
    float newZ = z_from_spherical(radius, degree_to_radian(elevation));


    if (sliderType == "radius")
    {
        radius = fader->getValue();
        newX = x_from_spherical(radius, degree_to_radian(elevation), degree_to_radian(azimuth));
        newY = -y_from_spherical(radius, degree_to_radian(elevation), degree_to_radian(azimuth));
        newZ = z_from_spherical(radius, degree_to_radian(elevation));

        if (std::abs(newX) < 10 && std::abs(newY) < 10 && std::abs(newZ) < 10)
        {
            audioProcessor.setCurrentRadius(radius);
        }
        else
        {
            newX = treeState.getParameterAsValue("xPos").toString().getFloatValue();
            newY = treeState.getParameterAsValue("yPos").toString().getFloatValue();
            newZ = treeState.getParameterAsValue("zPos").toString().getFloatValue();

            radius = radius_from_cartesian(newX, newY, newZ);
            fader->setValue(radius, juce::dontSendNotification);
        }
    }
    if (std::isnan(newX))
        newX = 0;
    if (std::isnan(newY))
        newY = 0;
    if (std::isnan(newZ))
        newZ = 0;
    //needed to not get clamped values

    xAttachment->setValueAsPartOfGesture(newX);
    yAttachment->setValueAsPartOfGesture(newY);
    zAttachment->setValueAsPartOfGesture(newZ);
    
    // if the radius slider gets dragged to a value > 10 this might result in a shift of azimuth/elevation
    // as the soundSource might hit its boundaries and move along them. 
    // When x,y,z parameters change, the callback-function inside the SphericalBox checks if criticalRadius
    // is true. If so, the slider-objects are updated to keep them in sync.
    if (audioProcessor.getCurrentRadius() > 10)
        audioProcessor.setCriticalRadius(true);
    onDrag = false;
}

void SphericalFader::setSliderRange(juce::Range<double> newRange, double newInterval)
{
    slider.setRange(newRange, newInterval);
}

void SphericalFader::setSliderTextValueSuffix(juce::String newSuffix)
{
    slider.setTextValueSuffix(newSuffix);
}

void SphericalFader::connectXtoParameter(juce::RangedAudioParameter& p)
{
    xAttachment = std::make_unique<juce::ParameterAttachment>(p, [this](float newValue)
        {
        });
}

void SphericalFader::connectYtoParameter(juce::RangedAudioParameter& p)
{
    yAttachment = std::make_unique<juce::ParameterAttachment>(p, [this](float newValue)
        {
        });
}

void SphericalFader::connectZtoParameter(juce::RangedAudioParameter& p)
{
    zAttachment = std::make_unique<juce::ParameterAttachment>(p, [this](float newValue)
        {
        });
}


