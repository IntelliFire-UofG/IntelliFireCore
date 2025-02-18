 #include <stdint.h>
 #include <unistd.h>
 #include <stdio.h>
 #include <stdlib.h>
 
 #include "ads1115rpi.h"
 
class FlameSensorReader: public ADS1115rpi::ADSCallbackInterface
{
private:
    ADS1115rpi ads1115rpi;
    bool discard = false;
    ADS1115settings::Input current_channel = ADS1115settings::AIN0;
    void printChannel()
    {
        switch (current_channel)
        {
        case ADS1115settings::AIN0:
            printf("A0: ");
            break;
        case ADS1115settings::AIN1:
            printf("A1: ");
            break;
        case ADS1115settings::AIN2:
            printf("A2: ");
            break;
        case ADS1115settings::AIN3:
            printf("A3: ");
            break;
        }
    }
    void nextChannel()
    {
        switch (current_channel)
        {
        case ADS1115settings::AIN0:
            current_channel = ADS1115settings::AIN1;
            break;
        case ADS1115settings::AIN1:
            current_channel = ADS1115settings::AIN2;
            break;
        case ADS1115settings::AIN2:
            current_channel = ADS1115settings::AIN3;
            break;
        case ADS1115settings::AIN3:
            current_channel = ADS1115settings::AIN0;
            break;
        }
        ads1115rpi.setChannel(current_channel);
    }
    

public:
    void start()
    {
        ads1115rpi.registerCallback(this);
        ADS1115settings s;
        s.samplingRate = ADS1115settings::FS128HZ;
        s.drdy_chip = 4; // for RPI1-4 chip = 0. For RPI5 it's chip = 4.
        ads1115rpi.start();
        fprintf(stderr,"fs = %d\n",ads1115rpi.getADS1115settings().getSamplingRate());
    }

    void stop()
    {
        ads1115rpi.registerCallback(this);
        ADS1115settings s;
        s.samplingRate = ADS1115settings::FS128HZ;
        s.drdy_chip = 4; // for RPI1-4 chip = 0. For RPI5 it's chip = 4.
        ads1115rpi.stop();
    }

    void hasADS1115Sample(float v) override 
    {
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
};

class MultichannelPrinter : public FlameSensorReader::ADSCallbackInterface {
     
};
 
 // Creates an instance of the ADS1115Printer class.
 // Prints data till the user presses a key.
 int main(int argc, char *argv[]) {
    fprintf(stderr,"Press any key to stop.\n");
    FlameSensorReader flame_sensors;
    flame_sensors.start();
    getchar();
    flame_sensors.stop();
    return 0;
 }
 
