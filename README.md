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

* **Lab 1: OBC Initialization & I2C Subsystems** * *Core Tasks:* Establish Serial Debugging interfaces and execute an I2C bus scanner to find online physical addresses.
    * *Application:* Extract real-time metrics from the RTC (PCF85063TP) and temperature sensor (TMP102), printing them as a single inline data stream to implement mandatory timestamping habits.
* **Lab 2: Data Storage & Memory Management** * *Core Tasks:* Map memory bounds and read JEDEC parameters from the SPI Flash (W25Q128) while testing basic read/write sectors.
    * *Application:* Program a sector dump routine to mirror internal flash structures directly into a structured backup archive (`Backup.txt`) inside the Micro SD Card, validating integrity via physical inspection.
* **Lab 3: EPS Power Management & Safe Mode** * *Core Tasks:* Fetch power consumption telemetry from INA226 nodes and handle low-level subsystem toggles via hardware GPIO configurations (PD1–PD3).
    * *Application:* Write an autonomous `SafeMode()` routine. If the simulated battery drop hits sub-30%, trigger structural load-shedding by killing payload/radio rails. Restore standard operations once charging clears a 70% hysteresis barrier.
* **Lab 4: RF Communication & Telemetry Protocol** * *Core Tasks:* Conduct basic antenna diagnostics (S11 and VSWR matching) and interface the RFM98PW LoRa transceiver.
    * *Application:* Design a rigid data telemetry packet structure: `[Header][Timestamp][Battery %][Temp][Checksum]`. Encapsulate binary frames using the KISS protocol and verify reception on the Ground Station. Handle flag warnings if a checksum validation fails.
* **Lab 5: Payload Integration & Metadata** * *Core Tasks:* Decode NMEA sentences from GPS modules via auxiliary serial lines and synchronize external camera capture routines.
    * *Application:* Fire an image capture command, pull active time-stamps from the RTC and coordinates from the GPS, and embed them directly as geo-location payload metadata into an SD storage entry.
* **Lab 6: Resilient Downlink & Chunk Transfer** * *Core Tasks:* Segment binary picture records on the SD card into fixed-size transport chunks.
    * *Application:* Implement a robust Store-and-Forward downlink. Simulate antenna disconnection or physical path failure mid-transfer. The satellite must register the link interruption, enter a holding state, and resume packet transmission seamlessly from the exact fail-point upon connection restoration.
* **Lab 7: Orbital Constraints & Fault Tolerance** * *Application Task 1 (GPS-Gating):* Constrain radio activities to orbital windows. Build a software gating mechanism that keeps the COMMS rail disabled until the active GPS telemetry falls into preconfigured regional boundaries.
    * *Application Task 2 (Watchdog Timer):* Configure the internal Independent Watchdog (IWDG) system. Simulate a Single Event Upset (SEU) radiation strike by forcing an infinite loop lockup. The hardware watchdog must automatically identify the freeze, force a full OBC reset, and bring the platform back online without human intervention.
* **Lab 8: The Final Mission – "A Day in the Life"** * *Objective:* Evaluates structural system integration skills. Provided with zero starter code, students must apply everything learned from Labs 1 through 7 to construct a complete, autonomous Finite State Machine (FSM):
    * **State 1 (Boot):** Execute hardware health checks, read battery margins (verify Safe Mode status), and mount file systems.
    * **State 2 (Orbiting):** Keep radio systems completely silent; continually stream background telemetry and monitor GPS coordinates.
    * **State 3 (Target Acquired):** Upon entering target coordinate bounds -> Trigger camera payload -> Inject spatial metadata -> Log to SD card.
    * **State 4 (Ground Contact):** Upon passing above the Ground Station position -> Spin up the radio rail -> Stream chunks downstream.
    * **State 5 (Sleep):** Once chunk handshakes finish and Ground Station checksum confirmations clear -> Power down COMMS and revert to idle orbiting states.

## 🛠️ Getting Started

### Prerequisites

To compile and execute the flight software code in this workspace, ensure you have configured:
1.  **Arduino IDE 2.x** installed.
2.  **STM32 MCU Based Boards** core addition provided through the official Board Manager.
3.  Required External Dependencies:
    * `RadioLib` (Tested on version 7.5.0)
    * `SdFat`
    * `RTC_NXP_Arduino` (For proper PCF85063TP chip matching)
