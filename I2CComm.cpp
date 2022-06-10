#include "I2CComm.h"

// For calloc()
#include <cstdlib>

// For memcpy
#include <cstring>

// For open()
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

// For close()
#include <unistd.h>

// For perror()
#include <stdio.h>
#include <errno.h>

// For ioctl()
#include <sys/ioctl.h>

// For I2C
#include <linux/i2c-dev.h>

#include <string>

I2CComm::I2CComm(std::uint8_t inBus,
                 std::uint8_t inAddr,
                 const char* inPathName) :
    bus(inBus),
    addr(inAddr),
    filedes(0),
    pathName(inPathName)
{
    pathName += std::to_string(bus);
    // Try and open the bus
    filedes = open(pathName.c_str(), O_RDWR);
    if(filedes < 0)
    {
        perror("Failed to open the i2c bus");
    }

    // initiate communication with BPM280
    // to read, last bit = 1. To write last bit = 0;
    if(ioctl(filedes, I2C_SLAVE, addr) < 0)
    {
        perror("Failed to acquire bus access and/or talk to slave");
    }
}

I2CComm::~I2CComm()
{
    close(filedes);
}

std::uint8_t I2CComm::readData(std::uint8_t reg,
                               std::uint8_t* buf,
                               std::uint8_t nBytes)
{

    std::uint8_t bytesRead{0};

    // write to the device the register to start reading from
    write(filedes, &reg, 1);

    // Read data into the buffer
    bytesRead = read(filedes, buf, nBytes);

    return bytesRead;
}

void I2CComm::writeData(std::uint8_t reg,
                        std::uint8_t* buf,
                        std::uint8_t nbytes)
{
    std::uint8_t* temp = static_cast<std::uint8_t*>(std::calloc(nbytes + 1, sizeof(std::uint8_t)));
    buf[0] = reg;
    std::memcpy(&temp[1], buf, nbytes);
    write(filedes, temp, nbytes+1);
    std::free(temp);
}

