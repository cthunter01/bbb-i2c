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
#include <i2c/smbus.h>

#include <string>
#include <iostream>
#include <iomanip>

DS3231::DS3231(std::uint8_t b, std::uint8_t a) :
    i2c_comm(std::make_unique<I2CComm>(b, a))
{

}

DS3231::~DS3231()
{
}

bool DS3231::initComms()
{
    return true;
}

void DS3231::writeTime(std::uint8_t year,
                       std::uint8_t month,
                       std::uint8_t date,
                       std::uint8_t day,
                       std::uint8_t hour,
                       std::uint8_t minute,
                       std::uint8_t second)
{
    // Create the BCD format expected by DS3231
    std::uint8_t buf[7];

    buf[0] = second / 10;
    buf[0] <<= 4;
    buf[0] += second % 10;

    buf[1] = minute / 10;
    buf[1] <<= 4;
    buf[1] += minute % 10;

    buf[2] = hour % 10;
    if((hour / 20) == 1)
    {
        // set the 20 hour bit
        buf[2] |= 0b00100000;
    }
    else if((hour / 10) == 1)
    {
        // set the 10 hour bit
        buf[2] |= 0b00010000;
    }

    buf[3] = day;

    buf[4] = date / 10;
    buf[4] <<= 4;
    buf[4] += date % 10;

    buf[5] = month / 10;
    buf[5] <<= 4;
    buf[5] += month % 10;
    
    buf[6] = year / 10;
    buf[6] <<= 4;
    buf[6] += year % 10;

    //std::uint8_t reg{0x0};
    //write(i2cbus, &reg, 1);
    i2c_comm->writeData(0x0, buf, 7);
    //write(i2cbus, buf, 8);

}

void DS3231::readTime()
{
    std::uint8_t buf[7];
    
    i2c_comm->readData(0x0, buf, 7);
    TimeStruct time;
    time.seconds = (buf[0] >> 4) & 0b00000111;
    time.seconds *= 10;
    time.seconds += buf[0] & 0b00001111;

    time.minutes = (buf[1] >> 4) & 0b00000111;
    time.minutes *= 10;
    time.minutes += buf[1] & 0b00001111;

    //time.hours

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