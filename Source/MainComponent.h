#pragma once

#include <JuceHeader.h>

#include "Rhythm.h"
#include "./Serial/SerialPortListMonitor.h"
#include "./Serial/SerialDevice.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::AudioAppComponent,
                       public juce::Button::Listener,
                       public juce::ComboBox::Listener,
                       public juce::Label::Listener
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;
    
    //==============================================================================
    void buttonClicked(juce::Button* button) override;
    
    void comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged) override;
    
    /** Called when a Label's text has changed. */
    void labelTextChanged (juce::Label* labelThatHasChanged) override;

private:
    juce::OwnedArray<Rhythm> rhythms;
    juce::TextButton startButton;
    float bpm = 400.0f;
    int samplesPerBeat = 24000;
    bool playing = false;
    
    std::unique_ptr<juce::AudioDeviceSelectorComponent> audioDeviceSelector;
    
    SerialPortListMonitor serialPortListMonitor;
    juce::ComboBox serialCombo;
    juce::String currentSerialDevice;
    void updateSerialComboBox();
    void updateSelectedSerialDevice();

    std::unique_ptr<SerialDevice> serialDevice;
    
    juce::Label loInputBox;
    juce::Label hiInputBox;
    
    float loInput = 0;
    float hiInput = 1;
    float scaleValue(float input);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
