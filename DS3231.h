#ifndef DS3231_H
#define DS3231_H
#include <cstdint>

#include <memory>

#include "I2CComm.h"

// The address of the DS3231 is 
constexpr std::uint8_t ds3231_addr{0x68};

class DS3231
{
public:
    DS3231(std::uint8_t b, std::uint8_t a);
    ~DS3231();

    bool initComms();
    void readTime();
    void writeTime(std::uint8_t year,
                   std::uint8_t month,
                   std::uint8_t date,
                   std::uint8_t day,
                   std::uint8_t hour,
                   std::uint8_t minute,
                   std::uint8_t second);

    typedef struct
    {
        std::uint8_t seconds;
        std::uint8_t minutes;
        std::uint8_t hour;
        std::uint8_t day;
        std::uint8_t date;
        std::uint8_t month;
        std::uint8_t year;
    } TimeStruct;

private:
    std::unique_ptr<I2CComm> i2c_comm;

};

#endif // DS3231_H