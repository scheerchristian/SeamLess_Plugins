#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <math.h>

//==============================================================================

// initialize static members



juce::OSCSender SeamLess_ClientAudioProcessor::sender1;

SeamLess_ClientAudioProcessor::SeamLess_ClientAudioProcessor()
    : parameters (*this, nullptr, juce::Identifier ("HuFo"),
                  createParameters()),
      client(new ClientConnection(*this)),    // no longer needed due to parameterAttachments
      AudioProcessor (BusesProperties()) // use no audio buses
{
    settings = juce::ValueTree("Settings");
    settings.setProperty("sourceIdx", juce::var(-1), nullptr);
    settings.setProperty("oscTargetAddress", juce::var("127.0.0.1"), nullptr);
    settings.setProperty("oscTargetPort", juce::var(9001), nullptr);
    settings.setProperty("settingsMode", juce::var(0), nullptr);
    settings.setProperty("sendButton", juce::var(true), nullptr);
    settings.setProperty("shapeState", juce::var(0), nullptr);
    settings.setProperty("gridState", juce::var(0), nullptr);
    settings.setProperty("selectedLFO", juce::var(0), nullptr);

    parameters.state.addChild(settings, 0, nullptr);

    xPos      = parameters.getRawParameterValue("xPos");
    yPos      = parameters.getRawParameterValue("yPos");
    zPos      = parameters.getRawParameterValue("zPos");

    sendGainWFS      = parameters.getRawParameterValue("sendGainWFS");
    sendGainHOA      = parameters.getRawParameterValue("sendGainHOA");
    sendGainREV      = parameters.getRawParameterValue("sendGainREV");

    // add a listener for every parameter to make it trigger parameterChanged()
    parameters.addParameterListener("xPos", this);
    parameters.addParameterListener("yPos", this);
    parameters.addParameterListener("zPos", this);

    parameters.addParameterListener("sendGainWFS", this);
    parameters.addParameterListener("sendGainHOA", this);
    parameters.addParameterListener("sendGainREV", this);

    client->connectToSocket("localhost", port_nr, 5000); 

    setSendState(true);  

    // LFOs

    xLFO = std::make_unique<juce::dsp::Oscillator<float>>();
    yLFO = std::make_unique<juce::dsp::Oscillator<float>>();
    zLFO = std::make_unique<juce::dsp::Oscillator<float>>();

    connectXtoParameter(*parameters.getParameter("xPos"));
    connectYtoParameter(*parameters.getParameter("yPos"));
    connectZtoParameter(*parameters.getParameter("zPos"));
    connectLfosToParameters();
    //connectLfosToParateres(getLfo());

    startTimer(SEND_INTERVAL);


}

SeamLess_ClientAudioProcessor::~SeamLess_ClientAudioProcessor()
{
}

//==============================================================================
const juce::String SeamLess_ClientAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SeamLess_ClientAudioProcessor::acceptsMidi() const
{
    return false;
}

bool SeamLess_ClientAudioProcessor::producesMidi() const
{
    return false;
}

bool SeamLess_ClientAudioProcessor::isMidiEffect() const
{
    return false;
}

double SeamLess_ClientAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SeamLess_ClientAudioProcessor::getNumPrograms()
{
    return 1;
}

int SeamLess_ClientAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SeamLess_ClientAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String SeamLess_ClientAudioProcessor::getProgramName (int index)
{
    return {};
}

void SeamLess_ClientAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void SeamLess_ClientAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    isSending=true;
    //processorChainLFO.prepare({ sampleRate / xLFOUpdateRate, (juce::uint32)getBlockSize(), (juce::uint32)getTotalNumInputChannels() });
    prepareLFOs();

}

void SeamLess_ClientAudioProcessor::releaseResources()
{
    isSending=false;
    //processorChainLFO.reset();
    xLFO->reset();
    yLFO->reset();
}


void SeamLess_ClientAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    auto* ph = getPlayHead();
    playSending = playInfo.getIsPlaying();

    int numSamples = buffer.getNumSamples();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());
    
    if (xLFO->isInitialised())
    {
        xLFOOut = xLFO->processSample(0.0f);
        yLFOOut = yLFO->processSample(0.0f);
        zLFOOut = zLFO->processSample(0.0f);
        xAttachment->setValueAsPartOfGesture(xLFOOut);
        yAttachment->setValueAsPartOfGesture(yLFOOut);
        zAttachment->setValueAsPartOfGesture(zLFOOut);
    }
}

bool SeamLess_ClientAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* SeamLess_ClientAudioProcessor::createEditor()
{
    return new SeamLess_ClientAudioProcessorEditor (*this, parameters);
}


void SeamLess_ClientAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void SeamLess_ClientAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName (parameters.state.getType()))
        {
            parameters.replaceState(juce::ValueTree::fromXml(*xmlState));
            if (xmlState->getFirstChildElement()->hasAttribute("sourceIdx"))
            {
                sourceIdx.referTo(parameters.state.getChildWithName("Settings").getPropertyAsValue("sourceIdx", nullptr));
                oscTargetAddress.referTo(parameters.state.getChildWithName("Settings").getPropertyAsValue("oscTargetAddress", nullptr));
                oscTargetPort.referTo(parameters.state.getChildWithName("Settings").getPropertyAsValue("oscTargetPort", nullptr));
                settingsMode.referTo(parameters.state.getChildWithName("Settings").getPropertyAsValue("settingsMode", nullptr));
                sendButtonState.referTo(parameters.state.getChildWithName("Settings").getPropertyAsValue("sendButton", nullptr));
                shapeState.referTo(parameters.state.getChildWithName("Settings").getPropertyAsValue("shapeState", nullptr));
                gridState.referTo(parameters.state.getChildWithName("Settings").getPropertyAsValue("gridState", nullptr));
                selectedLFO.referTo(parameters.state.getChildWithName("Settings").getPropertyAsValue("selectedLFO", nullptr));
                sender1.connect(oscTargetAddress.getValue(), 9001);
                DBG(xmlState->toString());
            }
            else
                sender1.connect(oscTargetAddress.getValue(), 9001);
        }



    // for additional parameters:
    /*
    std::unique_ptr<juce::XmlElement> xmlState2 (getXmlFromBinary (data, sizeInBytes));

    if (xmlState2.get() != nullptr)
    {
        if (xmlState2->hasTagName ("HoFo_Client"))
        {
            setOscTargetPort(xmlState2->getIntAttribute("sourceIdx"));
            oscTargetAddress = (juce::String) xmlState2->getStringAttribute("oscTargetAddress");
            setOscTargetPort((int) xmlState2->getIntAttribute("oscTargetPort", 1.0));
        }
    }
    */
}

juce::AudioProcessorValueTreeState::ParameterLayout SeamLess_ClientAudioProcessor::createParameters()
{
    
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("xPos", 1), "X Position", -10.0, 10.0, 0.0));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("yPos", 1), "Y Position", -10.0, 10.0, 0.0));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("zPos", 1), "Z Position", -10.0, 10.0, 0.0));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("sendGainWFS", 1), "Send Gain: WFS", -60.0, 0.0, -60.0));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("sendGainHOA", 1), "Send Gain: HOA", -60.0, 0.0, -60.0));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("sendGainREV", 1), "Send Gain: REV", -60.0, 0.0, -60.0));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("xLfoRate", 1), "LFO: Rate(Hz)", 0.0, 5.0, 0.0));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("xLfoDepth", 1), "LFO: Depth(percent)", 0.0, 100.0, 0.0));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("xLfoPhase", 1), "LFO: Phase(degree)", -180.0, 180.0, 0.0));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("xLfoOffset", 1), "LFO: Offset(m)", -10.0, 10.0, 0.0));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("yLfoRate", 1), "LFO: Rate(Hz)", 0.0, 5.0, 0.0));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("yLfoDepth", 1), "LFO: Depth(percent)", 0.0, 100.0, 0.0));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("yLfoPhase", 1), "LFO: Phase(degree)", -180.0, 180.0, 0.0));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("yLfoOffset", 1), "LFO: Offset(m)", -10.0, 10.0, 0.0));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("zLfoRate", 1), "LFO: Rate(Hz)", 0.0, 5.0, 0.0));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("zLfoDepth", 1), "LFO: Depth(percent)", 0.0, 100.0, 0.0));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("zLfoPhase", 1), "LFO: Phase(degree)", -180.0, 180.0, 0.0));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("zLfoOffset", 1), "LFO: Offset(m)", -10.0, 10.0, 0.0));

    //params.push_back(std::make_unique<juce::AudioParameterInt>(juce::ParameterID("sourceIdx", 1), "Source Index", -1, 128, -1));    //deprecated. Is now part of xml2

    return { params.begin(), params.end() };
}



float SeamLess_ClientAudioProcessor::getXPos()
{
    return (float) *xPos;
}

float SeamLess_ClientAudioProcessor::getYPos()
{
    return (float) *yPos;
}

float SeamLess_ClientAudioProcessor::getZPos()
{
    return (float) *zPos;
}


void SeamLess_ClientAudioProcessor::setXPos(float in)
{
    //*xPos = in;
    juce::Value val = parameters.getParameterAsValue("xPos");
    val.setValue(juce::var(in));
}

void SeamLess_ClientAudioProcessor::setYPos(float in)
{
    // *yPos = in;
    juce::Value val = parameters.getParameterAsValue("yPos");
    //val.setValue(juce::var(in));
}

void SeamLess_ClientAudioProcessor::setZPos(float in)
{
    //*zPos = in;
    juce::Value val = parameters.getParameterAsValue("zPos");
    //val.setValue(juce::var(in));
}


void SeamLess_ClientAudioProcessor::xyzPosSend()
{
    int i = (int) sourceIdx.getValue();
    float x = (float) *xPos;
    float y = (float) *yPos;
    float z = (float) *zPos;

    juce::OSCMessage m = juce::OSCMessage("/source/pos/xyz",i,x,y,z);
    sender1.send(m);
}


void SeamLess_ClientAudioProcessor::sendGainSend()
{
    int i = (int) sourceIdx.getValue();
    juce::OSCMessage m = juce::OSCMessage("/send/gain",i, 0, 0);

    float in = 20*log10((float) *sendGainHOA);
    m = juce::OSCMessage("/send/gain",i, 0, in);
    sender1.send(m);

    in = 20*log10((float) *sendGainWFS);
    m = juce::OSCMessage("/send/gain",i, 1, in);
    sender1.send(m);

    in = 20*log10((float) *sendGainREV);
    m = juce::OSCMessage("/send/gain",i, 2, in);
    sender1.send(m);

//    in = (float) *sendGainLFE;
//    m = juce::OSCMessage("/send/gain",i, 3, in);
//    sender1.send(m);

}

void SeamLess_ClientAudioProcessor::setSendGain(int sendIndex, float in)
{

    juce::Value val;

    switch (sendIndex)
    {
    case 0:
        val = parameters.getParameterAsValue("sendGainHOA");
        val.setValue(juce::var(in));
        break;
    case 1:
        val = parameters.getParameterAsValue("sendGainWFS");
        val.setValue(juce::var(in));
        break;
    case 2:
        val = parameters.getParameterAsValue("sendGainREV");
        val.setValue(juce::var(in));
        break;
//    case 3:
//        val = parameters.getParameterAsValue("sendGainLFE");
//        val.setValue(juce::var(in));
//        break;

    default:
        break;
    }

}


void SeamLess_ClientAudioProcessor::setSourceIndex(int i)
{
    juce::Identifier id ("sourceIdx");
    sourceIdx = i;
    std::cout << "Switched source index: " << i << '\n';
}

int SeamLess_ClientAudioProcessor::getSourceIndex()
{
    return sourceIdx.getValue();
}


void SeamLess_ClientAudioProcessor::setOscTargetAddress(juce::String address)
{
    oscTargetAddress.setValue(address);
    sender1.disconnect();
    sender1.connect(oscTargetAddress.getValue(), oscTargetPort.getValue());
    std::cout << "Switched OSC target (from address change): " << oscTargetAddress.getValue().toString() << ":" << oscTargetPort.getValue().toString() << '\n';
}


juce::String  SeamLess_ClientAudioProcessor::getOscTargetAddress()
{
    return oscTargetAddress.getValue();
}


void SeamLess_ClientAudioProcessor::setOscTargetPort(int port)
{
    oscTargetPort.setValue(port);
    sender1.disconnect();
    if (!sender1.connect(oscTargetAddress.getValue(), oscTargetPort.getValue()))
    {
        juce::String messageString("IP-Adress is invalid. Please enter a valid adress. Adress is now being set to the default value 127.0.0.1");

        juce::AlertWindow::showAsync(juce::MessageBoxOptions()
            .withIconType(juce::MessageBoxIconType::WarningIcon)
            .withTitle("Invalid IP-Adress")
            .withMessage(messageString)
            .withButton("OK"),
            nullptr);
    }

    std::cout << "Switched OSC target (from port change): " << oscTargetAddress.getValue().toString() << ":" << oscTargetPort.getValue().toString() << '\n';
}


int SeamLess_ClientAudioProcessor::getOscTargetPort()
{
    return oscTargetPort.getValue();
}

int SeamLess_ClientAudioProcessor::getSettingsMode()
{
    return settingsMode.toString().getIntValue();
}

void SeamLess_ClientAudioProcessor::setSettingsMode(int newValue)
{
    settingsMode.setValue(newValue);
}

int SeamLess_ClientAudioProcessor::getShapeState()
{
    return shapeState.getValue();
}

void SeamLess_ClientAudioProcessor::setShapeState(int newValue)
{
    shapeState.setValue(newValue);
}

juce::AudioProcessorValueTreeState& SeamLess_ClientAudioProcessor::getState()
{
    return parameters;
}

bool SeamLess_ClientAudioProcessor::getSendState()
{
    return isSending;
}

void SeamLess_ClientAudioProcessor::setSendState(bool s)
{
    isSending=s;
}

bool SeamLess_ClientAudioProcessor::getPlayState()
{
    return playSending;
}

bool SeamLess_ClientAudioProcessor::getSendButtonState()
{
    return sendButtonState.getValue();
}

void SeamLess_ClientAudioProcessor::setSendButtonState(bool newValue)
{
    sendButtonState.setValue(newValue);
}



void SeamLess_ClientAudioProcessor::parameterChanged(const juce::String & id, float val)
{
    // "Note that calling this method from within
    // AudioProcessorValueTreeState::Listener::parameterChanged()
    // is not guaranteed to return an up-to-date value for the parameter."

    //    if(id == "xPos")
    //        xPosSend(val);

    //    if(id == "yPos")
    //        yPosSend(val);

    //    if(id == "zPos")
    //        zPosSend(val);

    //    if(id=="sendGainWFS"||id=="sendGainHOA"||id=="sendGainREV"||id=="sendGainLFE")
    //        sendGainSend(id,val);
}


juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SeamLess_ClientAudioProcessor();
}


void SeamLess_ClientAudioProcessor::hiResTimerCallback()
{
    if(isSending==true && playSending==true)
    {

        // xPosSend();
        // yPosSend();
        // zPosSend();

        xyzPosSend();

        sendGainSend();
    }
    if (isSending)
    {
        int i = (int) sourceIdx.getValue();
        float x = (float) *xPos;
        float y = (float) *yPos;
        float z = (float) *zPos;
        juce::String ipcPos = juce::String(i)+"/"+juce::String(x)+"/"+juce::String(y)+"/"+juce::String(z);
        client->sendMessageToMain(ipcPos);
    }
    /*
    if (xLFO->isInitialised())
    {
        //xLFOOut = xLFO->processSample(0.0f);
        //yLFOOut = yLFO->processSample(0.0f);
        //xAttachment->setValueAsPartOfGesture(xLFOOut);
        //yAttachment->setValueAsPartOfGesture(yLFOOut);
    }
    */
}

int SeamLess_ClientAudioProcessor::getGridState()
{
    return gridState.getValue();
}

void SeamLess_ClientAudioProcessor::setGridState(int newValue)
{
    gridState.setValue(newValue);
}

int SeamLess_ClientAudioProcessor::getSelectedLFO()
{
    return selectedLFO.getValue();
}

void SeamLess_ClientAudioProcessor::setSelectedLFO(int newState)
{
    selectedLFO.setValue(newState);
}

bool SeamLess_ClientAudioProcessor::getConnectedToMain()
{
  return connectedToMain;
}

void SeamLess_ClientAudioProcessor::setConnectedToMain(bool b)
{
    connectedToMain = b;
}

void SeamLess_ClientAudioProcessor::reconnectToMainPlugin()
{
    client->connectToSocket("localhost", port_nr, 5000);

}

void SeamLess_ClientAudioProcessor::prepareLFOs()
{
    xLFO->prepare(juce::dsp::ProcessSpec({ getSampleRate() / getBlockSize(), (juce::uint32)getBlockSize(), 1 }));
    yLFO->prepare(juce::dsp::ProcessSpec({ getSampleRate() / getBlockSize(), (juce::uint32)getBlockSize(), 1 }));
    zLFO->prepare(juce::dsp::ProcessSpec({ getSampleRate() / getBlockSize(), (juce::uint32)getBlockSize(), 1 }));
}

void SeamLess_ClientAudioProcessor::refreshLFOs()
{   
    if (xLFO->isInitialised() && yLFO->isInitialised() && zLFO->isInitialised())
    {
        endLFOs();
        startLFOs();
    }
}

void SeamLess_ClientAudioProcessor::startLFOs()
{
    
    std::array<float, 12>lfoSettings = getLfoSettings();

    xLFO->setFrequency(lfoSettings[2]);
    xLFO->initialise([lfoSettings](float x)
        {
            return lfoSettings[0] * std::sin(x + lfoSettings[1]) + lfoSettings[3];
        });
    xAttachment->beginGesture();

    yLFO->setFrequency(lfoSettings[6]);
    yLFO->initialise([lfoSettings](float x)
        {
            return lfoSettings[4] * std::sin(x + lfoSettings[5]) + lfoSettings[7];
        });
    yAttachment->beginGesture();

    zLFO->setFrequency(lfoSettings[10]);
    zLFO->initialise([lfoSettings](float x)
        {
            return lfoSettings[8] * std::sin(x + lfoSettings[9]) + lfoSettings[11];
        });
    zAttachment->beginGesture();
    
}

void SeamLess_ClientAudioProcessor::endLFOs()
{
    xAttachment->endGesture();
    xLFO->reset();
    xLFO.reset();
    xLFO = std::make_unique<juce::dsp::Oscillator<float>>();

    yAttachment->endGesture();
    yLFO->reset();
    yLFO.reset();
    yLFO = std::make_unique<juce::dsp::Oscillator<float>>();

    zAttachment->endGesture();
    zLFO->reset();
    zLFO.reset();
    zLFO = std::make_unique<juce::dsp::Oscillator<float>>();
    prepareLFOs();
}

std::array<float, 12> SeamLess_ClientAudioProcessor::getLfoSettings()
{
    float xDepth = parameters.getParameterAsValue("xLfoDepth").toString().getFloatValue() / 10;
    float xPhase = parameters.getParameterAsValue("xLfoPhase").toString().getFloatValue() * M_PI / 180;
    float xRate = parameters.getParameterAsValue("xLfoRate").toString().getFloatValue();
    float xOffset = parameters.getParameterAsValue("xLfoOffset").toString().getFloatValue();

    float yDepth = parameters.getParameterAsValue("yLfoDepth").toString().getFloatValue() / 10;
    float yPhase = parameters.getParameterAsValue("yLfoPhase").toString().getFloatValue() * M_PI / 180;
    float yRate = parameters.getParameterAsValue("yLfoRate").toString().getFloatValue();
    float yOffset = parameters.getParameterAsValue("yLfoOffset").toString().getFloatValue();

    float zDepth = parameters.getParameterAsValue("zLfoDepth").toString().getFloatValue() / 10;
    float zPhase = parameters.getParameterAsValue("zLfoPhase").toString().getFloatValue() * M_PI / 180;
    float zRate = parameters.getParameterAsValue("zLfoRate").toString().getFloatValue();
    float zOffset = parameters.getParameterAsValue("zLfoOffset").toString().getFloatValue();

    std::array<float, 12> lfoSettings = {xDepth, xPhase, xRate, xOffset, yDepth, yPhase, yRate, yOffset, zDepth, zPhase, zRate, zOffset};
    return lfoSettings;
}

void SeamLess_ClientAudioProcessor::connectXtoParameter(juce::RangedAudioParameter& p)
{
    xAttachment = std::make_unique<juce::ParameterAttachment>(p, [this](float newValue) {});
}

void SeamLess_ClientAudioProcessor::connectYtoParameter(juce::RangedAudioParameter& p)
{
    yAttachment = std::make_unique<juce::ParameterAttachment>(p, [this](float newValue) {});
}

void SeamLess_ClientAudioProcessor::connectZtoParameter(juce::RangedAudioParameter& p)
{
    zAttachment = std::make_unique<juce::ParameterAttachment>(p, [this](float newValue) {});
}

void SeamLess_ClientAudioProcessor::connectLfosToParameters()
{
    xLfoRateAttachment = std::make_unique<juce::ParameterAttachment>(*parameters.getParameter("xLfoRate"), [this](float newValue) {refreshLFOs(); });
    xLfoDepthAttachment = std::make_unique<juce::ParameterAttachment>(*parameters.getParameter("xLfoDepth"), [this](float newValue) {refreshLFOs(); });
    xLfoPhaseAttachment = std::make_unique<juce::ParameterAttachment>(*parameters.getParameter("xLfoPhase"), [this](float newValue) {refreshLFOs(); });
    xLfoOffsetAttachment = std::make_unique<juce::ParameterAttachment>(*parameters.getParameter("xLfoOffset"), [this](float newValue) {refreshLFOs(); });

    yLfoRateAttachment = std::make_unique<juce::ParameterAttachment>(*parameters.getParameter("yLfoRate"), [this](float newValue) {refreshLFOs(); });
    yLfoDepthAttachment = std::make_unique<juce::ParameterAttachment>(*parameters.getParameter("yLfoDepth"), [this](float newValue) {refreshLFOs(); });
    yLfoPhaseAttachment = std::make_unique<juce::ParameterAttachment>(*parameters.getParameter("yLfoPhase"), [this](float newValue) {refreshLFOs(); });
    yLfoOffsetAttachment = std::make_unique<juce::ParameterAttachment>(*parameters.getParameter("yLfoOffset"), [this](float newValue) {refreshLFOs(); });

    zLfoRateAttachment = std::make_unique<juce::ParameterAttachment>(*parameters.getParameter("zLfoRate"), [this](float newValue) {refreshLFOs(); });
    zLfoDepthAttachment = std::make_unique<juce::ParameterAttachment>(*parameters.getParameter("zLfoDepth"), [this](float newValue) {refreshLFOs(); });
    zLfoPhaseAttachment = std::make_unique<juce::ParameterAttachment>(*parameters.getParameter("zLfoPhase"), [this](float newValue) {refreshLFOs(); });
    zLfoOffsetAttachment = std::make_unique<juce::ParameterAttachment>(*parameters.getParameter("zLfoOffset"), [this](float newValue) {refreshLFOs(); });
}