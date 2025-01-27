# Real-Time Flame Sensor Monitoring with libgpiod

## Description
This project provides a real-time flame sensor monitoring application built with C++ and the `libgpiod` library. The application uses the Linux GPIO character device interface to interact with a flame sensor (e.g., LM393) connected to a Raspberry Pi.

It continuously reads the flame sensor's state, displays real-time status on the console, and ensures minimal latency for real-time processing.

---

## Key Features
- **Real-Time Monitoring:** Continuously checks flame sensor status with low latency.
- **Event-Driven Architecture:** Designed to handle GPIO events efficiently using the `libgpiod` library.
- **Portable:** Runs on any Linux system with GPIO support and `libgpiod`.
- **Error Handling:** Graceful handling of hardware and system errors.

---

## Hardware Requirements
- Raspberry Pi or a similar Linux-based system with GPIO support.
- LM393 flame sensor or equivalent.
- Wiring:
  - VCC -> 3.3V or 5V.
  - GND -> Ground.
  - DOUT -> GPIO Pin (default: GPIO17).

---

## Software Requirements
- Linux (e.g., Raspberry Pi OS).
- **libgpiod** library installed.

Install `libgpiod` with:
```bash
sudo apt update
sudo apt install gpiod libgpiod-dev
```
## Run

### Compile the Code: Use g++ to compile:

```bash
g++ -o LM393_SupportCode_Read LM393_SupportCode_Read.cpp -lgpiod
```

### Run the Application: Run the executable as root to access GPIO:

``` bash
sudo ./LM393_SupportCode_Read.cpp
```