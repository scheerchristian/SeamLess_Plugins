#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <math.h>

//==============================================================================

// initialize static members

bool SeamLess_ClientAudioProcessor::isSending   = false;
bool SeamLess_ClientAudioProcessor::playSending = false;

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

    client->connectToSocket("localhost", port_nr, 5000);  // no longer needed due to parameterAttachments

    setSendState(true);   //no longer needed due to parameterAttachments

    startTimer(SEND_INTERVAL);

    // LFOs
    xLFO = std::make_unique<juce::dsp::Oscillator<float>>();
    yLFO = std::make_unique<juce::dsp::Oscillator<float>>();
    zLFO = std::make_unique<juce::dsp::Oscillator<float>>();

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
    xLFO->prepare({ sampleRate, (juce::uint32)getBlockSize(), (juce::uint32)getTotalNumInputChannels()});
    //isSending=true;
    //processorChainLFO.prepare({ sampleRate / xLFOUpdateRate, (juce::uint32)getBlockSize(), (juce::uint32)getTotalNumInputChannels() });
}

void SeamLess_ClientAudioProcessor::releaseResources()
{
    //isSending=false;
    //processorChainLFO.reset();
    xLFO->reset();
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

    //xLFO->process(buffer);
    if (playSending == true && xLFO->isInitialised())
    {
        for (int sample = 0; sample < numSamples; sample += int(std::log2(numSamples)))
        {
            auto xLFOOut = xLFO->processSample(sample);
            parameters.getParameter("xPos")->setValueNotifyingHost(xLFOOut-10);
        }
        /*
        for (size_t pos = 0; pos < size_t(numSamples);)
        {
            //channelData[sample] = buffer.getSample(channel, sample);
            auto max = juce::jmin(numSamples - pos, xLFOUpdateCounter);
            pos += max;
            xLFOUpdateCounter -= max;
            if (xLFOUpdateCounter == 0)
            {
                xLFOUpdateCounter = xLFOUpdateRate;
                auto xLFOOut = xLFO->processSample(0.0f);
                parameters.getParameter("xPos")->setValueNotifyingHost(xLFOOut);
            }
        }
        */
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

    // for the tree only:
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);

    /*
    // for additional parameters:
    std::unique_ptr<juce::XmlElement> xml2 (new juce::XmlElement ("HoFo_Client"));

    xml2->setAttribute("sourceIdx", (int) sourceIdx.getValue());
    copyXmlToBinary(*xml2, destData);

    xml2->setAttribute ("oscTargetAddress", (juce::String) oscTargetAddress.getValue());
    copyXmlToBinary (*xml2, destData);

    xml2->setAttribute ("oscTargetPort", (int) oscTargetPort.getValue());
    copyXmlToBinary (*xml2, destData
    */
}

void SeamLess_ClientAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{

    // for the tree only:
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
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("lfoRate", 1), "LFO: Rate(Hz)", 0.0, 10.0, 0.0));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("lfoDepth", 1), "LFO: Depth(percent)", 0.0, 100.0, 0.0));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("lfoPhase", 1), "LFO: Phase(degree)", -180.0, 180.0, 0.0));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("lfoOffset", 1), "LFO: Offset(m)", -10.0, 10.0, 0.0));




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
}

int SeamLess_ClientAudioProcessor::getGridState()
{
    return gridState.getValue();
}

void SeamLess_ClientAudioProcessor::setGridState(int newValue)
{
    gridState.setValue(newValue);
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
