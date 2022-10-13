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
    xRateSlider(p, apvts, false, { 0, 1, true }, "xLfoRate"),
    xDepthSlider(p, apvts, false, { 0, 2 * M_PI, false }, "xLfoDepth"),
    xPhaseSlider(p, apvts, false, { M_PI , M_PI, true }, "xLfoPhase"),
    xOffsetSlider(p, apvts, false, { M_PI , M_PI, true }, "xLfoOffset"),
    yRateSlider(p, apvts, false, { 0, 1, true }, "yLfoRate"),
    yDepthSlider(p, apvts, false, { 0, 2 * M_PI, false }, "yLfoDepth"),
    yPhaseSlider(p, apvts, false, { M_PI , M_PI, true }, "yLfoPhase"),
    yOffsetSlider(p, apvts, false, { M_PI , M_PI, true }, "yLfoOffset"),
    zRateSlider(p, apvts, false, { 0, 1, true }, "zLfoRate"),
    zDepthSlider(p, apvts, false, { 0, 2 * M_PI, false }, "zLfoDepth"),
    zPhaseSlider(p, apvts, false, { M_PI , M_PI, true }, "zLfoPhase"),
    zOffsetSlider(p, apvts, false, { M_PI , M_PI, true }, "zLfoOffset"),
    LFOStartButton(),
    LFONumberBox()
{
    for (int i = 0; i < 12; i++)
    {
        sliders[i]->setText(names[i]);
        addAndMakeVisible(*sliders[i]);
        sliders[i]->setVisible(false);
        sliders[i]->setSliderRange(ranges[i], 0.01);
        sliders[i]->setSliderTextValueSuffix(suffixes[i]);
    }

    addAndMakeVisible(LFOStartButton);
    LFOStartButton.addListener(this);
    LFOStartButton.setColour(juce::TextButton::buttonColourId, seamlessBlue);
    LFOStartButton.setComponentID("LFOStart");
    LFOStartButton.setButtonText("Start");

    addAndMakeVisible(LFONumberBox);
    LFONumberBox.addListener(this);
    LFONumberBox.setColour(juce::TextButton::buttonColourId, seamlessBlue);
    LFONumberBox.setComponentID("LFONumber");
    LFONumberBox.setHelpText("Choose the coordinate to be controlled by the LFO. You can set LFOs for all 3 dimensions simutaniously. Just set each and click the Start-Button afterwards.");
    LFONumberBox.addItem("x", 1);
    LFONumberBox.addItem("y", 2);
    LFONumberBox.addItem("z", 3);

    for (int i = 0; i < 3; i++)
    {
        rateSliders[i]->setSliderRange(juce::Range<double>{0.0, 5.0}, 0.01);
        rateSliders[i]->setSliderSkewFactor(0.5);
    }
    LFONumberBox.setSelectedId(1);
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
    LFONumberBox.setBounds(10 + sliderWidth + 10, 5, sliderWidth * 2, 20);

    r.removeFromTop(20);

    auto sendFaderFOASection = r.removeFromLeft(sliderWidth);
    for (int i = 0; i < 3; i++)
        rateSliders[i]->setBounds(sendFaderFOASection);

    r.removeFromLeft(10);

    auto sendFaderWFSSection = r.removeFromLeft(sliderWidth);
    for (int i = 0; i < 3; i++)
        depthSliders[i]->setBounds(sendFaderWFSSection);

    r.removeFromLeft(10);

    auto sendFaderREVSection = r.removeFromLeft(sliderWidth);
    for (int i = 0; i < 3; i++)
        phaseSliders[i]->setBounds(sendFaderREVSection);
    
    r.removeFromLeft(10);
    
    auto offsetSliderX = r.removeFromLeft(sliderWidth);
    for (int i = 0; i < 3; i++)
        offsetSliders[i]->setBounds(offsetSliderX);
}


void LFOBox::sliderValueChanged (juce::Slider* slider)
{
}

void LFOBox::buttonClicked(juce::Button* button)
{
    float xDepth = treeState.getParameterAsValue("xLfoDepth").toString().getFloatValue() / 10;
    float xPhase = treeState.getParameterAsValue("xLfoPhase").toString().getFloatValue();
    float xRate = treeState.getParameterAsValue("xLfoRate").toString().getFloatValue();

    float yDepth = treeState.getParameterAsValue("yLfoDepth").toString().getFloatValue() / 10;
    float yPhase = treeState.getParameterAsValue("yLfoPhase").toString().getFloatValue();
    float yRate = treeState.getParameterAsValue("yLfoRate").toString().getFloatValue();

    float zDepth = treeState.getParameterAsValue("zLfoDepth").toString().getFloatValue() / 10;
    float zPhase = treeState.getParameterAsValue("zLfoPhase").toString().getFloatValue();
    float zRate = treeState.getParameterAsValue("zLfoRate").toString().getFloatValue();

    if (button == &LFOStartButton) 
    {
        if (button->getButtonText() == "Start") 
        {
            LFOStartButton.setButtonText("Stop");
            LFOStartButton.setColour(juce::TextButton::buttonColourId, seamlessGrey);
            audioProcessor.xLFO->setFrequency(xRate);
            audioProcessor.xLFO->initialise([xDepth, xPhase, xRate](float x)
                {
                    return xDepth * std::sin(xRate * 2 * M_PI * x + xPhase);
                });
            audioProcessor.yLFO->setFrequency(yRate);
            audioProcessor.yLFO->initialise([yDepth, yPhase, yRate](float x)
                {
                    return yDepth * std::sin(yRate * 2 * M_PI * x + yPhase);
                });
            audioProcessor.zLFO->setFrequency(zRate);
            audioProcessor.zLFO->initialise([zDepth, zPhase, zRate](float x)
                {
                    return zDepth * std::sin(zRate * 2 * M_PI * x + zPhase);
                });
        }

        else
        {
            LFOStartButton.setButtonText("Start");
            LFOStartButton.setColour(juce::TextButton::buttonColourId, seamlessBlue);
            audioProcessor.xLFO->reset();
            audioProcessor.xLFO.reset();
            audioProcessor.xLFO = std::make_unique<juce::dsp::Oscillator<float>>();
            audioProcessor.yLFO->reset();
            audioProcessor.yLFO.reset();
            audioProcessor.yLFO = std::make_unique<juce::dsp::Oscillator<float>>();
            audioProcessor.zLFO->reset();
            audioProcessor.zLFO.reset();
            audioProcessor.zLFO = std::make_unique<juce::dsp::Oscillator<float>>();
            audioProcessor.xLFO->prepare(juce::dsp::ProcessSpec({ audioProcessor.getSampleRate() / audioProcessor.getBlockSize(), (juce::uint32)audioProcessor.getBlockSize(), 1 }));
            audioProcessor.yLFO->prepare(juce::dsp::ProcessSpec({ audioProcessor.getSampleRate() / audioProcessor.getBlockSize(), (juce::uint32)audioProcessor.getBlockSize(), 1 }));
            audioProcessor.zLFO->prepare(juce::dsp::ProcessSpec({ audioProcessor.getSampleRate() / audioProcessor.getBlockSize(), (juce::uint32)audioProcessor.getBlockSize(), 1 }));

        }
    }
}

void LFOBox::comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged)
{
    for (int i = 0; i < 12; i++)
        sliders[i]->setVisible(false);

    int idx = comboBoxThatHasChanged->getSelectedItemIndex();
    if (idx == 0)
        for (int i = 0; i < 4; i++)
            xSliders[i]->setVisible(true);
    else if (idx == 1)
        for (int i = 0; i < 4; i++)
            ySliders[i]->setVisible(true);    
    else if (idx == 2)
        for (int i = 0; i < 4; i++)
            zSliders[i]->setVisible(true);
}


void LFOBox::connectXtoParameter(juce::RangedAudioParameter& p)
{
    xAttachment = std::make_unique<juce::ParameterAttachment>(p, [this](float newValue) {});
}

void LFOBox::connectYtoParameter(juce::RangedAudioParameter& p)
{
    yAttachment = std::make_unique<juce::ParameterAttachment>(p, [this](float newValue) {});
}

void LFOBox::connectZtoParameter(juce::RangedAudioParameter& p)
{
    zAttachment = std::make_unique<juce::ParameterAttachment>(p, [this](float newValue) {});

}

