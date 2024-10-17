/// test_BluetoothManager.cpp

#include <cassert>

#include "BluetoothManager.h"
#include "Utils.h"

BluetoothManager* setup()
{
    return BluetoothManager::getInstance();
}

BluetoothManager* teardown()
{
    return BluetoothManager::getInstance(true);
}

void testSingleton()
{
    // Create and destroy the singleton many times while monitoring memory usage.
    //#TODO: Develop a more automated way. A strange memory leak also appears to be present.
    BluetoothManager *pManager = nullptr;
    for (int i = 0; i < 100000; i++)
    {
        pManager = setup();
        assert(nullptr != pManager);
        pManager = teardown();
        assert(nullptr == pManager);
    }
}

void testScan()
{
    BluetoothManager *pManager = setup();

    std::vector<DeviceInfo> devices = pManager->scan();
    assert(2 == devices.size()); //#TODO: Further assert device information.

    teardown();
}

void testLoad()
{
    DeviceInfo *pInfo = setup()->getDeviceInfo();

    assert(pInfo->autoConnect);
    assert("Test_Device" == pInfo->deviceName);
    assert("01:23:45:67:89:AB" == pInfo->macAddress);

    teardown();
}

void testPairing()
{
    BluetoothManager *pManager = setup();
    DeviceInfo *pInfo = pManager->getDeviceInfo();

    std::string device = "Device_1";
    std::string mac = "2C:54:91:88:C9:E3";

    DeviceInfo testInfo =  { false, device, mac };

    // Verify updated device info post pairing.
    assert(pManager->pairDevice(testInfo));
    assert(pInfo->autoConnect);
    assert(device == pInfo->deviceName);

    // Verify updated autoconnect post unpairing.
    assert(pManager->unpairDevice());
    assert(!pInfo->autoConnect);

    teardown();
}

void testSend()
{
    BluetoothManager *pManager = setup();

    assert(pManager->sendData("TEST"));

    teardown();
}

void testReceive()
{
    BluetoothManager *pManager = setup();

    assert(COMMAND_ALLOCATE == pManager->receiveCommand());
    assert(COMMAND_DEALLOCATE == pManager->receiveCommand());
    assert(COMMAND_COMPACT == pManager->receiveCommand());
    assert(COMMAND_REPORT == pManager->receiveCommand());
    assert(COMMAND_ALLOCATE == pManager->receiveCommand());

    teardown();
}

int main()
{
    log_info("BTMANAGER", "----------------BEGINNING TESTS------------------");

    testSingleton();
    log_info("BTMANAGER", "----------------SINGLETON PASSED------------------");

    testScan();
    log_info("BTMANAGER", "----------------SCAN PASSED------------------");

    testLoad();
    log_info("BTMANAGER", "----------------LOAD PASSED------------------");

    //#TODO: Test saving.

    testPairing();
    log_info("BTMANAGER", "----------------PAIRING PASSED------------------");

    testSend();
    log_info("BTMANAGER", "----------------SEND PASSED------------------");

    testReceive();
    log_info("BTMANAGER", "----------------RECEIVE PASSED------------------");

    log_info("BTMANAGER", "----------------END TESTS------------------");
    return 0;
}