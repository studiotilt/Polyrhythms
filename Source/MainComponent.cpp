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
    
    // Make sure you set the size of the component after
    // you add any child components.
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

    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.

    // For more details, see the help for AudioProcessor::prepareToPlay()
    
    // force the bpm to 120 for now
    samplesPerBeat = sampleRate*60.0f / bpm;
    for (auto rhythm : rhythms)
    {
        rhythm->setSamplesPerBeat(samplesPerBeat);
    }
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    // Your audio-processing code goes here!

    // For more details, see the help for AudioProcessor::getNextAudioBlock()

    // Right now we are not producing any data, in which case we need to clear the buffer
    // (to prevent the output of random noise)
    bufferToFill.clearActiveBufferRegion();
    if (!playing) return;
    
    bufferPos+=bufferToFill.buffer->getNumSamples();
    
    for(auto rhythm : rhythms)
    {
        rhythm->getNextBlock(*bufferToFill.buffer);
    }
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    // You can add your drawing code here!
}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    startButton.setBounds(0, 0, 50, 40);
    int y = 40;
    for(auto rhythm : rhythms)
    {
        rhythm->setBounds(0, y, getWidth(), 50);
        y += 60;
    }
}

void MainComponent::buttonClicked(juce::Button* button)
{
    if (button == &startButton)
    {
        if (button->getToggleState())
        {
            playing = true;
        }
        else
        {
            playing = false;
            bufferPos = 0;
            for (auto rhythm : rhythms)
            {
                rhythm->restart();
            }
        }
    }
}
