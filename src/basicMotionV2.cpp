#include "../include/LN298MotorControlV2.h"

int main() {
    Motor leftMotor(12, 17, 27);  // Left Motor: PWM=12, IN1=17, IN2=27
    Motor rightMotor(13, 22, 23); // Right Motor: PWM=13, IN3=22, IN4=23

    leftMotor.moveForward();
    rightMotor.moveForward();
    std::this_thread::sleep_for(std::chrono::seconds(3)); // Move forward for 3 seconds

    turnLeft(leftMotor, rightMotor);  // Turn left for 1 second

    leftMotor.moveBackward();
    rightMotor.moveBackward();
    std::this_thread::sleep_for(std::chrono::seconds(3)); // Move backward for 3 seconds

    turnRight(leftMotor, rightMotor);  // Turn right for 1 second

    leftMotor.stop();
    rightMotor.stop();

    return 0;
}
