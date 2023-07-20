 #include "SerialDevice.h"

#define kBPS 9600
const auto kNumberOfDecimalPlaces { 4 };


const int kMaxPayloadSize = 20;


SerialDevice::SerialDevice (juce::String newSerialPortName)
{
    serialPortName = newSerialPortName;
    openSerialPort();
    startTimer (5);
}

SerialDevice::~SerialDevice ()
{
    stopTimer();
    closeSerialPort ();
}


bool SerialDevice::openSerialPort ()
{
    serialPort = std::make_unique<SerialPort> ([] (juce::String, juce::String) {});
    bool opened = serialPort->open (serialPortName);

    if (opened)
    {
        SerialPortConfig serialPortConfig;
        serialPort->getConfig (serialPortConfig);
        serialPortConfig.bps = kBPS;
        serialPortConfig.databits = 8;
        serialPortConfig.parity = SerialPortConfig::SERIALPORT_PARITY_NONE;
        serialPortConfig.stopbits = SerialPortConfig::STOPBITS_1;
        serialPort->setConfig (serialPortConfig);

        serialPortInput = std::make_unique<SerialPortInputStream> (serialPort.get());
        serialPortOutput = std::make_unique<SerialPortOutputStream> (serialPort.get ());
        juce::Logger::outputDebugString ("Serial port: " + serialPortName + " opened");
    }
    else
    {
        // report error
        juce::Logger::outputDebugString ("Unable to open serial port:" + serialPortName);
    }

    return opened;
}

void SerialDevice::closeSerialPort ()
{
    serialPortOutput = nullptr;
    serialPortInput = nullptr;
    if (serialPort != nullptr)
    {
        serialPort->close ();
        serialPort = nullptr;
    }
}


#define kSerialPortBufferLen 256
void SerialDevice::timerCallback ()
{
    // handle reading from the serial port
    if ((serialPortInput != nullptr) && (!serialPortInput->isExhausted ()))
    {
        auto  bytesRead = 0;
        auto  dataIndex = 0;
        char incomingData [kSerialPortBufferLen];

        bytesRead = serialPortInput->read (incomingData, kSerialPortBufferLen);
        {
            while (dataIndex < bytesRead)
            {
                if (incomingData[dataIndex] == '[')
                {
                    
                    if (bytesRead - dataIndex >= 12)
                    {
                        
                    }
                    else
                    {
                        break;
                    }
                    
                    // get the first array
                    int index = dataIndex;
                    while (incomingData[index] != ']')
                    {
                        index++;
                    }
                    
                    juce::String newString;
                    for (int startIndex = dataIndex; startIndex <= index; startIndex++)
                    {
                        newString+=incomingData[startIndex];
                    }
                    
                    DBG(newString);
                    
                    dataIndex = index+1;
                    continue;
                }
                dataIndex++;
            }
        }
    }
}

