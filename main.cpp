// For open()
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

// For close()
#include <unistd.h>

// For perror()
#include <stdio.h>
#include <errno.h>

// C++ Headers
#include <iostream>
#include <string>

std::string i2cbus{"/dev/i2c-2"};

// BMP180 sampling accuracy modes
// MODE        #samples | time (ms) | current (mA) | RMS noise (hPa | m)
// ULTRALOW    1          4.5         3              0.06    0.5
// STANDARD    2          7.5         5              0.05    0.4
// HIGH        4          13.5        7              0.04    0.3
// ULTRAHIGH   8          25.5        12             0.03    0.25
enum class BMP180Mode
{
    ULTRALOWPOWER = 0,
    STANDARD      = 1,
    HIGH          = 2,
    ULTRAHIGH     = 3,
};

int main(int argc, char* argv[])
{
    // Try and open the bus
    int file = open(i2cbus.c_str(), O_RDWR);
    if(file < 0)
    {
        perror("Failed to open the i2c bus");
        exit(1);
    }

    // From the data sheet:
    // 1. Start
    // 2. read calibration data from EEPROM
    // 3. start temperature measurement
    //    -- wait 4.5 ms
    // 4. read temperature
    // 5. start pressure measurement
    // 6. wait (depends on mode for time)
    // 7. read pressure
    // 8. calculate pressure using temperature and calibration data



    close(file);

    return 0;
}