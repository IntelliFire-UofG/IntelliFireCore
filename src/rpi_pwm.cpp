#include "rpi_pwm.h"
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <math.h>

int RPI_PWM::start(int channel, int frequency, float duty_cycle, int chip) {
    chippath = "/sys/class/pwm/pwmchip" + std::to_string(chip);
    pwmpath = chippath + "/pwm" + std::to_string(channel);
    std::string p = chippath + "/export";
    FILE* const fp = fopen(p.c_str(), "w");
    if (NULL == fp) {
        std::cerr << "PWM device does not exist. Make sure to add 'dtoverlay=pwm-2chan' to /boot/firmware/config.txt.\n";
        return -1;
    }
    const int r = fprintf(fp, "%d", channel);
    fclose(fp);
    if (r < 0) return r;
    usleep(100000); // Wait till the PWM subdir is created

    per = (int)1E9 / frequency;
    setPeriod(per);
    setDutyCycle(duty_cycle);
    enable();
    return r;
}

void RPI_PWM::stop() {
    disable();
}

RPI_PWM::~RPI_PWM() {
    disable();
}

int RPI_PWM::setDutyCycle(float v) const {
    const int dc = (int)round((float)per * (v / 100.0));
    return setDutyCycleNS(dc);
}

void RPI_PWM::setPeriod(int ns) const {
    writeSYS(pwmpath + "/period", ns);
}

int RPI_PWM::setDutyCycleNS(int ns) const {
    return writeSYS(pwmpath + "/duty_cycle", ns);
}

void RPI_PWM::enable() const {
    writeSYS(pwmpath + "/enable", 1);
}

void RPI_PWM::disable() const {
    writeSYS(pwmpath + "/enable", 0);
}

int RPI_PWM::writeSYS(std::string filename, int value) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open " << filename << std::endl;
        return -1;
    }
    file << value;
    file.close();
    return 0;
}