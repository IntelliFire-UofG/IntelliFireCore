#ifndef BASIC_MOTION_V2_H
#define BASIC_MOTION_V2_H
/**
 * @brief Initializes and runs the keyboard-controlled motion system for the fire truck
 * 
 * This function sets up the left and right motors, initializes the keyboard listener,
 * and runs the motor control loop. It handles W/A/S/D key commands to control the truck's
 * movement and returns when the user presses Q to quit.
 * 
 * @return int Status code (0 for success)
 */
int basicMotion();

/**
 * @brief Stops both motors when called from external components
 * 
 * This function can be called by other parts of the system (like flame detection)
 * to immediately stop all motors in an emergency situation.
 */
void stopMotors();

#endif