#include "BMP280.h"

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

BMP280::BMP280(std::uint8_t b) :
    calData(),
    bus(b),
    mode(BMP280_MODE::FORCED),
    osrs_p(BMP280_OSRS_P::STANDARD),
    osrs_t(BMP280_OSRS_T::STANDARD),
    i2cbus(-1)
{
}

BMP280::~BMP280()
{
    close(i2cbus);
}

bool BMP280::initComms()
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

    // initiate communication with BPM280
    // to read, last bit = 1. To write last bit = 0;
    if(ioctl(i2cbus, I2C_SLAVE, addr) < 0)
    {
        perror("Failed to acquire bus access and/or talk to slave");
        retVal = false;
    }
    return retVal;
}
void BMP280::readCalibration()
{
    // Send a soft reset
    // first write 0x0 to the device to reset internal ptr
    std::uint8_t reg{0x0};
    write(i2cbus, &reg, 1);
    // Now set the register to 0xE0
    reg = 0xE0;
    write(i2cbus, &reg, 1);
    // Now perform the soft reset by writing 0xB6 to 0xE0
    std::uint8_t val{0xB6};
    write(i2cbus, &val, 1);

    // The calibration data is 24 bytes. From 0x88-0x9F
    char buf[24];
    reg = 0x88;
    write(i2cbus, &reg, 1);
    read(i2cbus, buf, 24);

    calData.T1 = (buf[0] << 8) | buf[1];
    
    calData.T2 = (buf[2] << 8) | buf[3];
    //calData.T2 = ~calData.T2 + 1;

    calData.T3 = (buf[4] << 8) | buf[5];
    //calData.T3 = ~calData.T3 + 1;

    calData.P1 = (buf[6] << 8) | buf[7];

    calData.P2 = (buf[8] << 8) | buf[9];
    //calData.P2 = ~calData.P2 + 1;

    calData.P3 = (buf[10] << 8) | buf[11];
    //calData.P3 = ~calData.P3 + 1;

    calData.P4 = (buf[12] << 8) | buf[13];
    //calData.P4 = ~calData.P4 + 1;

    calData.P5 = (buf[14] << 8) | buf[15];
    //calData.P5 = ~calData.P5 + 1;

    calData.P6 = (buf[16] << 8) | buf[17];
    //calData.P6 = ~calData.P6 + 1;

    calData.P7 = (buf[18] << 8) | buf[19];
    //calData.P7 = ~calData.P7 + 1;

    calData.P8 = (buf[20] << 8) | buf[21];
    //calData.P8 = ~calData.P8 + 1;

    calData.P9 = (buf[22] << 8) | buf[23];
    //calData.P9 = ~calData.P9 + 1;

    // Print the calibration data
    std::cout << "calData.T1: " << calData.T1 << std::endl;
    std::cout << "calData.T2: " << calData.T2 << std::endl;
    std::cout << "calData.T3: " << calData.T3 << std::endl;
    std::cout << "calData.P1: " << calData.P1 << std::endl;
    std::cout << "calData.P2: " << calData.P2 << std::endl;
    std::cout << "calData.P3: " << calData.P3 << std::endl;
    std::cout << "calData.P4: " << calData.P4 << std::endl;
    std::cout << "calData.P5: " << calData.P5 << std::endl;
    std::cout << "calData.P6: " << calData.P6 << std::endl;
    std::cout << "calData.P7: " << calData.P7 << std::endl;
    std::cout << "calData.P8: " << calData.P8 << std::endl;
    std::cout << "calData.P9: " << calData.P9 << std::endl;
}

void BMP280::writeConfig()
{
    // Writes zero to the config register.
    // 0.5 ms standby
    // no filter
    // No SPI
    std::uint8_t buf[2];
    buf[0] = 0xF5;
    buf[1] = 0x0;
    write(i2cbus, buf, 2);
}

void BMP280::takeMeasurement()
{
    writeConfig();
    std::uint8_t buffer[2];
    std::uint8_t reg{0x0};
    std::uint8_t val{0x0};

    //val = (osrs_t << 5) | (osrs_p << 2) | mode;
    val = (3 << 5) | (3 << 2) | 1;
    write(i2cbus, &reg, 1);
    //reg = 0xF4;
    //write(i2cbus, &reg, 1);
    buffer[0] = 0xF4;
    buffer[1] = val;
    write(i2cbus, buffer, 2);

    usleep(100000);
    std::uint8_t buf[6];
    if(read(i2cbus, buf, 6) < 6)
    {
        std::cout << "error reading bmp280\n";
    }
    std::uint32_t ut{0};
    std::uint32_t up{0};
    up = (buf[0] << 8) | buf[1];
    std::cout << "up = " << std::to_string(up) << std::endl;
}