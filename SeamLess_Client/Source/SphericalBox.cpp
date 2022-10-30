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


void SphericalBox::updateSphericalSliders(float x, float y, float z, bool alsoUpdateRadius)
{
    azimuthSlider.slider.setValue(radian_to_degree(azimuth_from_cartesian(x, y)), juce::dontSendNotification);
    elevationSlider.slider.setValue(radian_to_degree(elevation_from_cartesian(x, y, z)), juce::dontSendNotification);
    if (alsoUpdateRadius == true)
    {
        radiusSlider.slider.setValue(radius_from_cartesian(x, y, z), juce::dontSendNotification);

        if (radiusSlider.getValue() != 0)
        {
            azimuthSlider.setRadiusCritical(false);
            elevationSlider.setRadiusCritical(false);
        }
        
    }

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
                updateSphericalSliders(x, y, z, true);    // update the sliders graphically
            // check if the radius slider was moved to a value > 10. If yes, update the slider-objects of azimuth and elevation
            // as they might have changed because the soundSource hit its boundaries and moved along them.
            if (radiusSlider.isRadiusCritical() == true)
                updateSphericalSliders(x, y, z, false);
            //if ((azimuthSlider.onDrag || elevationSlider.onDrag))
            if (radiusSlider.getCurrentRadius() != 0)
            {
                if (azimuthSlider.onDrag || elevationSlider.onDrag)
                {
                    if (std::abs(x_from_spherical(radiusSlider.slider.getValue(), elevationSlider.slider.getValue(), azimuthSlider.slider.getValue())) > 10) 
                    {
                        //backupRadius = radiusSlider.slider.getValue();
                        //backup = true;
                        radiusSlider.slider.setValue(radius_from_cartesian(x, y, z), juce::dontSendNotification);
                    }
                    //if (( std::abs(x_from_spherical(backupRadius, elevationSlider.slider.getValue(), azimuthSlider.slider.getValue())) <= 10) &&
                       // ( std::abs(y_from_spherical(backupRadius, elevationSlider.slider.getValue(), azimuthSlider.slider.getValue())) <= 10) &&
                        // (std::abs(z_from_spherical(backupRadius, elevationSlider.slider.getValue())) <= 10)
                    //{
                      //  backup = false;
                        //radiusSlider.slider.setValue(backupRadius, juce::dontSendNotification);
                   // }

                                    /*if (radius_from_cartesian(x, y, z) > 10)
                    if (azimuthSlider.onDrag || elevationSlider.onDrag)
                        radiusSlider.slider.setValue(radius_from_cartesian(x, y, z), juce::dontSendNotification);*/
                }
            }
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
                updateSphericalSliders(x, y, z, true);
            // check if the radius slider was moved to a value > 10. If yes, update the slider-objects of azimuth and elevation
            // as they might have changed because the soundSource hit its boundaries and moved along them.
            if (radiusSlider.isRadiusCritical() == true)
                updateSphericalSliders(x, y, z, false);
            if (radiusSlider.isRadiusCritical() == true)
                updateSphericalSliders(x, y, z, false);
            
            if (radiusSlider.getCurrentRadius() != 0)
            {
                //if (radius_from_cartesian(x, y, z) > 10)
                if (azimuthSlider.onDrag || elevationSlider.onDrag)
                {

                    if (std::abs(y_from_spherical(radiusSlider.slider.getValue(), elevationSlider.slider.getValue(), azimuthSlider.slider.getValue())) > 10) {
                        //backup = true;
                        radiusSlider.slider.setValue(radius_from_cartesian(x, y, z), juce::dontSendNotification);
                    }
                    /*if ( std::abs(y_from_spherical(backupRadius, elevationSlider.slider.getValue(), azimuthSlider.slider.getValue())) <= 10){
                        radiusSlider.slider.setValue(backupRadius, juce::dontSendNotification);
                    }*/
                }
            }
        
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
                updateSphericalSliders(x, y, z, true);    // update the sliders graphically
            // check if the radius slider was moved to a value > 10. If yes, update the slider-objects of azimuth and elevation
            // as they might have changed because the soundSource hit its boundaries and moved along them.
            if (radiusSlider.isRadiusCritical() == true)
                updateSphericalSliders(x, y, z, false);
            if (radiusSlider.getCurrentRadius() != 0)
            {
                if (azimuthSlider.onDrag || elevationSlider.onDrag)

                    if (std::abs(z_from_spherical(radiusSlider.slider.getValue(), elevationSlider.slider.getValue())) > 10)
                    {
                        //backup = true;
                        radiusSlider.slider.setValue(radius_from_cartesian(x, y, z), juce::dontSendNotification);
                    }

                /*if ( std::abs(z_from_spherical(backupRadius, elevationSlider.slider.getValue())) <= 10)
                        radiusSlider.slider.setValue(backupRadius, juce::dontSendNotification); */

                        /*if (radius_from_cartesian(x, y, z) > 10)
                            if (azimuthSlider.onDrag || elevationSlider.onDrag)
                                radiusSlider.slider.setValue(radius_from_cartesian(x, y, z), juce::dontSendNotification);*/
            }
        });
}

