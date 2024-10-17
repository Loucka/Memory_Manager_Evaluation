cmake_minimum_required(VERSION 3.10)
project(BluetoothMemoryManager)

set(CMAKE_CXX_STANDARD 17)

include_directories(inc
    inc/Bluetooth
    inc/Reporting
    inc/Memory)

add_executable(test_memory_pool
    src/Memory/MemoryPool.cpp
    src/Utils.cpp
    tests/test_MemoryPool.cpp
)

add_executable(test_bluetooth_manager
    src/Bluetooth/BluetoothManager.cpp
    src/Bluetooth/BluetoothDevice.cpp
    src/Bluetooth/SimulatedBluetoothDevice.cpp
    src/Memory/NvmManager.cpp
    src/Memory/FakeNvmManager.cpp
    src/Reporting/EventLogger.cpp
    src/Utils.cpp
    tests/test_BluetoothManager.cpp)

add_executable(test_event_logging
    src/Reporting/EventLogger.cpp
    src/Utils.cpp
    tests/test_EventLogger.cpp)

add_executable(test_memory_manager
    src/Bluetooth/BluetoothManager.cpp
    src/Bluetooth/BluetoothDevice.cpp
    src/Bluetooth/SimulatedBluetoothDevice.cpp
    src/Memory/NvmManager.cpp
    src/Memory/FakeNvmManager.cpp
    src/Memory/MemoryPool.cpp
    src/Memory/MemoryManager.cpp
    src/Reporting/EventLogger.cpp
    src/Utils.cpp
    tests/test_MemoryManager.cpp)

# TODO: Add executable for bluetooth device tests.
# TODO: Add executable for NVM tests.
# TODO: Add executable for Utility test