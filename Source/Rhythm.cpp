/*
  ==============================================================================

    Rhythm.cpp
    Created: 17 Jul 2023 4:10:03pm
    Author:  Arthur Wilson

  ==============================================================================
*/

#include "Rhythm.h"

Rhythm::Rhythm(const char* dataName)
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
    
    currentColour = juce::Colours::transparentBlack;
    
    juce::AudioFormatManager formatManager;
    formatManager.registerBasicFormats();
    int size = 0;
    auto resource = BinaryData::getNamedResource(dataName, size);
    juce::AudioFormatReader* reader {
        formatManager.createReaderFor(std::make_unique<juce::MemoryInputStream>(resource, size, false))
    };
    
    sampleBuffer.setSize(reader->numChannels, (int) reader->lengthInSamples);
    reader->read(&sampleBuffer, 0, reader->lengthInSamples, 0, true, true);
    
    startTimer(5);
}

void Rhythm::paint(juce::Graphics &g)
{
    g.setColour(currentColour);
    g.fillAll();
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
    updateSamplesPerBeat();
}


void Rhythm::updateTimeSig()
{
    timeSignature = juce::String(nominator);
    timeSignature += "/";
    timeSignature += juce::String(denominator);
    timeSigLabel.setText(timeSignature, juce::dontSendNotification);
}


void Rhythm::setSamplesPerBeat(int samples)
{
    samplesPerBeat = samples;
}

void Rhythm::updateSamplesPerBeat()
{
    samplesPerBeatMul = samplesPerBeat / (denominator / 4.0f);
    samplesPosition = 0;
    bufferPos = 0;
    readyToStart = true;
}

void Rhythm::homeBeat()
{
    readyToStart = false;
}

void Rhythm::getNextBlock(juce::AudioBuffer<float>& buffer)
{
    if (readyToStart) return;
    
    int numSamples = buffer.getNumSamples();
    int oldSamplesPosition = samplesPosition;
    
    samplesPosition+=numSamples;
    
    for (int i = 0; i < numSamples; i++)
    {
        if ((oldSamplesPosition + i) % samplesPerBeatMul == 0)
        {
            beatCount++;
            if (beatCount % nominator == 0)
            {
                beatCount = 0;
                playBeep = true;
                showColour = true;
                currentColour = juce::Colour((juce::uint8)244, (juce::uint8)244, (juce::uint8)244, (float)volumeSlider.getValue());
                bufferPos = 0;
            }
        }
    }
    
    if (playBeep)
    {
        int numSamplesToAdd = juce::jmin(sampleBuffer.getNumSamples()-bufferPos, numSamples);
        juce::AudioBuffer<float> copyBuffer;
        
        copyBuffer.setSize(2, numSamplesToAdd);
        copyBuffer.copyFrom(0, 0, sampleBuffer, 0, bufferPos, numSamplesToAdd);
        copyBuffer.applyGain(volumeSlider.getValue());
        
        buffer.addFrom(0, 0, copyBuffer, 0, 0, numSamplesToAdd);
        
        if(buffer.getNumChannels() > 1)
            buffer.addFrom(1, 0, copyBuffer, 0, 0, numSamplesToAdd);
        
        bufferPos += numSamplesToAdd;
        if (bufferPos >= sampleBuffer.getNumSamples())
        {
            bufferPos = 0;
            playBeep = false;
        }
    }
    
    if (showColour)
    {
        imageCyclePos+=numSamples;
        if (imageCyclePos >= 5000)
        {
            currentColour = juce::Colours::transparentBlack;
            imageCyclePos = 0;
            showColour = false;
        }
    }
    
}
