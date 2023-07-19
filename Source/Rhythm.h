/*
  ==============================================================================

    Rhythm.h
    Created: 17 Jul 2023 4:10:03pm
    Author:  Arthur Wilson

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class Rhythm : public juce::Component, private juce::ComboBox::Listener, private juce::Timer
{
public:
    Rhythm(const char* dataName);
    void paint(juce::Graphics &g) override;
    void resized() override;
    
    void comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged) override;
    
    void setSamplesPerBeat(int samples);
    void getNextBlock(juce::AudioBuffer<float>& buffer);
    
    void timerCallback() override {repaint();}
    
    void restart();
    void readFile(const char* dataName);
   
private:
    void updateTimeSig();
    void updateSamplesPerBeat();
    
    juce::Label volumeLabel;
    juce::Slider volumeSlider;
    juce::ComboBox nominatorCombo;
    juce::ComboBox denominatorCombo;
    juce::Label timeSigLabel;
    
    juce::String timeSignature;
    int nominator;
    int denominator;
    
    juce::Colour currentColour;
    int samplesPerBeat = 5000;
    int samplesPosition = 0;
    int beatCount = 0;
    
    juce::AudioBuffer<float> sampleBuffer;
    bool playBeep = false;
    int bufferPos = 0;
    
    bool showColour;
    int samplesPerBeatMul;
    int imageCyclePos = 0;
    
};
