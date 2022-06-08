#ifndef BMP280_H
#define BMP280_H

#include <cstdint>

#include "I2CComm.h"

// The address of the BMP280 is either 0x76 or 0x77
// depending on the SDO pin pulled low or high (respectively)
//constexpr std::uint8_t addr{0x76};

enum class BMP280_OSRS_P
{
    SKIPPED  = 0,
    ULPOWER  = 1,
    LPOWER   = 2,
    STANDARD = 3,
    HIRES    = 4,
    UHIRES   = 5
};

enum class BMP280_OSRS_T
{
    SKIPPED  = 0,
    ULPOWER  = 1,
    LPOWER   = 2,
    STANDARD = 3,
    HIRES    = 4,
    UHIRES   = 5
};

enum class BMP280_MODE
{
    SLEEP  = 0,
    FORCED = 1,
    NORMAL = 2
};

class BMP280
{
public:
    BMP280(std::uint8_t b, std::uint8_t addr);
    ~BMP280();

    void readCalibration();
    void writeConfig();
    void takeMeasurement();
    //double calculateTrueTemperatureDouble(long ut);
    std::int32_t calculateTrueTemperature(std::int32_t ut);
    std::uint32_t calculateTruePressure(std::int32_t up);

    typedef struct
    {
        std::uint16_t T1;
	std::int16_t  T2;
	std::int16_t  T3;
	std::uint16_t P1;
	std::int16_t  P2;
	std::int16_t  P3;
	std::int16_t  P4;
	std::int16_t  P5;
	std::int16_t  P6;
	std::int16_t  P7;
	std::int16_t  P8;
	std::int16_t  P9;
    } BMP280CalibrationStruct;

    BMP280CalibrationStruct calData;

    BMP280_MODE mode;
    BMP280_OSRS_P osrs_p;
    BMP280_OSRS_T osrs_t;

    std::int32_t t_fine;

    I2CComm i2c_comm;

};

#endif // BMP280_H
