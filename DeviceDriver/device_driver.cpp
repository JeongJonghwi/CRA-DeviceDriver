#include "device_driver.h"

class ReadFailException : public std::exception {
public:
    explicit ReadFailException(string _Message) :
        _Message{ _Message } {
    }
private:
    string _Message;
};

DeviceDriver::DeviceDriver(FlashMemoryDevice* hardware) : m_hardware(hardware)
{
}

int DeviceDriver::read(long address)
{
    int data = (int)(m_hardware->read(address));
    for (int i = 0; i < 4; i++) {
        if (data != (int)(m_hardware->read(address)))
            throw ReadFailException("read fail exception");
    }
    return data;
}

void DeviceDriver::write(long address, int data)
{
    // TODO: implement this method
    m_hardware->write(address, (unsigned char)data);
}