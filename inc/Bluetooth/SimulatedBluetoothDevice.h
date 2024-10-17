#ifndef SIMULATED_BLUETOOTH_DEVICE
#define SIMULATED_BLUETOOTH_DEVICE

#include "BluetoothDevice.h"

#include <cstdint>

class SimulatedBluetoothDevice : BluetoothDevice
{
    public:
        SimulatedBluetoothDevice(std::string macAddress);
        ~SimulatedBluetoothDevice() = default;

        bool connect() override;
        bool disconnect() override;
        bool sendData(const std::string& data) const override;
        const std::string receiveCommand() override;

    private:
        static const uint8_t COMMAND_MIN = 0;
        static const uint8_t COMMAND_MAX = 3;
        uint8_t commandIndex_;
};

#endif