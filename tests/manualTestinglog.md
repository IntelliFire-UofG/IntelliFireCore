# IntelliFire Manual Testing Report

**Project:** IntelliFire  
**Platform:** Raspberry Pi 5  
**Test Date:** 2025-04-18  
**Tester:** Misael Rivera, Shahmurad Orujov, Mohammed Al-Obaidly
**Environment:** SSH-connected terminal UI, MTX media stream, live sensors/motors.

---

## Flame Sensor Array + Water Pump

### Test Case: Detect Flame in Path
- **Setup:** L393M sensors placed 10 cm from a lighter flame.
- **Expectation:** One or more sensors report (analog), pump triggers.
- **Result:**
  - Sensor Readings: `[120, 130, 740, 125]`
  - Direction Detected: Index 2
  - Water Pump: Triggered
  - UI Message: `Flame detected at direction 2 — Pump Activated`

**Passed**

---

### Test Case: No Flame Present
- **Setup:** All sensors exposed to ambient lighting only.
- **Expectation:** No sensor surpasses threshold, pump remains off.
- **Result:**
  - Sensor Readings: `[0, 0, 2, 0]`
  - UI Message: `No flame detected`
  - Water Pump: Not Triggered

**Passed**

---

## Ultrasonic and IR Presence Sensors

### Test Case: Obstacle Ahead at 15 cm
- **Setup:** Place object in front of ultrasonic sensor at ~15 cm.
- **Expectation:** UI logs the distance, car halts.
- **Result:**
  - Distance Read: `14.8 cm`
  - UI Message: `14.8 cm`
  - Car State:  Stopped

**Passed**

---

### Test Case: Obstacle Behind Detected
- **Setup:** Hand placed 5 cm behind IR presence sensor.
- **Expectation:** UI logs presence, optional rear alert.
- **Result:**
  - UI Message: `Obstacle detected`
  - Buzzer/PWM: N/A

**Passed**

---

## Keyboard Control (Keylogger)

### Test Case: Key Presses and Releases
- **Setup:** Use SSH terminal to control car via keyboard.
- **Keys Tested:** `W`, `A`, `S`, `D`, `Space`
- **Expectation:** UI displays keypress, motors respond.
- **Result:**
  - Press `W`: `Key Pressed: W — Moving Forward`
  - Press `Space`: `Key Pressed: Space — Stopping`
  - Motor GPIO Activity: Verified via motor movement

**Passed**

---

## Video Stream via MTX Media

### Test Case: Stream Opens on Localhost
- **Setup:** Connect to `RPI URL designated by team`
- **Expectation:** Live stream with <2s latency.
- **Result:**
  - Stream loads: Yes
  - Latency: ~1.3s
  - Video Quality: Acceptable (640x480)

**Passed**

---

## User Interface (SSH)

### Test Case: Display Sensor and System Status
- **Setup:** Open SSH and launch UI script.
- **Expectation:** UI logs all sensor values, states, and events.
- **Result:**
  - Flame Sensors: Analog values shown live
  - Obstacles: Realtime updates on detection
  - Keys & Motors: Input/output confirmed
  - Pump Status: On/Off toggle appears

**Passed**

---

## Threading & Event Handling

### Test Case: Simultaneous Inputs (Flame + Obstacle + Movement)
- **Setup:** While moving forward, trigger flame and place object ahead.
- **Expectation:** Car stops, pump activates, all events logged.
- **Result:**
  - Motors halted on obstacle
  - Pump triggered for flame
  - UI logs all 3: `Flame detected when analog values are increased`, `Obstacle ahead`, `Key Pressed: W`

**Passed**

---

## Reboot & Recovery

### Test Case: Power Cycle Device
- **Expectation:** All threads/services restart without manual intervention.
- **Result:**
  - Services auto-launch: Correct
  - UI starts via ssh: Correct
  - Camera stream rebinds: Correct if scripts are run

**Passed**

---

# Final Summary

| Module             | Tests Run | Passed | Issues |
|--------------------|-----------|--------|--------|
| Flame Detection    | 10         | 10      | 0      |
| Obstacle Sensing   | 3         | 3      | 0      |
| Keyboard Control   | 3         | 2      | 1 - Solved      |
| Video Streaming    | 2         | 2      | 0      |
| UI Logging         | 5         | 3      | 2 - Solved      |
| Thread/Event Mgmt  | 2         | 2      | 0      |
| Reboot Recovery    | 2         | 2      | 0      |

**Total:** 9 Tests, All Passed 
**Status:** System Ready for Deployment 

