#include <JuceHeader.h>
#include "PluginProcessor.h"
#include <cmath>

//==============================================================================
/*
*/
class SphericalFader : public juce::Slider, juce::Slider::Listener
{


public:
    SphericalFader(SeamLess_ClientAudioProcessor& p, juce::AudioProcessorValueTreeState& apvts, bool endless, juce::Slider::RotaryParameters rotaryParameters, juce::String type);
    ~SphericalFader() override;

    void paint(juce::Graphics&) override;
    void resized() override;

    void setText(juce::String s);

    void sliderDragStarted(juce::Slider* fader) override;
    void sliderDragEnded(juce::Slider*) override;
    void sliderValueChanged(juce::Slider*) override;

    void setSliderRange(juce::Range<double> newRange, double newInterval);
    void setSliderTextValueSuffix(juce::String newSuffix);



    float radius_from_cartesian(float x, float y, float z);
    float azimuth_from_cartesian(float x, float y);
    float elevation_from_cartesian(float x, float y, float z);
    float x_from_spherical(float radius, float elevation, float azimuth);
    float y_from_spherical(float radius, float elevation, float azimuth);
    float z_from_spherical(float radius, float elevation);
    float radian_to_degree(float radian);
    float degree_to_radian(float degree);

    juce::Slider slider;

    bool onDrag = false;

private:
    juce::Label sphericalNameLabel;
    SeamLess_ClientAudioProcessor& audioProcessor;
    juce::AudioProcessorValueTreeState& treeState;
    juce::String sliderType;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SphericalFader)
};
