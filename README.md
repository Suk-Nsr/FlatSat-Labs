# 🛰️ NBSPACE FlatSat Learning Kit

![NBSPACE Labs](https://img.shields.io/badge/NBSPACE-Labs-blue.svg)
![Platform](https://img.shields.io/badge/Platform-STM32F429ZI-lightgrey.svg)
![Framework](https://img.shields.io/badge/Framework-Arduino_IDE-00979D.svg)

Welcome to the **NBSPACE FlatSat Learning Kit** repository! This educational platform is designed to provide a hands-on, deeply practical understanding of Satellite Systems Engineering. By deploying the core subsystems of a CubeSat onto an accessible flat plane, students can program, interface, and test the real-world operational challenges of a modern small satellite.

## 📖 About The Project

Unlike traditional hardware labs that focus primarily on passive data logging, this curriculum adopts a **Systems Engineering and Resiliency approach**. 

The course material utilizes a "punched-hole" structure (Skeleton Code with structured `TODO` blocks) combined with a background testbench environment. Students are challenged to bridge the gap between low-level hardware communication and dynamic satellite state handling, mimicking the firmware requirements of actual space missions.

## 🧩 System Architecture

The FlatSat platform features fully modular segments integrated through a standardized **PC104 Bus Interface**:

* **💻 On-Board Computer (OBC):** Powered by an STM32F429ZI MCU, directing mission operations, orchestrating SPI Flash/SD Card resources, and executing central processing loops.
* **⚡ Electrical Power System (EPS):** Centrally managed via an external I2C bus (SDA: PF0, SCL: PF1). Monitors active voltage/current lines through INA226 and routes power channels using ADM1177 controllers.
* **📡 Communication Subsystem (COMMS):** Utilizes an RFM98PW radio transceiver operating on amateur 433 MHz frequencies to transmit satellite frames using the KISS protocol.
* **📷 Imaging Payload:** A 5-Megapixel SPI/I2C digital camera configured for Earth Observation payload processing.
* **🌍 Ground Segment:** An STM32F103RC transceiver unit serving as the operator's primary interface to dispatch commands and capture downlinked frames.

## 🚀 Lab Curriculum & Learning Path

The curriculum guides students step-by-step from fundamental I/O operations through system resiliency, culminating in a fully autonomous orbit simulation:

* **Lab 1: Satellite System**
   * *Core Tasks:* Establish Serial Monitor communication, execute an I2C bus scanner, and integrate RTC time with temperature readings. Apply the KISS protocol for packet encapsulation and handle byte stuffing.
   * *Application:* Build a space-to-ground link using the RF transceiver. Transmit data frames from the satellite to the Ground Station, decode them, and verify data integrity using Checksums to handle simulated space environment anomalies.
* **Lab 2: Memory & Power Subsystems of Satellite**
   * *Core Tasks:* Interface with SPI Flash to track boot counts and build a telemetry vault on the SD Card using append mode. Fetch power telemetry from INA226 nodes and handle low-level subsystem toggles via GPIO.
   * *Application:* Implement an emergency backup mission transferring critical data from SPI Flash to SD Card to prepare for a simulated power brownout. Write an autonomous Safe Mode routine utilizing hysteresis to trigger structural load-shedding at 30% battery and restore operations at 70%.
* **Lab 3: Earth Observation & Mission Logic**
   * *Core Tasks:* Decode NMEA sentences from GPS modules via auxiliary serial lines and synchronize external camera capture routines. Implement GPS-Gating to evaluate orbital constraints.
   * *Application:* Build a gating mechanism that powers the payload only inside a Region of Interest. Fire an image capture command, pull active time-stamps (RTC) and coordinates (GPS), and embed them directly as geo-location payload metadata into an SD storage entry.
* **Lab 4: Resilient Downlink & System Fault Tolerance**
   * *Core Tasks:* Segment binary picture records on the SD card into fixed-size transport chunks. Implement a robust Store-and-Forward downlink using Handshake and ACK mechanisms.
   * *Application:* Configure the Independent Watchdog Timer (IWDG) to protect against radiation-induced lockups (SEU). Implement fault recovery states utilizing non-volatile memory to resume packet transmission seamlessly from the exact fail-point after a forced reboot.
* **Lab 5: Full satellite system and ground station project**

## 🛠️ Getting Started

### Prerequisites

To compile and execute the flight software code in this workspace, ensure you have configured:
1.  **Arduino IDE 2.x** installed.
2.  **STM32 MCU Based Boards** core addition provided through the official Board Manager.
3.  Required External Dependencies:
    * `RadioLib` (Tested on version 7.5.0)
    * `SdFat`
    * `RTC_NXP_Arduino` (For proper PCF85063TP chip matching)
