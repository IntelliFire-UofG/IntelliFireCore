#ifndef __ADS1115RPI_H
#define __ADS1115RPI_H

#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <linux/i2c-dev.h>
#include <thread>
#include <vector>
#include <atomic>
#include <mutex>
#include <gpiod.h>

#ifndef NDEBUG
#define DEBUG
#endif

#define ISR_TIMEOUT 1000
#define DEFAULT_ADS1115_ADDRESS 0x48
#define DEFAULT_ALERT_RDY_TO_GPIO 5

struct ADS1115settings {
    int i2c_bus = 1;
    uint8_t address = DEFAULT_ADS1115_ADDRESS;

    enum SamplingRates {
        FS8HZ = 0, FS16HZ, FS32HZ, FS64HZ, FS128HZ, FS250HZ, FS475HZ, FS860HZ
    };

    inline unsigned getSamplingRate() const {
        static const unsigned SamplingRateEnum2Value[8] = {8, 16, 32, 64, 128, 250, 475, 860};
        return SamplingRateEnum2Value[samplingRate];
    }

    SamplingRates samplingRate = FS8HZ;

    enum PGA {
        FSR4_096 = 1,
        FSR2_048 = 2,
        FSR1_024 = 3,
        FSR0_512 = 4,
        FSR0_256 = 5
    };

    PGA pgaGain = FSR2_048;

    enum Input {
        AIN0 = 0,
        AIN1,
        AIN2,
        AIN3
    };

    Input channel = AIN0;

    int drdy_chip = 0;
    int drdy_gpio = DEFAULT_ALERT_RDY_TO_GPIO;
};

class ADS1115rpi {
public:
    ~ADS1115rpi();

    struct ADSCallbackInterface {
        virtual void hasADS1115Sample(float sample) = 0;
        virtual ~ADSCallbackInterface() = default;
    };

    void registerCallback(ADSCallbackInterface* ci);
    void start(ADS1115settings settings = ADS1115settings());
    void stop();

    void setChannel(ADS1115settings::Input channel);
    ADS1115settings getADS1115settings() const;

private:
    void worker();
    void dataReady();
    void i2c_writeWord(uint8_t reg, unsigned data);
    unsigned i2c_readWord(uint8_t reg);
    int i2c_readConversion();
    float fullScaleVoltage();

    ADS1115settings ads1115settings;

    std::thread thr;
    std::atomic<bool> running{false};

    gpiod_chip* chipDRDY = nullptr;
    gpiod_line* pinDRDY = nullptr;
    int fd_i2c = -1;

    std::vector<ADSCallbackInterface*> adsCallbackInterfaces;
    std::mutex callback_mutex;

    const uint8_t reg_config = 1;
    const uint8_t reg_lo_thres = 2;
    const uint8_t reg_hi_thres = 3;
};

#endif
