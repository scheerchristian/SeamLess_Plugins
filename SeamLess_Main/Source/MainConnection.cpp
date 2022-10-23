/*
  ==============================================================================

    Connection.cpp
    Created: 19 Mar 2021 2:52:36pm
    Author:  anwaldt

  ==============================================================================
*/

#include "MainConnection.h"


MainConnection::MainConnection(juce::WaitableEvent& stop_signal, SeamLess_MainAudioProcessor *p)
: InterprocessConnection(true, 15),
  stop_signal_(stop_signal)
{
    processor = p;
    message = "-1/0/0/0";
}

void MainConnection::connectionMade()
{

        std::cout << "Got connection from client!" << std::endl;

        // String msg("Stop talking!");
        // MemoryBlock mb(msg.toRawUTF8(), msg.length());
        // sendMessage(mb);
}

void MainConnection::connectionLost()
{
        printf("Connection to client lost\n");

        processor->removeClosedConnections();
}

void MainConnection::messageReceived(const juce::MemoryBlock& msg)
{
    //seperate message type from message
    juce::StringArray stringArray;
    stringArray.addTokens(msg.toString(), "/", "§");
    juce::String messageType = stringArray[0];
    juce::String receivedMessage = msg.toString().trimCharactersAtStart(stringArray[0]+"/");
    //save different messages as different variables, so that they dont get overidden
    if (messageType == "pos")
        posmessage = receivedMessage;
     
    else
        message = receivedMessage;
}
