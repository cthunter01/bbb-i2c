#include <unistd.h>

// C++ Headers
#include <iostream>

#include "BMP280.h"
#include "DS3231.h"

int main(int argc, char* argv[])
{

    BMP280 bmp280(2, 0x76);
    if(true /*bmp280.initComms()*/)
    {
        bmp280.readCalibration();
        //bmp280.writeConfig();
        bmp280.takeMeasurement();
    }
/*
    DS3231 ds3231(2);
    if(ds3231.initComms())
    {

        ds3231.writeTime(22,
                         5,
                         30,
                         2,
                         17,
                         57,
                         0);

        for(int i = 0; i < 60; ++i)
        {
            usleep(1000000);
            ds3231.readTime();
        }
    }
*/
    return 0;
}

