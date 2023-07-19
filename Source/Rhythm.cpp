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
    volumeSlider.setValue(0.0f);
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
    
    granularPosition.setRange(0.0f, 1.0f);
    granularPosition.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    granularPosition.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 100, 25);
    addAndMakeVisible(granularPosition);

    granularPositionLabel.setText("StartPos", juce::dontSendNotification);
    granularPositionLabel.setJustificationType(juce::Justification::topLeft);
    granularPositionLabel.setInterceptsMouseClicks(false, false);
    addAndMakeVisible(granularPositionLabel);
    
    granularEnvAttack.setRange(0.0f, 1.0f);
    granularEnvAttack.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    granularEnvAttack.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 100, 25);
    addAndMakeVisible(granularEnvAttack);
    granularEnvAttack.addListener(this);
    
    granularEnvAttackLabel.setText("Attack", juce::dontSendNotification);
    granularEnvAttackLabel.setJustificationType(juce::Justification::topLeft);
    granularEnvAttackLabel.setInterceptsMouseClicks(false, false);
    addAndMakeVisible(granularEnvAttackLabel);
    
    granularEnvDecay.setRange(0.0f, 1.0f);
    granularEnvDecay.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    granularEnvDecay.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 100, 25);
    addAndMakeVisible(granularEnvDecay);
    
    granularEnvDecayLabel.setText("Decay", juce::dontSendNotification);
    granularEnvDecayLabel.setJustificationType(juce::Justification::topLeft);
    granularEnvDecayLabel.setInterceptsMouseClicks(false, false);
    addAndMakeVisible(granularEnvDecayLabel);
    
    granularEnvSustain.setRange(0.0f, 1.0f);
    granularEnvSustain.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    granularEnvSustain.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 100, 25);
    addAndMakeVisible(granularEnvSustain);
    
    granularEnvSustainLabel.setText("Sustain", juce::dontSendNotification);
    granularEnvSustainLabel.setJustificationType(juce::Justification::topLeft);
    granularEnvSustainLabel.setInterceptsMouseClicks(false, false);
    addAndMakeVisible(granularEnvSustainLabel);
    
    granularEnvRel.setRange(0.0f, 1.0f);
    granularEnvRel.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    granularEnvRel.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 100, 25);
    addAndMakeVisible(granularEnvRel);
    granularEnvRel.addListener(this);
    
    granularEnvRelLabel.setText("Release", juce::dontSendNotification);
    granularEnvRelLabel.setJustificationType(juce::Justification::topLeft);
    granularEnvRelLabel.setInterceptsMouseClicks(false, false);
    addAndMakeVisible(granularEnvRelLabel);
    
    granularLength.setRange(0.0f, 1.0f);
    granularLength.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    granularLength.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 100, 25);
    addAndMakeVisible(granularLength);
    granularLength.addListener(this);
    
    granularEnvLengthLabel.setText("Length", juce::dontSendNotification);
    granularEnvLengthLabel.setJustificationType(juce::Justification::topLeft);
    granularEnvLengthLabel.setInterceptsMouseClicks(false, false);
    addAndMakeVisible(granularEnvLengthLabel);
    
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
    auto bounds = getLocalBounds();
    
    auto volumeBounds = bounds.removeFromLeft(200);
    volumeLabel.setBounds(volumeBounds.removeFromTop(volumeBounds.getHeight()/2));
    volumeSlider    .setBounds(volumeBounds);
    
    auto comboBoxBounds = bounds.removeFromLeft(50);
    nominatorCombo  .setBounds(comboBoxBounds.removeFromTop(comboBoxBounds.getHeight()/2));
    denominatorCombo.setBounds(comboBoxBounds);
    
    auto timeSigBounds = bounds.removeFromLeft(30);
    timeSigLabel.setBounds(timeSigBounds);
    
    auto granularSlidersBounds = bounds;
    granularPosition.setBounds(granularSlidersBounds = bounds.removeFromLeft(100));
    granularPositionLabel.setBounds(granularSlidersBounds);
    granularEnvAttack.setBounds(granularSlidersBounds = bounds.removeFromLeft(100));
    granularEnvAttackLabel.setBounds(granularSlidersBounds);
    granularEnvDecay.setBounds(granularSlidersBounds = bounds.removeFromLeft(100));
    granularEnvDecayLabel.setBounds(granularSlidersBounds);
    //    granularEnvSustain.setBounds(granularSlidersBounds = bounds.removeFromLeft(100));
    //    granularEnvSustainLabel.setBounds(granularSlidersBounds);
    granularLength.setBounds(granularSlidersBounds = bounds.removeFromLeft(100));
    granularEnvLengthLabel.setBounds(granularSlidersBounds);
    granularEnvRel.setBounds(granularSlidersBounds = bounds.removeFromLeft(100));
    granularEnvRelLabel.setBounds(granularSlidersBounds);
}

void Rhythm::comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged)
{
    nominator = nominatorCombo.getSelectedItemIndex() + 1;
    denominator = denominatorCombo.getSelectedItemIndex()+ 1;
    updateTimeSig();
    updateSamplesPerBeat();
}

void Rhythm::sliderValueChanged (juce::Slider* slider)
{
    setEnvelopeParameters();
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

void Rhythm::setSampleRate(int sampleRate, int maxBufferSize)
{
    deviceSampleRate = sampleRate;
    envelope.setSampleRate(deviceSampleRate);
    setEnvelopeParameters();
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
                granularLengthInt = granularLength.getValue() * sampleBuffer.getNumSamples() - (granularPosition.getValue() * sampleBuffer.getNumSamples());
                envelope.noteOn();
            }
        }
    }
    
    if (playBeep)
    {
        int startPos = bufferPos + (granularPosition.getValue() * sampleBuffer.getNumSamples());
        int numSamplesToAdd = juce::jmin(sampleBuffer.getNumSamples()-startPos, numSamples);
           
        if (numSamples > 0)
        {
            juce::AudioBuffer<float> copyBuffer;
            
            copyBuffer.setSize(2, numSamplesToAdd);
            copyBuffer.copyFrom(0, 0, sampleBuffer, 0, startPos, numSamplesToAdd);
            copyBuffer.applyGain(volumeSlider.getValue());
            
            envelope.applyEnvelopeToBuffer(copyBuffer, 0, numSamplesToAdd);
            
            buffer.addFrom(0, 0, copyBuffer, 0, 0, numSamplesToAdd);
            if(buffer.getNumChannels() > 1)
                buffer.addFrom(1, 0, copyBuffer, 0, 0, numSamplesToAdd);
            
            bufferPos += numSamplesToAdd;
            
            if (bufferPos > granularLengthInt)
            {
                envelope.noteOff();
            }
        }
        else
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

void Rhythm::setEnvelopeParameters()
{
    juce::ADSR::Parameters newParameters;
    
    newParameters.attack = granularEnvAttack.getValue();
//    newParameters.sustain = granularEnvSustain.getValue();
    newParameters.decay = granularEnvDecay.getValue();
    newParameters.release = granularEnvRel.getValue();
    
    envelope.setParameters(newParameters);
}
