#define _USE_MATH_DEFINES
#include "SphericalBox.h"
#include "trigonometricHelpers.h"

//==============================================================================
SphericalBox::SphericalBox(SeamLess_ClientAudioProcessor& p, juce::AudioProcessorValueTreeState& apvts) :
    audioProcessor(p), treeState(apvts),
   
    radiusSlider(p, apvts, false, { 0, 1, true }, "radius"),
    azimuthSlider(p, apvts, true, { 3 * juce::MathConstants<double>::pi / 2, 7*juce::MathConstants<double>::pi/2,  false }, "azimuth"),
    elevationSlider(p, apvts, true, {0, juce::MathConstants<double>::pi, false }, "elevation")
{
    radiusSlider.setName("radiusSlider");
    azimuthSlider.setName("azimutSlider");
    elevationSlider.setName("elevationSlider");
    for (int i = 0; i < 3; i++)
    {
        faders[i]->setText(names[i]);
        addAndMakeVisible(*faders[i]);
        faders[i]->setSliderTextValueSuffix(suffixes[i]);
        if (i == 1) // special treatment for azimut slider
        {
            auto range = juce::NormalisableRange<double>(-180.0, 180.0,
                [](auto rangeStart, auto rangeEnd, auto normalised)
                { return rangeStart + (1.0 - normalised) * (rangeEnd - rangeStart); },
                [](auto rangeStart, auto rangeEnd, auto value)
                { return 1.0 - (value - rangeStart) / (rangeEnd - rangeStart); },
                [](auto rangeStart, auto rangeEnd, auto value)
                { return int(value); });
            range.interval = 0.01;

            faders[i]->slider.setNormalisableRange(range);
            faders[i]->slider.setNumDecimalPlacesToDisplay(0);
        }
        else if (i == 2) // special treatment for elevation slider
        {
            auto range = juce::NormalisableRange<double>(-90.0, 90.0,
                [](auto rangeStart, auto rangeEnd, auto normalised)
                { return rangeStart + (1.0 - normalised) * (rangeEnd - rangeStart); },
                [](auto rangeStart, auto rangeEnd, auto value)
                { return 1.0 - (value - rangeStart) / (rangeEnd - rangeStart); },
                [](auto rangeStart, auto rangeEnd, auto value)
                { return int(value); });
            range.interval = 0.01;

            faders[i]->slider.setNormalisableRange(range);
            faders[i]->slider.setNumDecimalPlacesToDisplay(0);
        }
        else
            faders[i]->setSliderRange(ranges[i], 0.01);

    }

    connectXtoParameter(*treeState.getParameter("xPos"));
    connectYtoParameter(*treeState.getParameter("yPos"));
    connectZtoParameter(*treeState.getParameter("zPos"));
}

SphericalBox::~SphericalBox()
{
}

void SphericalBox::paint (juce::Graphics& g)
{
    g.setColour (seamlessBlue);
    g.fillRoundedRectangle(getLocalBounds().toFloat(), 15);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    //g.drawText ("Spherical Coordinates", getWidth()/2-100, 20, 200, 20, juce::Justification::centred, true);
}


void SphericalBox::resized()
{
    auto r = getLocalBounds().reduced(10, 10);

    auto sliderWidth = (r.getWidth() - 20) / 3;

    //r.removeFromTop(40);

    auto sendFaderFOASection = r.removeFromLeft(sliderWidth);
    radiusSlider.setBounds(sendFaderFOASection);

    r.removeFromLeft(10);

    auto sendFaderWFSSection = r.removeFromLeft(sliderWidth);
    azimuthSlider.setBounds(sendFaderWFSSection);

    r.removeFromLeft(10);

    auto sendFaderREVSection = r.removeFromLeft(sliderWidth);
    elevationSlider.setBounds(sendFaderREVSection);

}


void SphericalBox::updateSphericalSliders(float x, float y, float z)
{
    audioProcessor.setCurrentRadius(radius_from_cartesian(x, y, z));
    audioProcessor.setCurrentAzimuth(radian_to_degree(azimuth_from_cartesian(x, y)));
    audioProcessor.setCurrentElevation(radian_to_degree(elevation_from_cartesian(x, y, z)));

    if (audioProcessor.getCurrentRadius() != 0)
    {
        azimuthSlider.slider.setValue(audioProcessor.getCurrentAzimuth(), juce::dontSendNotification);
        elevationSlider.slider.setValue(audioProcessor.getCurrentElevation(), juce::dontSendNotification);
    }
    radiusSlider.slider.setValue(audioProcessor.getCurrentRadius(), juce::dontSendNotification);

}

//==============================================================================
//  Callback-functions for which are called when parameters in the apvts change
//==============================================================================


void SphericalBox::connectXtoParameter(juce::RangedAudioParameter& p)
{
    xAttachment = std::make_unique<juce::ParameterAttachment>(p, [this](float newValue)
        {
            float x = newValue;
            float y = treeState.getParameterAsValue("yPos").toString().getFloatValue();
            float z = treeState.getParameterAsValue("zPos").toString().getFloatValue();

            // check if spherical sliders are not sending information to avoid sync-problems
            if (radiusSlider.onDrag == false && azimuthSlider.onDrag == false && elevationSlider.onDrag == false)
                updateSphericalSliders(x, y, z);    // update the sliders graphically
        });
}

void SphericalBox::connectYtoParameter(juce::RangedAudioParameter& p)
{
    yAttachment = std::make_unique<juce::ParameterAttachment>(p, [this](float newValue)
        {            
            float x = treeState.getParameterAsValue("xPos").toString().getFloatValue();
            float y = newValue;
            float z = treeState.getParameterAsValue("zPos").toString().getFloatValue();

            // check if spherical sliders are not sending information to avoid sync-problems
            if (radiusSlider.onDrag == false && azimuthSlider.onDrag == false && elevationSlider.onDrag == false)
                updateSphericalSliders(x, y, z);    // update the sliders graphically
        });
}

void SphericalBox::connectZtoParameter(juce::RangedAudioParameter& p)
{
    zAttachment = std::make_unique<juce::ParameterAttachment>(p, [this](float newValue)
        {
            float x = treeState.getParameterAsValue("xPos").toString().getFloatValue();
            float y = treeState.getParameterAsValue("yPos").toString().getFloatValue();
            float z = newValue;

            // check if spherical sliders are not sending information to avoid sync-problems
            if (radiusSlider.onDrag == false && azimuthSlider.onDrag == false && elevationSlider.onDrag == false)
                updateSphericalSliders(x, y, z);    // update the sliders graphically
        });
}

