#include "LN298MotorControlV2.h"
#include "basicMotionV2.h"
#include <atomic>
#include <thread>

int basicMotion() {
    Motor leftMotor(12, 17, 27);
    Motor rightMotor(13, 22, 23);

    std::atmoic<char> lastKey;
    lastKey.store('\0');

    std::thread keyboardThread(keyboardListener, std::ref(lastKey));
    keyboardControl(leftMotor, rightMotor, std::ref(lastKey));

    keyboardThread.detach();
    return 0
}

// Keep this if you want to be able to compile this file independently
#ifdef COMPILE_STANDALONE
int main() {
    return basicMotion();
}
#endif
