/*
  ==============================================================================

    LFOBox.cpp
    Created: 12 Sep 2022 12:41:22pm
    Author:  Michael Witte

  ==============================================================================
*/

#include "LFOBox.h"
#include "LFOBox.h"


LFOBox::LFOBox(SeamLess_ClientAudioProcessor& p, juce::AudioProcessorValueTreeState& apvts) :
    audioProcessor(p), treeState(apvts),
    rateSlider(p, apvts, false, { 0, 1, true }, "lfoRate"),
    depthSlider(p, apvts, false, { 0, 2 * M_PI, false }, "lfoDepth"),
    phaseSlider(p, apvts, false, { M_PI , M_PI, true }, "lfoPhase"),
    offsetSlider(p, apvts, false, { M_PI , M_PI, true }, "lfoOffset"),
    LFOStartButton()
{
    for (int i = 0; i < 4; i++)
    {
        slider[i]->setText(names[i]);
        addAndMakeVisible(*slider[i]);
        slider[i]->setSliderRange(ranges[i], 0.01);
        slider[i]->setSliderTextValueSuffix(suffixes[i]);
        //getLookAndFeel().setColour (juce::Slider::thumbColourId, seamlessBlue);
    }
    addAndMakeVisible(LFOStartButton);
    LFOStartButton.addListener(this);
    LFOStartButton.setColour(juce::TextButton::buttonColourId, seamlessBlue);
    LFOStartButton.setComponentID("LFOStart");
    LFOStartButton.setButtonText("Start");

    rateSlider.setSliderRange(juce::Range<double>{0.0, 5.0}, 0.01);
    rateSlider.setSliderSkewFactor(0.5);
}

LFOBox::~LFOBox()
{
}

void LFOBox::paint (juce::Graphics& g)
{
    g.setColour (seamlessBlue);
    g.fillRoundedRectangle(getLocalBounds().toFloat(), 15);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    //g.drawText ("LFO", getWidth()/2-100, 20, 200, 20, juce::Justification::centred, true);
}


void LFOBox::resized()
{
    auto r = getLocalBounds().reduced(10, 10);

    auto sliderWidth = (r.getWidth() - 30) / 4;
    
    LFOStartButton.setBounds(10, 5, sliderWidth, 20);
    r.removeFromTop(20);

    auto sendFaderFOASection = r.removeFromLeft(sliderWidth);
    rateSlider.setBounds(sendFaderFOASection);

    r.removeFromLeft(10);

    auto sendFaderWFSSection = r.removeFromLeft(sliderWidth);
    depthSlider.setBounds(sendFaderWFSSection);

    r.removeFromLeft(10);

    auto sendFaderREVSection = r.removeFromLeft(sliderWidth);
    phaseSlider.setBounds(sendFaderREVSection);
    
    r.removeFromLeft(10);
    
    auto offsetSliderX = r.removeFromLeft(sliderWidth);
    offsetSlider.setBounds(offsetSliderX);
}


void LFOBox::sliderValueChanged (juce::Slider* slider)
{
}

void LFOBox::buttonClicked(juce::Button* button)
{
    float depth = treeState.getParameterAsValue("lfoDepth").toString().getFloatValue() / 10;
    float phase = treeState.getParameterAsValue("lfoPhase").toString().getFloatValue();
    float rate = treeState.getParameterAsValue("lfoRate").toString().getFloatValue();

    if (button == &LFOStartButton) 
    {
        if (button->getButtonText() == "Start") 
        {
            LFOStartButton.setButtonText("Stop");
            LFOStartButton.setColour(juce::TextButton::buttonColourId, seamlessGrey);
            audioProcessor.xLFO->setFrequency(rate);
            audioProcessor.xLFO->initialise([depth, phase, rate](float x)
                {
                    return depth * std::sin(rate * 2 * M_PI * x + phase);
                });
            audioProcessor.yLFO->setFrequency(rate);
            audioProcessor.yLFO->initialise([depth, phase, rate](float x)
                {
                    return depth * std::cos(rate * 2 * M_PI * x + phase);
                });
            //xAttachment->beginGesture();
            //yAttachment->beginGesture();
        }

        else
        {
            LFOStartButton.setButtonText("Start");
            LFOStartButton.setColour(juce::TextButton::buttonColourId, seamlessBlue);
            //xAttachment->endGesture();
            //yAttachment->endGesture();
            audioProcessor.xLFO->reset();
            audioProcessor.xLFO.reset();
            audioProcessor.xLFO = std::make_unique<juce::dsp::Oscillator<float>>();
            audioProcessor.yLFO->reset();
            audioProcessor.yLFO.reset();
            audioProcessor.yLFO = std::make_unique<juce::dsp::Oscillator<float>>();
            audioProcessor.xLFO->prepare(juce::dsp::ProcessSpec({ audioProcessor.getSampleRate() / audioProcessor.getBlockSize(), (juce::uint32)audioProcessor.getBlockSize(), 1 }));
            audioProcessor.yLFO->prepare(juce::dsp::ProcessSpec({ audioProcessor.getSampleRate() / audioProcessor.getBlockSize(), (juce::uint32)audioProcessor.getBlockSize(), 1 }));
        }
    }
}


void LFOBox::connectXtoParameter(juce::RangedAudioParameter& p)
{
    xAttachment = std::make_unique<juce::ParameterAttachment>(p, [this](float newValue) {});
}

void LFOBox::connectYtoParameter(juce::RangedAudioParameter& p)
{
    yAttachment = std::make_unique<juce::ParameterAttachment>(p, [this](float newValue) {});
}

