/*
  ==============================================================================

    Rhythm.h
    Created: 17 Jul 2023 4:10:03pm
    Author:  Arthur Wilson

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DragAndDrop.h"

class Rhythm : public juce::Component,
               private juce::ComboBox::Listener,
               private juce::Slider::Listener,
               private juce::Timer,
               private DragAndDrop::Listener
{
public:
    Rhythm(const char* dataName);
    void paint(juce::Graphics &g) override;
    void resized() override;
    
    void comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged) override;
    void sliderValueChanged (juce::Slider* slider) override;
    
    void setSamplesPerBeat(int samples);
    void setSampleRate(int sampleRate, int maxBufferSize);
    void getNextBlock(juce::AudioBuffer<float>& buffer, int outputChannel);
    
    void timerCallback() override {repaint();}
    
    void restart();
    void readFile(const char* dataName);
    
    void setInteractionValue(float newValue);
    
    void readyToProcess(juce::AudioBuffer<float> buffer, int sampleRate) override;
   
private:
    void updateTimeSig();
    void updateSamplesPerBeat();
    void setEnvelopeParameters();
    
    std::atomic<float> volume = 0.0f;
    juce::Label volumeLabel;
    juce::Slider volumeSlider;
    juce::ComboBox nominatorCombo;
    juce::ComboBox denominatorCombo;
    juce::Label timeSigLabel;
    juce::Slider granularPosition;
    juce::Label granularPositionLabel;
    
    std::atomic<float> attack = 0.0f;
    std::atomic<float> sustain = 0.0f;
    std::atomic<float> decay = 0.0f;
    std::atomic<float> release = 0.0f;
    std::atomic<float> length = 0.0f;
    juce::Slider granularEnvAttack;
    juce::Slider granularEnvSustain;
    juce::Slider granularEnvDecay;
    juce::Slider granularEnvRel;
    juce::Slider granularLength;
    juce::Label granularEnvAttackLabel;
    juce::Label granularEnvSustainLabel;
    juce::Label granularEnvDecayLabel;
    juce::Label granularEnvRelLabel;
    juce::Label granularEnvLengthLabel;
    
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
    int granularLengthInt = 0;
    
    bool showColour;
    int samplesPerBeatMul;
    int imageCyclePos = 0;
    
    juce::ADSR envelope;
    int deviceSampleRate;
    
    juce::Slider* currentAffectedSlider;
    
    juce::ComboBox affectedSliderCombo;
    
    juce::CriticalSection cs;
    
    DragAndDrop dragAndDrop;
};
