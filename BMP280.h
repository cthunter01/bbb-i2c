#ifndef BMP280_H
#define BMP280_H

#include <cstdint>

// The address of the BMP280 is either 0x76 or 0x77
// depending on the SDO pin pulled low or high (respectively)
constexpr std::uint8_t addr{0x76};

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
    BMP280(std::uint8_t b);
    ~BMP280();

    bool initComms();
    void readCalibration();
    void writeConfig();
    void takeMeasurement();

    typedef struct
    {
        unsigned short T1;
        short          T2;
        short          T3;
        unsigned short P1;
        short          P2;
        short          P3;
        short          P4;
        short          P5;
        short          P6;
        short          P7;
        short          P8;
        short          P9;
    } BMP280CalibrationStruct;

    BMP280CalibrationStruct calData;

    std::uint8_t bus;

    BMP280_MODE mode;
    BMP280_OSRS_P osrs_p;
    BMP280_OSRS_T osrs_t;

    int i2cbus;

};

#endif // BMP280_H