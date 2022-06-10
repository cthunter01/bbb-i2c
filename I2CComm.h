#ifndef I2CCOMM_H
#define I2CCOMM_H

#include <cstdint>

class I2CComm
{
public:
    I2CComm(std::uint8_t inBus,
            std::uint8_t inAddr,
            const char* inPathName = "/dev/i2c-");

    ~I2CComm();

    std::uint8_t readData(std::uint8_t reg,
                  std::uint8_t* buf,
                  std::uint8_t nbytes);

    void writeData(std::uint8_t reg,
                   std::uint8_t* buf,
                   std::uint8_t nbytes);


private:
    std::uint8_t bus;

    std::uint8_t addr;
    
    int filedes;

    std::string pathName;
};

#endif // I2CCOMM_H