#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    
    audioDeviceSelector = std::make_unique<juce::AudioDeviceSelectorComponent>
    (deviceManager,
     1,     // int minAudioInputChannels,
     2,     // int maxAudioInputChannels,
     2,     // int minAudioOutputChannels,
     16,     // int maxAudioOutputChannels,
     false, // bool showMidiInputOptions,
     false, // bool showMidiOutputSelector,
     false,  // bool showChannelsAsStereoPairs,
     true  // bool hideAdvancedOptionsWithButton
     );
    addAndMakeVisible(*audioDeviceSelector);
    
    
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
    
    serialCombo.addListener(this);
    addAndMakeVisible(serialCombo);
    
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
    
    if(serialPortListMonitor.hasListChanged())
        juce::MessageManager::callAsync([this]() { updateSerialComboBox(); });
    
    if (serialDevice.get())
    {
        juce::Array<int> values = serialDevice->getCurrentValues();
        for (int value : values)
            DBG(value);
    }
    
    if (!playing) return;
    int numChannels = bufferToFill.buffer->getNumChannels();
    
    for(int i = 0; i < rhythms.size(); i++)
    {
        auto rhythm = rhythms[i];
        rhythm->getNextBlock(*bufferToFill.buffer, i%numChannels);
    }
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
    
    serialCombo.setBounds(0, y, 40, 40);
    
    audioDeviceSelector->setBounds(getLocalBounds().removeFromBottom(audioDeviceSelector->getHeight()));
}

void MainComponent::buttonClicked(juce::Button* button)
{
    playing = button->getToggleState();
    
    if (!playing)
        for (auto rhythm : rhythms)
            rhythm->restart();
}

void MainComponent::comboBoxChanged(juce::ComboBox *comboBoxThatHasChanged)
{
    juce::StringArray devices = serialPortListMonitor.getSerialPortList().getAllValues();
    juce::String selectedDevice = devices[serialCombo.getSelectedItemIndex()];
    
    if (currentSerialDevice != selectedDevice)
    {
        currentSerialDevice = selectedDevice;
        updateSelectedSerialDevice();
    }
    
    DBG(currentSerialDevice);
}

void MainComponent::updateSerialComboBox()
{
    serialCombo.clear();
    juce::StringArray devices = serialPortListMonitor.getSerialPortList().getAllValues();
    
    for(int i = 0; i < devices.size(); i++)
    {
        auto device = devices[i];
        serialCombo.addItem(device, i+1);
    }
}

void MainComponent::updateSelectedSerialDevice()
{
    serialDevice = std::make_unique<SerialDevice>(currentSerialDevice);
}
