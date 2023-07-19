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
    readFile(dataName);
    
    currentColour = juce::Colours::transparentBlack;
    
    volumeLabel.setText("Volume", juce::dontSendNotification);
    volumeLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(volumeLabel);
    
    volumeSlider.setRange(0.0f, 1.0f);
    volumeSlider.setValue(1.0f);
    addAndMakeVisible(volumeSlider);
    
    for(int i = 1; i < 32; i++)
    {
        juce::String stringIndex(i);
        nominatorCombo.addItem(stringIndex, i);
        denominatorCombo.addItem(stringIndex, i);
    }
    
    nominatorCombo.setSelectedItemIndex(3);
    addAndMakeVisible(nominatorCombo);
    nominatorCombo.addListener(this);
    
    denominatorCombo.setSelectedItemIndex(3);
    addAndMakeVisible(denominatorCombo);
    denominatorCombo.addListener(this);
    
    addAndMakeVisible(timeSigLabel);

    startTimer(5);
}

void Rhythm::paint(juce::Graphics &g)
{
    // paint the whole component the current colour
    g.setColour(currentColour);
    g.fillAll();
}

void Rhythm::resized()
{
    // size all the UI compoinents
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
}

void Rhythm::restart()
{
    bufferPos = 0;
    samplesPosition = 0;
    imageCyclePos = 0;
    playBeep = false;
    beatCount = 0;
}

void Rhythm::getNextBlock(juce::AudioBuffer<float>& buffer)
{
    int numSamples = buffer.getNumSamples();
    
    // check if the beat is in this block
    for (int i = 0; i < numSamples; i++)
    {
        if ((samplesPosition + i) % samplesPerBeatMul == 0)
        {
            beatCount++;
            if (beatCount % nominator == 0)
            {
                // beat is in this block, set everything accordingly
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
        
        // next block will overrun internal buffer. Stop playing now.
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
    
    samplesPosition += numSamples;
    
}

void Rhythm::readFile(const char* dataName)
{
    // set up format manager
    juce::AudioFormatManager formatManager;
    formatManager.registerBasicFormats();
    
    // get the resource
    int size = 0;
    auto resource = BinaryData::getNamedResource(dataName, size);
    
    // create reader and read file to internal audio buffer
    juce::AudioFormatReader* reader = formatManager.createReaderFor(std::make_unique<juce::MemoryInputStream>(resource, size, false));
    sampleBuffer.setSize(reader->numChannels, (int) reader->lengthInSamples);
    reader->read(&sampleBuffer, 0, (int) reader->lengthInSamples, 0, true, true);
    
}
