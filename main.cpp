
// C++ Headers
#include <iostream>

#include "BMP280.h"

int main(int argc, char* argv[])
{
    BMP280 bmp280(2);
    if(bmp280.initComms())
    {
        bmp280.readCalibration();
        //bmp280.writeConfig();
        bmp280.takeMeasurement();
    }
    // Try and open the bus

    return 0;
}

