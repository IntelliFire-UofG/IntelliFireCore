#ifndef ADCREADER_H
#define ADCREADER_H

#include <functional>

/**
 * @class ADCReader
 * @brief Reads LM393 flame sensor values through ADS1015/ADS1115 ADC via I2C.
 *
 * This class communicates with an ADC to get sensor values and triggers events when flames are detected.
 *
 * @author Misael Rivera
 * @date 2025-02-01
 */
class ADCReader {
public:
    /**
     * @brief Constructs an ADCReader for the given I2C address.
     * 
     * @param i2c_address I2C address of ADS1015/ADS1115.
     */
    ADCReader(int i2c_address);
    
    /**
     * @brief Reads the values of all connected flame sensors.
     */
    void readSensors();

    /**
     * @brief Registers a callback for flame detection.
     * 
     * @param callback Function to execute when flames are detected.
     *        The function receives the sensor channel and detected value.
     */
    void registerFlameCallback(std::function<void(int, int)> callback);

private:
    int address;
    std::function<void(int, int)> flameCallback;

    /**
     * @brief Reads a specific channel from the ADC.
     * 
     * @param channel ADC channel number (0-3).
     * @return int Measured analog value (0-4095).
     */
    int readADC(int channel);
};

#endif // ADCREADER_H
