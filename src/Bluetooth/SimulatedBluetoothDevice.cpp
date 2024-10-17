/// SimulatedBluetoothDevice.cpp

#include "SimulatedBluetoothDevice.h"
#include "Utils.h"

SimulatedBluetoothDevice::SimulatedBluetoothDevice(std::string macAddress): BluetoothDevice(macAddress)
{
    commandIndex_ = COMMAND_MIN;
}

bool SimulatedBluetoothDevice::connect()
{
    // Assume successful connection for simulation.
    isConnected_ = true;
    return isConnected_;
}

bool SimulatedBluetoothDevice::disconnect()
{
    isConnected_ = false;
    return isConnected_;
}

bool SimulatedBluetoothDevice::sendData(const std::string& data) const
{
    //#TODO: Serialize the data and send it on its way to the handling peripheral.
    //# Ideally, the serialized data is sent to a queue that is serviced by an async utility for actual sending.
    return true;
}

const std::string SimulatedBluetoothDevice::receiveCommand()
{
    //#TODO: Poll the peripheral and deserialize any available data.
    //# Ideally, receiving of commands is done async via interrupt handling that permits final handling in MemoryManager.

    // Implements a crude way of cycling through simulated commands.
    std::string command = "";

    switch (commandIndex_)
    {
        case COMMAND_MIN:
            command = COMMAND_ALLOCATE;
            break;
        case 1:
            command = COMMAND_DEALLOCATE;
            break;
        case 2:
            command = COMMAND_COMPACT;
            break;
        case COMMAND_MAX:
            command = COMMAND_REPORT;
            break;
        default:
            // Do nothing.
            break;
    }

    commandIndex_ = COMMAND_MAX == commandIndex_ ? COMMAND_MIN : commandIndex_ + 1;

    return command;
    
}