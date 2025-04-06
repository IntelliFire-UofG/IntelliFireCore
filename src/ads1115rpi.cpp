#include "ads1115rpi.h"
#include <fcntl.h>
#include <sys/ioctl.h>
#include <cstring>

ADS1115rpi::~ADS1115rpi() {
    stop();  // Ensure safe cleanup on destruction
}

void ADS1115rpi::registerCallback(ADSCallbackInterface* ci) {
    std::lock_guard<std::mutex> lock(callback_mutex);
    adsCallbackInterfaces.push_back(ci);
}

void ADS1115rpi::start(ADS1115settings settings) {
    ads1115settings = settings;

    // Open I2C device
    char device[32];
    snprintf(device, sizeof(device), "/dev/i2c-%d", settings.i2c_bus);
    fd_i2c = open(device, O_RDWR);
    if (fd_i2c < 0) {
        throw "ADS1115rpi: Could not open I2C device.";
    }

    if (ioctl(fd_i2c, I2C_SLAVE, settings.address) < 0) {
        close(fd_i2c);
        fd_i2c = -1;
        throw "ADS1115rpi: Failed to set I2C address.";
    }

    // Enable ALERT/RDY
    i2c_writeWord(reg_lo_thres, 0x0000);
    i2c_writeWord(reg_hi_thres, 0x8000);

    unsigned config = (1 << 15); // OS=1 (start single conversion)
    config |= (settings.channel << 12);
    config |= (settings.pgaGain << 9);
    config |= (1 << 8); // MODE=1 (single-shot mode)
    config |= (settings.samplingRate << 5);
    config |= (3 << 0); // COMP_MODE=traditional, COMP_POL=active low, COMP_LAT=non-latching

    i2c_writeWord(reg_config, config);

    // Open GPIO chip
    chipDRDY = gpiod_chip_open_by_number(settings.drdy_chip);
    if (!chipDRDY) {
        throw "ADS1115rpi: Failed to open GPIO chip.";
    }

    pinDRDY = gpiod_chip_get_line(chipDRDY, settings.drdy_gpio);
    if (!pinDRDY) {
        gpiod_chip_close(chipDRDY);
        chipDRDY = nullptr;
        throw "ADS1115rpi: Failed to access ALERT/RDY line.";
    }

    if (gpiod_line_request_rising_edge_events(pinDRDY, "ADS1115rpi") < 0) {
        gpiod_chip_close(chipDRDY);
        chipDRDY = nullptr;
        pinDRDY = nullptr;
        throw "ADS1115rpi: Could not request rising edge event.";
    }

    running = true;
    thr = std::thread(&ADS1115rpi::worker, this);
}

void ADS1115rpi::stop() {
    if (!running.exchange(false)) return;

    if (thr.joinable()) {
        thr.join();
    }

    if (pinDRDY) {
        gpiod_line_release(pinDRDY);
        pinDRDY = nullptr;
    }

    if (chipDRDY) {
        gpiod_chip_close(chipDRDY);
        chipDRDY = nullptr;
    }

    if (fd_i2c >= 0) {
        close(fd_i2c);
        fd_i2c = -1;
    }
}

void ADS1115rpi::setChannel(ADS1115settings::Input channel) {
    unsigned reg = i2c_readWord(reg_config);
    reg &= ~(0x03 << 12);  // Clear old channel
    reg |= (channel << 12);
    i2c_writeWord(reg_config, reg);
    ads1115settings.channel = channel;
}

ADS1115settings ADS1115rpi::getADS1115settings() const {
    return ads1115settings;
}

float ADS1115rpi::fullScaleVoltage() {
    switch (ads1115settings.pgaGain) {
        case ADS1115settings::FSR4_096: return 4.096f;
        case ADS1115settings::FSR2_048: return 2.048f;
        case ADS1115settings::FSR1_024: return 1.024f;
        case ADS1115settings::FSR0_512: return 0.512f;
        case ADS1115settings::FSR0_256: return 0.256f;
        default: return 2.048f;
    }
}

void ADS1115rpi::dataReady() {
    int raw = i2c_readConversion();
    float voltage = (float)raw / 0x7FFF * fullScaleVoltage();

    std::lock_guard<std::mutex> lock(callback_mutex);
    for (auto& cb : adsCallbackInterfaces) {
        if (cb) cb->hasADS1115Sample(voltage);
    }
}

void ADS1115rpi::worker() {
    while (running) {
        const struct timespec timeout = {1, 0};
        int r = gpiod_line_event_wait(pinDRDY, &timeout);
        if (r == 1) {
            gpiod_line_event event;
            if (gpiod_line_event_read(pinDRDY, &event) == 0) {
                dataReady();
            }
        }
    }
}

// I2C communication helpers
void ADS1115rpi::i2c_writeWord(uint8_t reg, unsigned data) {
    uint8_t buf[3];
    buf[0] = reg;
    buf[1] = (data >> 8) & 0xFF;
    buf[2] = data & 0xFF;

    if (write(fd_i2c, buf, 3) != 3) {
        throw "ADS1115rpi: Failed to write I2C word.";
    }
}

unsigned ADS1115rpi::i2c_readWord(uint8_t reg) {
    uint8_t cmd = reg;
    if (write(fd_i2c, &cmd, 1) != 1) {
        throw "ADS1115rpi: Failed to write I2C register address.";
    }

    uint8_t buf[2];
    if (read(fd_i2c, buf, 2) != 2) {
        throw "ADS1115rpi: Failed to read I2C word.";
    }

    return (buf[0] << 8) | buf[1];
}

int ADS1115rpi::i2c_readConversion() {
    uint8_t cmd = 0x00; // Conversion register
    if (write(fd_i2c, &cmd, 1) != 1) {
        throw "ADS1115rpi: Failed to write conversion register.";
    }

    uint8_t buf[2];
    if (read(fd_i2c, buf, 2) != 2) {
        throw "ADS1115rpi: Failed to read ADC conversion.";
    }

    return (int16_t)((buf[0] << 8) | buf[1]);
}
