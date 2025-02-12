/*
 * ADS1015 test/demo program for the Raspberry PI
 *
 * Copyright (c) 2007  MontaVista Software, Inc.
 * Copyright (c) 2007  Anton Vorontsov <avorontsov@ru.mvista.com>
 * Copyright (c) 2013-2024  Bernd Porr <mail@berndporr.me.uk>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License.
 *
 * Cross-compile with cross-gcc -I/path/to/cross-kernel/include
 */

 #include <stdint.h>
 #include <unistd.h>
 #include <stdio.h>
 #include <stdlib.h>
 
 #include "ads1015rpi.h"
 
// We inherit ADS1015rpi, implement
// hasSample() and print the ADC reading.
class FlameSensorReader : public ADS1015rpi
{
private:
    bool discard = false;
    ADS1015settings::Input current_channel = ADS1015settings::AIN0;
    void printChannel()
    {
        switch (current_channel)
        {
        case ADS1015settings::AIN0:
            printf("A0: ");
            break;
        case ADS1015settings::AIN1:
            printf("A1: ");
            break;
        case ADS1015settings::AIN2:
            printf("A2: ");
            break;
        case ADS1015settings::AIN3:
            printf("A3: ");
            break;
        }
    }
    void nextChannel()
    {
        switch (current_channel)
        {
        case ADS1015settings::AIN0:
            current_channel = ADS1015settings::AIN1;
            break;
        case ADS1015settings::AIN1:
            current_channel = ADS1015settings::AIN2;
            break;
        case ADS1015settings::AIN2:
            current_channel = ADS1015settings::AIN3;
            break;
        case ADS1015settings::AIN3:
            current_channel = ADS1015settings::AIN0;
            break;
        }
        setChannel(current_channel);
    }
 };
class MultichannelPrinter : public FlameSensorReader::ADSCallbackInterface {
     ADSCallbackInterface {
        virtual void hasADS1015Sample(float v) override {
            if (discard == true)
            {
                return;
                discard = false;
            }
            printChannel();
            printf("%e\n",v);
            nextChannel();
            discard = true;
        }
    }
};
 
 // Creates an instance of the ADS1015Printer class.
 // Prints data till the user presses a key.
 int main(int argc, char *argv[]) {
    fprintf(stderr,"Press any key to stop.\n");
    MultichannelPrinter ads1015Callback;
    ADS1015rpi ads1015rpi;
    ads1015rpi.registerCallback(&ads1015Callback);
    ADS1015settings s;
    s.samplingRate = ADS1015settings::FS128HZ;
    s.drdy_chip = 4; // for RPI1-4 chip = 0. For RPI5 it's chip = 4.
    ads1015rpi.start(s);
    fprintf(stderr,"fs = %d\n",ads1015rpi.getADS1015settings().getSamplingRate());
    getchar();
    ads1015rpi.stop();
    return 0;
 }
 
