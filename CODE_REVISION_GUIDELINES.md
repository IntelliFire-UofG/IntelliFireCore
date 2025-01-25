# C++ Code Revision Guidelines

> First Revision: January 24th 2025

> Author: Misael Rivera

> Status: Under Approval 

## Naming Conventions

### Functions
- Use **camelCase** for function names.
- Example:
  ```cpp
  void calculateSum();
  void processData(int input);
  ```

### Classes
- Use **PascalCase** for class names.
- Example:
  ```cpp
  class MyClass {
      // Class members
  };
  ```

### Variables
- Use **snake_case** for non-constant variables.
- Example:
  ```cpp
  int user_age;
  float sensor_reading;
  ```

### Constants
- Use **UPPERCASE_SNAKE_CASE** for constants.
- Example:
  ```cpp
  const int MAX_LIMIT = 100;
  const double PI = 3.14159;
  ```

---

## General Coding Practices

### 1. Object-Oriented Programming (OOP) Rules
- **Encapsulation**: Keep data members private or protected and use public getter and setter methods.
- **Inheritance**: Use virtual functions for polymorphism and always provide a virtual destructor for base classes.
- **Abstraction**: Use abstract base classes or interfaces when needed.
- **Modularity**: Separate class declarations in header files (`.h`) and implementations in source files (`.cpp`).

### 2. Embedded and Real-Time Programming Practices
- **Avoid Dynamic Memory Allocation**: Prefer stack allocation or static allocation over `new` or `malloc` for predictable memory use.
- **Low Latency**: Minimize function execution time. Avoid blocking functions like `delay()` where possible.
- **Thread Safety**: Use mutexes or atomic variables when dealing with shared resources.
- **Interrupts**: Keep interrupt service routines (ISRs) short and avoid function calls within ISRs.
- **Error Handling**: Always check the return values of critical functions and handle errors appropriately.

### 3. Documentation Guidelines
Every function or class must have a docstring. Include:
- **Parameters**: List all input parameters with types and purpose.
- **Outcome**: Describe the function's return value or side effects.
- **Creation Date**: When the function was initially created.
- **Author**: Who wrote the function.
- **Modification Date**: When the function was last modified.
- **Modified By**: Name of the person who modified it.

Example:
```cpp
/**
 * @brief Calculates the sum of two integers.
 *
 * @param a First integer to add.
 * @param b Second integer to add.
 * @return The sum of a and b.
 *
 * @date 2025-01-24
 * @author John Doe
 * @modification 2025-01-25 Jane Smith - Optimized calculation.
 */
int calculateSum(int a, int b) {
    return a + b;
}
```

---

## Examples

### Class Example
```cpp
/**
 * @brief Represents a simple Robot with basic functionalities.
 *
 * @date 2025-01-24
 * @author John Doe
 */
class Robot {
private:
    std::string robot_name;
    int battery_level;

public:
    /**
     * @brief Constructor to initialize the robot.
     *
     * @param name The name of the robot.
     * @param battery The initial battery level.
     */
    Robot(const std::string& name, int battery) : robot_name(name), battery_level(battery) {}

    /**
     * @brief Retrieves the robot's name.
     *
     * @return The name of the robot.
     */
    std::string getName() const {
        return robot_name;
    }

    /**
     * @brief Sets the robot's name.
     *
     * @param name The new name of the robot.
     */
    void setName(const std::string& name) {
        robot_name = name;
    }

    /**
     * @brief Displays the robot's current status.
     */
    void displayStatus() const {
        std::cout << "Robot Name: " << robot_name << "\n";
        std::cout << "Battery Level: " << battery_level << "%\n";
    }
};
```

### Embedded System Example
```cpp
/**
 * @brief Reads a temperature value from a sensor.
 *
 * @return The current temperature reading in Celsius.
 *
 * @date 2025-01-24
 * @author John Doe
 */
double readTemperature() {
    // Simulate sensor reading
    return 25.3;
}

/**
 * @brief Main function for the embedded system.
 */
int main() {
    const double temperature = readTemperature();
    std::cout << "Current Temperature: " << temperature << " °C\n";
    return 0;
}
