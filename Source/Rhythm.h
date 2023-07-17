/*
  ==============================================================================

    Rhythm.h
    Created: 17 Jul 2023 4:10:03pm
    Author:  Arthur Wilson

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class Rhythm : public juce::Component, private juce::ComboBox::Listener
{
public:
    Rhythm();
    void paint(juce::Graphics &g) override;
    void resized() override;
    
    void comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged);
    
    void updateTimeSig();
    
private:
    juce::Label volumeLabel;
    juce::Slider volumeSlider;
    juce::ComboBox nominatorCombo;
    juce::ComboBox denominatorCombo;
    juce::Label timeSigLabel;
    
    juce::String timeSignature;
    int nominator;
    int denominator;
    
};
