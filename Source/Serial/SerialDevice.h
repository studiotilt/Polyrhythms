#pragma once

#include "SerialPortListMonitor.h"

// NOTE: This class could be named for the name of the thing you are connecting to: LEDContllerDevice, DrumMachineDevice, SolarMonitorDevice, etc
class SerialDevice : private juce::Timer
{
public:
    SerialDevice (juce::String newSerialPortName);
    ~SerialDevice ();

private:
    juce::String serialPortName;
    std::unique_ptr<SerialPort> serialPort;
    std::unique_ptr<SerialPortInputStream> serialPortInput;
    std::unique_ptr<SerialPortOutputStream> serialPortOutput;

    bool openSerialPort (void);
    void closeSerialPort (void);

    void timerCallback () override;
};
