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
    slider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colour(00000000));
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
    onDrag = true;
    float currentX = treeState.getParameterAsValue("xPos").getValue().toString().getFloatValue();
    float currentY = treeState.getParameterAsValue("yPos").getValue().toString().getFloatValue();
    float currentZ = treeState.getParameterAsValue("zPos").getValue().toString().getFloatValue();

    // store backupAngles in case that radius slider gets pulled to 0
    if (currentX == 0 && currentY == 0 && currentZ == 0)
    {
        currentAzimuth = backupAzimuth;
        currentElevation = backupElevation;
        currentRadius = 0;
    }
    else if (currentX == 0 && currentY == 0) //same thing for |elevation| == 90¡
    {
        currentAzimuth = backupAzimuth;
        currentRadius = std::abs(currentZ);
    }
    else
    {
        currentAzimuth = -azimuth_from_cartesian(currentX, currentY);
        currentElevation = elevation_from_cartesian(currentX, currentY, currentZ);
        currentRadius = radius_from_cartesian(currentX, currentY, currentZ);
    }
        
    if (sliderType == "radius")
        currentRadius = fader->getValue();
    else if (sliderType == "azimuth")
        currentAzimuth = degree_to_radian(-fader->getValue());
    else if (sliderType == "elevation")
        currentElevation = degree_to_radian(fader->getValue());

    backupAzimuth = currentAzimuth;
    backupElevation = currentElevation;

    newX = x_from_spherical(currentRadius, currentElevation, currentAzimuth);
    newY = y_from_spherical(currentRadius, currentElevation, currentAzimuth);
    newZ = z_from_spherical(currentRadius, currentElevation);

    if (std::isnan(newX))
        newX = 0;
    if (std::isnan(newY))
        newY = 0;
    if (std::isnan(newZ))
        newZ = 0;
    //needed to not get clamped values

    xAttachment->setValueAsCompleteGesture(newX);
    yAttachment->setValueAsCompleteGesture(newY);
    zAttachment->setValueAsCompleteGesture(newZ);
    
    // if the radius slider gets dragged to a value > 10 this might result in a shift of azimuth/elevation
    // as the soundSource might hit its boundaries and move along them. 
    // When x,y,z parameters change, the callback-function inside the SprericalBox checks if criticalRadius
    // ís true. If so, the slider-objects are updated to keep them in sync.
    if (currentRadius > 10)
        setRadiusCritical(true);

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


bool  SphericalFader::isRadiusCritical()
{
    return criticalRadius;
}

void SphericalFader::setRadiusCritical(bool newValue)
{
    criticalRadius = newValue;
}
