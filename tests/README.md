# IntelliFire - Test Scenarios

This document outlines the test scenarios for the IntelliFire flame-detection car system. The tests cover functionality, integration, performance, and fail-safe behavior for each major subsystem.

The testing stage was done on the last sprint of our ART (Agile Release Train)

---

## 1. Flame Sensor Array (LM393 via ADS1015)

### Test 1.1: Sensor Read Validity
- **Objective**: Ensure each flame sensor reports analog values in expected range (0–1023).
- **Steps**: 
  - Run the system without flame.
  - Introduce a lighter or flame source at various angles.
- **Expected Result**: 
  - Analog values increase for the sensor closest to the flame.
  - UI displays all 4 sensor values correctly.

### Test 1.2: Flame Direction Estimation
- **Objective**: Detect flame direction based on sensor with highest value.
- **Steps**: 
  - Place flame to the left, center, right.
- **Expected Result**: 
  - The correct directional inference is logged.
  - Water pump is directed accordingly.

### Test 1.3: ADS1015 Failure Handling
- **Objective**: Validate error handling on I2C communication failure.
- **Steps**: 
  - Disconnect ADS1015 briefly while running.
- **Expected Result**: 
  - UI logs I2C failure.
  - No crash occurs, and system attempts reconnection.

---

## 2. Water Pump Trigger

### Test 2.1: Automatic Activation
- **Objective**: Trigger the pump only when flame is detected.
- **Steps**:
  - Approach a flame gradually.
- **Expected Result**:
  - Pump is activated when threshold is passed.
  - Pump deactivates when flame is gone.

### Test 2.2: No False Positives
- **Objective**: Ensure pump doesn’t activate due to ambient IR or noise.
- **Steps**:
  - Shine an IR remote or flashlight in front.
- **Expected Result**:
  - Pump remains inactive.
  - UI does not indicate flame detection.

---

## 3. Ultrasonic Sensor (Obstacle Ahead)

### Test 3.1: Obstacle Detection Accuracy
- **Objective**: Measure and log distance accurately.
- **Steps**:
  - Place a wall/object at known distances (10cm, 20cm, 50cm).
- **Expected Result**:
  - UI reflects correct measurements within ±5cm.

### Test 3.2: No Echo Handling
- **Objective**: Test behavior when no echo is received.
- **Steps**:
  - Point sensor at open space.
- **Expected Result**:
  - Timeout occurs, UI shows "No object".
  - No crash or hang.

---

## 4. IR Presence Sensor (Obstacle Behind)

### Test 4.1: Rear Detection Trigger
- **Objective**: Detect presence of object from the rear.
- **Steps**:
  - Move hand slowly behind the car.
- **Expected Result**:
  - UI indicates obstacle presence.
  - Appropriate alert or movement restriction occurs if active.

---

## 5. Motor & Motion Control

### Test 5.1: Keyboard Movement
- **Objective**: Control the car with key presses.
- **Steps**:
  - Press keys: W (forward), S (backward), A (left), D (right).
- **Expected Result**:
  - Motors respond accordingly.
  - UI logs key press and corresponding direction.

### Test 5.2: Motor Fail-safe
- **Objective**: Stop all motors on error or key release.
- **Steps**:
  - Simulate input drop or release key.
- **Expected Result**:
  - Motors stop immediately.
  - UI shows “idle” or “stopped”.

---

## 6. UI System (SSH Terminal)

### Test 6.1: Real-time Updates
- **Objective**: Ensure all sensor data, key events, and actions are reflected.
- **Steps**:
  - Activate each sensor and press keys.
- **Expected Result**:
  - UI updates all fields in <1s.
  - No lag or freezing.

### Test 6.2: Long-run Stability
- **Objective**: Validate system over extended runtime.
- **Steps**:
  - Run IntelliFire for 1 hour.
- **Expected Result**:
  - No memory leaks or crashes.
  - UI remains responsive.

---

## 7. Camera Streaming (MTX Media)

### Test 7.1: Stream Availability
- **Objective**: Verify live stream availability on local network.
- **Steps**:
  - Start MTX server and camera.
  - Open stream URL on browser.
- **Expected Result**:
  - Live video displays with <1s delay.

### Test 7.2: Performance Under Load
- **Objective**: Validate system with both camera and UI active.
- **Steps**:
  - Move the car while streaming and viewing UI.
- **Expected Result**:
  - No significant delay in UI or video.
  - Motors and sensors remain unaffected.

---

## 8. Integration & Edge Cases

### Test 8.1: Flame + Obstacle + Movement
- **Objective**: Handle flame detection and obstacle presence during motion.
- **Steps**:
  - Approach a flame while an object is behind.
- **Expected Result**:
  - Car moves, detects flame, activates pump.
  - IR sensor prevents reversing.

### Test 8.2: Full System Recovery
- **Objective**: Simulate reboot and check restoration.
- **Steps**:
  - Power cycle the device.
- **Expected Result**:
  - All threads restart.
  - UI, motors, and sensors function normally.

---

## 9. Logging & Debug

### Test 9.1: Error Message Clarity
- **Objective**: Ensure errors are logged clearly.
- **Steps**:
  - Simulate sensor failure or GPIO conflict.
- **Expected Result**:
  - Log includes sensor name, error type, and suggested fix.

---

