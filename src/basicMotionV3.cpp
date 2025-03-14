#include "../include/LN298MotorControlV3.h"
#include <atomic>
#include <thread>

// Function Declarations
void keyboardListener(std::atomic<char>& lastKey);
void keyboardControl(Motor &leftMotor, Motor &rightMotor, std::atomic<char>& lastKey);

int basicMotion() {
    Motor leftMotor(12, 17, 27);                      
    Motor rightMotor(13, 22, 23);

    std::atomic<char> lastKey;
    lastKey.store('\0');

    std::thread keyboardThread(keyboardListener, std::ref(lastKey));
    keyboardControl(leftMotor, rightMotor, std::ref(lastKey));

    keyboardThread.detach();
    return 0;
}

// Add the main() function
int main() {
    return basicMotion();
}
