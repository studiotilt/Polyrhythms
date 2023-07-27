/*
  ==============================================================================

    DragAndDrop.cpp
    Created: 20 May 2023 12:28:13pm
    Author:  Arthur Wilson

  ==============================================================================
*/

#include "DragAndDrop.h"

DragAndDrop::DragAndDrop()
{
    text.setText("Drop Files Here", juce::dontSendNotification);
    text.setFont(juce::Font(12));
    addAndMakeVisible(text);
}

DragAndDrop::~DragAndDrop()
{
    
}

//==============================================================================
bool DragAndDrop::isInterestedInFileDrag (const juce::StringArray& files)
{
    if (files.size() == 1)
        return true;
    else
        return false;
}

void DragAndDrop::fileDragEnter (const juce::StringArray& files, int x, int y)
{
    isFileValid(files[0]);
    repaint();
}

void DragAndDrop::filesDropped (const juce::StringArray& files, int x, int y)
{
    if (isFileValid(files[0]))
        readDroppedFile(files[0]);
    
    colour = juce::Colours::blue;
}

void DragAndDrop::fileDragExit(const juce::StringArray& files)
{
    colour = juce::Colours::blue;
    repaint();
}

void DragAndDrop::paint (juce::Graphics& g)
{
    g.setColour(colour);
    g.fillRect(getLocalBounds());
}

void DragAndDrop::resized()
{
    text.setBounds(0, 0, getLocalBounds().getWidth(), 30);
}

//==============================================================================
void DragAndDrop::passUpBuffer()
{
    juce::MessageManager::callAsync([=] {
        listeners.call(&Listener::readyToProcess, buffer, sampleRate);
    });
}

void DragAndDrop::readDroppedFile(juce::String filePath)
{
    juce::File fileToRead(filePath);
    juce::AudioFormatManager manager;
    
    manager.registerBasicFormats();
    std::unique_ptr<juce::AudioFormatReader> reader(manager.createReaderFor(fileToRead));
    
    if(reader)
    {
        buffer.clear();
        buffer.setSize(reader->numChannels, (int) reader->lengthInSamples);
        sampleRate = reader->sampleRate;
        reader->read(&buffer, 0, (int) reader->lengthInSamples, 0, true, true);
        passUpBuffer();
    }
}

bool DragAndDrop::isFileValid(juce::String filePath)
{
    if (filePath.endsWithIgnoreCase(".wav")
        || filePath.endsWithIgnoreCase(".ogg")
        || filePath.endsWithIgnoreCase(".aiff")
        || filePath.endsWithIgnoreCase(".mp3")
        || filePath.endsWithIgnoreCase(".flac"))
    {
        colour = juce::Colours::green;
        return true;
    }
    else
    {
        colour = juce::Colours::red;
        return false;
    }
}
