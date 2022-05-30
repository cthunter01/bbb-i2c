#include "DS3231.h"

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
#include <iostream>
#include <iomanip>

DS3231::DS3231(std::uint8_t b) :
    bus(b),
    i2cbus(-1)
{

}

DS3231::~DS3231()
{
    close(i2cbus);
}

bool DS3231::initComms()
{
    std::string i2cbusName("/dev/i2c-");
    i2cbusName += std::to_string(bus);
    bool retVal{true};

    // Try and open the bus
    i2cbus = open(i2cbusName.c_str(), O_RDWR);
    if(i2cbus < 0)
    {
        perror("Failed to open the i2c bus");
        retVal = false;
    }

    // initiate communication with DS3231
    if(ioctl(i2cbus, I2C_SLAVE, ds3231_addr) < 0)
    {
        perror("Failed to acquire bus access and/or talk to slave");
        retVal = false;
    }
    return retVal;
}

void DS3231::writeTime(std::uint8_t year,
                       std::uint8_t month,
                       std::uint8_t date,
                       std::uint8_t day,
                       std::uint8_t hour,
                       std::uint8_t minute,
                       std::uint8_t second)
{
    std::uint8_t reg{0x0};
    write(i2cbus, &reg, 1);

}

void DS3231::readTime()
{
    std::uint8_t reg{0x0};
    std::uint8_t buf[7];
    write(i2cbus, &reg, 1);
    
    read(i2cbus, buf, 7);
    TimeStruct time;
    time.seconds = (buf[0] >> 4) & 0b00000111;
    time.seconds *= 10;
    time.seconds += buf[0] & 0b00001111;

    time.minutes = (buf[1] >> 4) & 0b00000111;
    time.minutes *= 10;
    time.minutes += buf[1] & 0b00001111;

    time.hours

    std::cout << "time: "
              << std::setfill('0')
              << std::setw(2)
              << static_cast<unsigned>(time.minutes)
              << ":"
              << std::setfill('0')
              << std::setw(2)
              << static_cast<unsigned>(time.seconds)
              << std::endl;
}