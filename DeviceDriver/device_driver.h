#pragma once
#include "flash_memory_device.h"
#include <string>

using std::string;

class DeviceDriver
{
public:
    DeviceDriver(FlashMemoryDevice* hardware);
    int read(long address);
    void write(long address, int data);

protected:
    FlashMemoryDevice* m_hardware;
};

class ReadFailException : public std::exception {
public:
    explicit ReadFailException(char const* _Message) :
        exception{ _Message } {
    }
};

class WriteFailException : public std::exception {
public:
    explicit WriteFailException(char const* _Message) :
        exception{ _Message } {
    }
};