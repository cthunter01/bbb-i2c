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

BMP280::BMP280(std::uint8_t b, std::uint8_t addr) :
    calData(),
    mode(BMP280_MODE::FORCED),
    osrs_p(BMP280_OSRS_P::STANDARD),
    osrs_t(BMP280_OSRS_T::STANDARD),
    i2c_comm(b, addr)
{
}

BMP280::~BMP280()
{
}

void BMP280::readCalibration()
{
    // Send a soft reset
    // first write 0x0 to the device to reset internal ptr
    //std::uint8_t reg{0x0};
    //write(i2cbus, &reg, 1);

    // Now set the register to 0xE0 and write 0xB6
    //std::uint8_t temp[2];
    //temp[0] = 0xE0;
    //temp[1] = 0xB6;
    //write(i2cbus, temp, 2);

    // The calibration data is 24 bytes. From 0x88-0x9F
    std::uint8_t buf[24];
    //reg = 0x88;

    i2c_comm.readData(0x88, buf, 24);

    calData.T1 = (buf[1] << 8) | buf[0];
    calData.T2 = (buf[3] << 8) | buf[2];
    calData.T3 = (buf[5] << 8) | buf[4];

    calData.P1 = (buf[7] << 8) | buf[6];
    calData.P2 = (buf[9] << 8) | buf[8];
    calData.P3 = (buf[11] << 8) | buf[10];
    calData.P4 = (buf[13] << 8) | buf[12];
    calData.P5 = (buf[15] << 8) | buf[14];
    calData.P6 = (buf[17] << 8) | buf[16];
    calData.P7 = (buf[19] << 8) | buf[18];
    calData.P8 = (buf[21] << 8) | buf[20];
    calData.P9 = (buf[23] << 8) | buf[22];

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
    //std::uint8_t buf[2];
    //buf[0] = 0xF5;
    //buf[1] = 0x0;
    std::uint8_t data = 0x0;
    i2c_comm.writeData(0xF5, &data, 1);
    //write(i2cbus, buf, 2);
}

void BMP280::takeMeasurement()
{
    writeConfig();
    std::uint8_t buffer[2];
    std::uint8_t reg{0x0};
    //write(i2cbus, &reg, 1);
    //reg = 0xF4;
    //write(i2cbus, &reg, 1);
    //std::uint8_t val = (osrs_t << 5) | (osrs_p << 2) | mode;
    std::uint8_t val = (5 << 5) | (5 << 2) | 1;
    buffer[0] = 0xF4;
    buffer[1] = val;
    //write(i2cbus, buffer, 2);

    i2c_comm.writeData(0xF4, &val, 1);
    usleep(1000000);
    reg = 0xF7;
    //write(i2cbus, &reg, 1);
    std::uint8_t buf[6];
    
    if(i2c_comm.readData(reg, buf, 6) < 6)
    {
        std::cout << "error reading bmp280\n";
    }
    std::uint32_t temp[3];
    std::int32_t  ut{0};
    std::int32_t up{0};
    
    // get raw pressure measurement
    up = ((buf[0] << 16) | (buf[1] << 8) | buf[0]) >> 4;

    // get raw temperature measurement
    ut = ((buf[3] << 16) | (buf[4] << 8) | buf[5]) >> 4;

    std::cout << "up = " << std::to_string(up) << std::endl;
    std::cout << "ut = " << std::to_string(ut) << std::endl;
    std::cout << "T = " << std::to_string(calculateTrueTemperature(ut)) << std::endl;
    std::cout << "P = " << std::to_string(calculateTruePressure(up)) << std::endl;
    std::cout << "P2 = " << ((double)calculateTruePressure(up)/256.0) << std::endl;

}

std::int32_t BMP280::calculateTrueTemperature(std::int32_t ut)
{
	std::int32_t var1, var2, T;
	var1 = ((((ut>>3) - ((std::int32_t)calData.T1<<1))) * ((std::int32_t)calData.T2)) >> 11;
	var2 = (((((ut>>4) - ((std::int32_t)calData.T1)) * ((ut>>4) - ((std::int32_t)calData.T1))) >> 12) * ((std::int32_t)calData.T3)) >> 14;
	t_fine = var1 + var2;
	T = (t_fine * 5 + 128) >> 8;
	return T;
}

std::uint32_t BMP280::calculateTruePressure(std::int32_t up)
{
std::int64_t var1, var2, p;
var1 = ((std::int64_t)t_fine) - 128000;
var2 = var1 * var1 * (std::int64_t)calData.P6;
var2 = var2 + ((var1*(std::int64_t)calData.P5)<<17);
var2 = var2 + (((std::int64_t)calData.P4)<<35);
var1 = ((var1 * var1 * (std::int64_t)calData.P3)>>8) + ((var1 * (std::int64_t)calData.P2)<<12);
var1 = (((((std::int64_t)1)<<47)+var1))*((std::int64_t)calData.P1)>>33;
if(var1 == 0)
{
return 0;
}
p = 1048576 - up;
p = (((p<<31)-var2)*3125)/var1;
var1 = (((std::int64_t)calData.P9) * (p>>13) * (p>>13)) >> 25;
var2 = (((std::int64_t)calData.P8) * p) >> 19;
p = ((p + var1 + var2) >> 8) + (((std::int64_t)calData.P7)<<4);
return (std::uint32_t)p;
}

