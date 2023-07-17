/*
  ==============================================================================

    Rhythm.cpp
    Created: 17 Jul 2023 4:10:03pm
    Author:  Arthur Wilson

  ==============================================================================
*/

#include "Rhythm.h"

Rhythm::Rhythm()
{
    volumeLabel.setText("Volume", juce::dontSendNotification);
    volumeLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(volumeLabel);
    
    volumeSlider.setRange(0.0f, 1.0f);
    volumeSlider.setValue(1.0f);
    addAndMakeVisible(volumeSlider);
    
    for(int i = 1; i < 16; i++)
    {
        juce::String stringIndex(i);
        nominatorCombo.addItem(stringIndex, i);
        denominatorCombo.addItem(stringIndex, i);
    }
    
    nominatorCombo.setSelectedItemIndex(3);
    denominatorCombo.setSelectedItemIndex(3);
    nominatorCombo.addListener(this);
    denominatorCombo.addListener(this);
    addAndMakeVisible(nominatorCombo);
    addAndMakeVisible(denominatorCombo);
    
    addAndMakeVisible(timeSigLabel);
    
}

void Rhythm::paint(juce::Graphics &g)
{
}

void Rhythm::resized()
{
    volumeLabel     .setBounds(0,                         0,             getWidth()/3, getHeight()/2);
    volumeSlider    .setBounds(0,                         getHeight()/2, getWidth()/3, getHeight()/2);
    nominatorCombo  .setBounds(getWidth()/3,              0,             getWidth()/6, getHeight()/2);
    denominatorCombo.setBounds(getWidth()/3,              getHeight()/2, getWidth()/6, getHeight()/2);
    timeSigLabel    .setBounds(getWidth()/3+getWidth()/6, getHeight()/2, getWidth()/6, getHeight()/2);
    
}

void Rhythm::comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged)
{
    nominator = nominatorCombo.getSelectedItemIndex() + 1;
    denominator = denominatorCombo.getSelectedItemIndex()+ 1;
    updateTimeSig();
}


void Rhythm::updateTimeSig()
{
    timeSignature = juce::String(nominator);
    timeSignature += "/";
    timeSignature += juce::String(denominator);
    timeSigLabel.setText(timeSignature, juce::dontSendNotification);
}
