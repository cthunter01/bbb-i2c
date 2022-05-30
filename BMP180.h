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

typedef struct
{
    short AC1;
    short AC2;
    short AC3;
    std::uint16_t AC4;
    std::uint16_t AC5;
    std::uint16_t AC6;
    short B1;
    short B2;
    short MB;
    short MC;
    short MD;
} BMP180CalibrationStruct;

/*
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

    BMP180CalibrationStruct cal;
    assert(sizeof(cal) == 22);
    assert(sizeof(short) == 2);

    char calBuffer[22];

    // read the whole calibration
    // First write to BMP180 the register to point to.
    // Reset to 0x0, then set to 0xAA, since the calibration offset is 0xAA
    std::uint8_t offset = 0x0;
    if(write(file, &offset, 1) != 1)
    {
        std::cout << "Error setting register to read\n";
        exit(1);
    }
    // Now read calibration data from 0xAA (22 bytes)
    offset = 0xAA;
    write(file, &offset, 1);
    if(read(file, calBuffer, 22) != 22)
    {
        std::cout << "Error reading calibration data\n";
        exit(1);
    }

    // Set AC1 (2s-complement)
    cal.AC1 = calBuffer[0];
    cal.AC1 = (cal.AC1 << 8) | calBuffer[1];
    cal.AC1 = ~cal.AC1 + 1;
    // AC2 (2s-complement)
    cal.AC2 = calBuffer[2];
    cal.AC2 = (cal.AC2 << 8) | calBuffer[3];
    cal.AC2 = ~cal.AC2 + 1;
    // AC3 (2s-complement)
    cal.AC3 = calBuffer[4];
    cal.AC3 = (cal.AC3 << 8) | calBuffer[5];
    cal.AC3 = ~cal.AC3 + 1;

    // AC4 (unsigned)
    cal.AC4 = calBuffer[6];
    cal.AC4 = (cal.AC4 << 8) | calBuffer[7];

    // AC5 (unsigned)
    cal.AC5 = calBuffer[8];
    cal.AC5 = (cal.AC5 << 8) | calBuffer[9];

    // AC6 (unsigned)
    cal.AC6 = calBuffer[10];
    cal.AC6 = (cal.AC6 << 8) | calBuffer[11];

    // B1
    cal.B1 = calBuffer[12];
    cal.B1 = (cal.B1 << 8) | calBuffer[13];
    cal.B1 = ~cal.B1 + 1;

    // B2
    cal.B2 = calBuffer[14];
    cal.B2 = (cal.B2 << 8) | calBuffer[15];
    cal.B2 = ~cal.B2 + 1;

    // MB
    cal.MB = calBuffer[16];
    cal.MB = (cal.MB << 8) | calBuffer[17];
    cal.MB = ~cal.MB + 1;

    // MC
    cal.MC = calBuffer[18];
    cal.MC = (cal.MC << 8) | calBuffer[19];
    cal.MC = ~cal.MC + 1;

    // MD
    cal.MD = calBuffer[20];
    cal.MD = (cal.MD << 8) | calBuffer[21];
    cal.MD = ~cal.MD + 1;

    // Now read the uncompensated temperature
    // Write 0x2E to register 0xF4
    offset = 0xF4;
    std::uint8_t val{0x2E};
    write(file, &offset, 1);
    write(file, &val, 1);
    // 4.5 ms
    usleep(4500);
    // read register 0xF6 (MSB) 0xF7 (LSB)
    offset = 0xF6;
    write(file, &offset, 1);
    char buf[2];
    if(read(file, buf, 2) < 2)
    {
        perror("error reading UT");
        exit(1);
    }
    short UT{0};
    UT = buf[0];
    UT = (UT << 8) | buf[1];
    UT = ~UT + 1;

    std::cout << "UT = " << UT << std::endl;
    std::cout << "AC1: " << cal.AC1 << std::endl;
    std::cout << "AC2: " << cal.AC2 << std::endl;
    std::cout << "AC3: " << cal.AC3 << std::endl;
    std::cout << "AC4: " << cal.AC4 << std::endl;
    std::cout << "AC5: " << cal.AC5 << std::endl;
    std::cout << "AC6: " << cal.AC6 << std::endl;
    std::cout << "B1: " << cal.B1 << std::endl;
    std::cout << "B2: " << cal.B2 << std::endl;
    std::cout << "MB: " << cal.MB << std::endl;
    std::cout << "MC: " << cal.MC << std::endl;
    std::cout << "MD: " << cal.MD << std::endl;

*/