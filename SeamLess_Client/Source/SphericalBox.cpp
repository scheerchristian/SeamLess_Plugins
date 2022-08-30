#define _USE_MATH_DEFINES
#include "SphericalBox.h"
//==============================================================================
SphericalBox::SphericalBox(SeamLess_ClientAudioProcessor& p, juce::AudioProcessorValueTreeState& apvts) :
    audioProcessor(p), treeState(apvts),
   
    rSlider(p, apvts, false, { 0, 1, true }, "radius"),
    azimuthSlider(p, apvts, true, { 7*M_PI/2, 3 * M_PI / 2,  false }, "azimuth"),
    elevationSlider(p, apvts, true, { M_PI, 0, false }, "elevation")
{
    rSlider.setName("rSlider");
    azimuthSlider.setName("azimutSlider");
    elevationSlider.setName("elevationSlider");
    for (int i = 0; i < 3; i++)
    {
        faders[i]->setText(names[i]);
        addAndMakeVisible(*faders[i]);
        faders[i]->setSliderRange(ranges[i], 0.01);
        faders[i]->setSliderTextValueSuffix(suffixes[i]);
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
    g.fillRoundedRectangle(getLocalBounds().toFloat(), 20);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("Spherical Coordinates", getWidth()/2-100, 20, 200, 20, juce::Justification::centred, true);
}


void SphericalBox::resized()
{
    auto r = getLocalBounds().reduced(20, 20);

    auto sliderWidth = (r.getWidth() - 40) / 3;

    r.removeFromTop(40);

    auto sendFaderFOASection = r.removeFromLeft(sliderWidth);
    rSlider.setBounds(sendFaderFOASection);

    r.removeFromLeft(20);

    auto sendFaderWFSSection = r.removeFromLeft(sliderWidth);
    azimuthSlider.setBounds(sendFaderWFSSection);

    r.removeFromLeft(20);

    auto sendFaderREVSection = r.removeFromLeft(sliderWidth);
    elevationSlider.setBounds(sendFaderREVSection);

}

void SphericalBox::connectXtoParameter(juce::RangedAudioParameter& p)
{
    xAttachment = std::make_unique<juce::ParameterAttachment>(p, [this](float newValue)
        {
            float x = newValue;
            float y = treeState.getParameterAsValue("yPos").toString().getFloatValue();
            float z = treeState.getParameterAsValue("zPos").toString().getFloatValue();
            
            if (rSlider.onDrag == false && azimuthSlider.onDrag == false && elevationSlider.onDrag == false)
                updateSphericalSliders(x, y, z);
        });
}

void SphericalBox::connectYtoParameter(juce::RangedAudioParameter& p)
{
    yAttachment = std::make_unique<juce::ParameterAttachment>(p, [this](float newValue)
        {            
            float x = treeState.getParameterAsValue("xPos").toString().getFloatValue();
            float y = newValue;
            float z = treeState.getParameterAsValue("zPos").toString().getFloatValue();
            
            if (rSlider.onDrag == false && azimuthSlider.onDrag == false && elevationSlider.onDrag == false)
                updateSphericalSliders(x, y, z);
        });
}

void SphericalBox::connectZtoParameter(juce::RangedAudioParameter& p)
{
    zAttachment = std::make_unique<juce::ParameterAttachment>(p, [this](float newValue)
        {
            float x = treeState.getParameterAsValue("xPos").toString().getFloatValue();
            float y = treeState.getParameterAsValue("yPos").toString().getFloatValue();
            float z = newValue;

            if (rSlider.onDrag == false && azimuthSlider.onDrag == false && elevationSlider.onDrag == false)
                updateSphericalSliders(x, y, z);
        });
}

void SphericalBox::updateSphericalSliders(float x, float y, float z)
{
    rSlider.slider.setValue(rSlider.radius_from_cartesian(x, y, z), juce::dontSendNotification);
    azimuthSlider.slider.setValue(rSlider.radian_to_degree(azimuthSlider.azimuth_from_cartesian(x, y)), juce::dontSendNotification);
    elevationSlider.slider.setValue(elevationSlider.radian_to_degree(elevationSlider.elevation_from_cartesian(x, y, z)), juce::dontSendNotification);
}
