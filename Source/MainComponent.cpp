#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    startButton.setButtonText("Start");
    startButton.setToggleable(true);
    startButton.setClickingTogglesState(true);
    startButton.addListener(this);
    addAndMakeVisible(startButton);
    for (int i = 0; i < 8; i++)
    {
        rhythms.add(new Rhythm(BinaryData::namedResourceList[i]));
        addAndMakeVisible(rhythms[i]);
    }
    
    setSize (800, 600);

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.

    samplesPerBeat = sampleRate*60.0f / bpm;
    for (auto rhythm : rhythms)
        rhythm->setSamplesPerBeat(samplesPerBeat);
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    // Your audio-processing code goes here!

    bufferToFill.clearActiveBufferRegion();
    
    if (!playing) return;
    
    for(auto rhythm : rhythms)
        rhythm->getNextBlock(*bufferToFill.buffer);
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    
    startButton.setBounds(0, 0, 50, 40);
    
    int y = 40;
    for(auto rhythm : rhythms)
    {
        rhythm->setBounds(0, y, getWidth(), 80);
        y += 80;
    }
}

void MainComponent::buttonClicked(juce::Button* button)
{
    playing = button->getToggleState();
    
    if (!playing)
        for (auto rhythm : rhythms)
            rhythm->restart();
}
