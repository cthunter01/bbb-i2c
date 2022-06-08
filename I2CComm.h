#ifndef I2CCOMM_H
#define I2CCOMM_H

#include <cstdint>

class I2CComm
{
public:
    I2CComm(std::uint8_t busNumber,
            std::uint8_t addr,
            const char* pathName = "/dev/i2c-");

    ~I2CComm();

    std::uint8_t readData(std::uint8_t reg,
                  std::uint8_t* buf,
                  std::uint8_t nbytes);

    void writeData(std::uint8_t reg,
                   std::uint8_t* buf,
                   std::uint8_t nbytes);


private:
    std::uint8_t bus;
    
    int filedes;
};

#endif // I2CCOMM_H