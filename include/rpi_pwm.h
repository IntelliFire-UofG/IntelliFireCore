#ifndef __RPIPWM
#define __RPIPWM

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<string>
#include <iostream>
#include<math.h>

/**
 * PWM class for the Raspberry PI 5
 **/
class RPI_PWM {
public:
    /**
     * Starts the PWM
     * \param channel The GPIO channel which is 0 or 1 for the RPI5 (GPIO12, GPIO13)
     * \param frequency The PWM frequency
     * \param duty_cycle The initial duty cycle of the PWM (default 0)
     * \param chip The chip number (for RPI5 it's 0)
     * \return >0 on success and -1 if an error has happened.
     **/
    int start(int channel, int frequency, float duty_cycle = 0, int chip = 0);

    /**
     * Stops the PWM
     **/
    void stop();
    
    ~RPI_PWM();

    /**
     * Sets the duty cycle.
     * \param v The duty cycle in percent.
     * \return >0 on success and -1 after an error.
     **/
    int setDutyCycle(float v) const;

private:
    void setPeriod(int ns) const;
    int setDutyCycleNS(int ns) const;
    void enable() const;
    void disable() const;

    int per = 0;
    std::string chippath;
    std::string pwmpath;

    int writeSYS(std::string filename, int value) const;
};

#endif
