#include "../include/LN298MotorControlV3.h"
#include "../include/basicMotionV2.h"

// Function Declarations
void keyboardListener(std::atomic<char>& lastKey);
void keyboardControl(Motor &leftMotor, Motor &rightMotor, std::atomic<char>& lastKey);

// Global motor instances for access by other system components
std::shared_ptr<Motor> leftMotorInstance = nullptr;
std::shared_ptr<Motor> rightMotorInstance = nullptr;

// This function can be called from flame detection
void stopMotors() {
    if (leftMotorInstance) leftMotorInstance->stop();
    if (rightMotorInstance) rightMotorInstance->stop();
}

int basicMotion() {
    Motor leftMotor(12, 17, 27, 100);                      
    Motor rightMotor(13, 23, 22, 100);

    leftMotorInstance = std::make_shared<Motor>(leftMotor);
    rightMotorInstance = std::make_shared<Motor>(rightMotor);

    std::atomic<char> lastKey;
    lastKey.store('\0');

    std::thread keyboardThread(keyboardListener, std::ref(lastKey));
    keyboardControl(leftMotor, rightMotor, std::ref(lastKey));

    keyboardThread.detach();
    return 0;
}
