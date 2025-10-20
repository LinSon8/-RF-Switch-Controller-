# RF-Switch-Controller


This project was developed as part of my bachelor’s thesis at Technische Universität Berlin. It demonstrates the design of a modular, network-based control unit for hardware test automation.
The goal is to provide a cost-effective and flexible solution to control RF switches, attenuators, and relays using **SCPI-Befehle (Standard Commands for Programmable Instruments)**.

## Motivation
Commercial control solutions for switching components often cost several thousand euros.
However, many individual components (e.g., relays or attenuators) are available at low cost, they simply lack a network interface.

The purpose of this project is to develop a **universal control unit** based on an **ESP32-S3 DevKitC-1** featuring:
- Control via **Ethernet (W5500 Chip, SPI, ioLibrary_Driver)**
- Communication using **SCPI/TCP (laboratory equipment standard)**
- Modular expansion for various switching components
- 
## Hardware / Software
- **Board:** ESP32-S3 DevKitC-1  
- **Framework:** Arduino (PlatformIO, VS Code)  
- **Network:** W5500 (SPI, ioLibrary_Driver)  
- **Storage:** Preferences (NVS) for network configuration
- **I/O (optional):** MCP23017 GPIO-Expander for relay/switch matrices

## Quickstart

### 1) Clone the repository

git clone [https://github.com/<dein-user>/esp32-s3-scpi-switch.git](https://github.com/LinSon8/-RF-Switch-Controller-.git)

cd rf-switch-controller

### 2) Build with PlatformIO

pio run -t upload

### 3) Configure network settings

IP: 10.0.0.220
Subnet: 255.255.255.0
Port: 5025

These can be adjusted in the code (Config.cpp).


## SCPI Commands (Basic)

- *IDN?
- *RST

- SPDTMULTI=
- SP4TMULTI=
- SP6TMULTI=
- SP12TMULTI=

Multiple switch states can be configured using either a decimal or binary representation (e.g., 0b101 for SPDT switches A–C). Here, the least significant bit (LSB) corresponds to SPDTA and the most significant bit (MSB) to SPDTC.

- SPDTX:STATE?
- SP4TX:STATE?
- SP6TX:STATE?
- SP12TX:STATE?
- SPDTX:STATE:
- SP4TX:STATE:
- SP6TX:STATE:
- SP12TX:STATE:

The placeholder X denotes a specific switch instance, identified by its capital letter (e.g., SP4TA).

To set the state of a switch, you can either use a decimal value or the corresponding binary value (e.g., 0b0110 for an SP4T switch).


