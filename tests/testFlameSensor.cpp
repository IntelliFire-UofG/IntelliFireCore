 #include <stdio.h>
 #include <gpiod.h>
 #include <time.h>
 #include <iostream>

 #define ISR_TIMEOUT 1000
 void flameDetection(struct gpiod_line_event event){
    if (event.event_type == GPIOD_LINE_EVENT_RISING_EDGE){
        std::cout<<"the area is safe";

    }
    else if (event.event_type == GPIOD_LINE_EVENT_FALLING_EDGE){
        std::cout<<"Flame detected";

    }

 }

 int main(){
 int chipNo = 0;
 //make sure its the right pin on the rp5
 int pinNo = 1;

 gpiod_chip *chipGPIO = gpiod_chip_open_by_number(chipNo);
 gpiod_line *flameGPIO = gpiod_chip_get_line(chipGPIO,pinNo);
 // might not have to set it as input as "gpiod_line_request_both_edges_events" does this.
 /*
 //configure flame pin as input
if(gpiod_line_request_input(flameGPIO, "flamepin")<0){
    perror("Failed to configure input pin");
    gpiod_chip_close(chipGPIO);
    return -1;
}
*/
if(gpiod_line_request_both_edges_events(flameGPIO,"Consumer")<0){
    perror("Failed to configure input pin");
    gpiod_chip_close(chipGPIO);
    return -1;


while (true){
    const timespec ts = {ISR_TIMEOUT, 0};
    int r = gpiod_line_event_wait(flameGPIO,&ts);
    if (1 ==r) {
        gpiod_line_event event;
        gpiod_line_event_read(flameGPIO,&event);
        flameDetection(event);

    }

}


 }

