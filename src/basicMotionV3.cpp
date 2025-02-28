#include "LN298MotorControlV2.h"
#include <atomic>
#include <thread>

int main() {
    Motor leftMotor(12, 17, 27);
    Motor rightMotor(13, 22, 23);

    std::atomic<char> lastKey;  // Correct declaration (remove direct assignment)
    lastKey.store('\0');        // Store initial value separately

    // Start keyboard listener in a separate thread
    std::thread keyboardThread(keyboardListener, std::ref(lastKey));

    // Start motor control loop with correct parameters
    keyboardControl(leftMotor, rightMotor, std::ref(lastKey));

    // Cleanup
    keyboardThread.detach();  // Let keyboard listener run indefinitely
    return 0;
}
