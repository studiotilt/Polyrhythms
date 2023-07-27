/*
  ==============================================================================

    DragAndDrop.h
    Created: 20 May 2023 12:28:13pm
    Author:  Arthur Wilson

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class DragAndDrop : public juce::FileDragAndDropTarget, public juce::Component
{
public:
    
    DragAndDrop();
    ~DragAndDrop();
    
    bool isInterestedInFileDrag (const juce::StringArray& files) override;
    void fileDragEnter (const juce::StringArray& files, int x, int y) override;
    void filesDropped (const juce::StringArray& files, int x, int y) override;
    void fileDragExit (const juce::StringArray& files) override;
    
    void paint (juce::Graphics&) override;
    void resized() override;
    
    class Listener
    {
    public:
        Listener() {}
        virtual ~Listener() {}
        
        // called when a file was dropped onto component and successfuly read
        virtual void readyToProcess(juce::AudioBuffer<float> buffer, int sampleRate) {}
    };
    
    void addListener(Listener* listener) { listeners.add(listener); }
    void removeListener(Listener* listener) { listeners.remove(listener); }
    
private:
    void passUpBuffer();
    void readDroppedFile(juce::String filePath);
    bool isFileValid(juce::String filePath);
    
    juce::Colour colour = juce::Colours::blue;
    juce::ListenerList<Listener> listeners;
    juce::AudioBuffer<float> buffer;
    int sampleRate = 48000;
    juce::Label text;
    bool fileRead = false;
};
