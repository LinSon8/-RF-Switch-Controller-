# RF-Switch-Controller


Dieses Projekt ist Teil meiner Bachelorarbeit an der Technischen Universität Berlin und zeigt die Entwicklung einer **modularen, netzwerkbasierten Steuereinheit** für Hardware-Testautomatisierung.  
Ziel ist es, eine **kostengünstige und flexible Lösung** bereitzustellen, mit der sich RF-Schalter, Dämpfungsglieder und Relais über **SCPI-Befehle (Standard Commands for Programmable Instruments)** ansteuern lassen.

## Motivation
Kommerzielle Steuerlösungen für Schaltkomponenten kosten oft mehrere tausend Euro.  
Viele Einzelkomponenten (z. B. Relais oder Dämpfungsglieder) sind jedoch günstig verfügbar – sie besitzen allerdings keine Netzwerkschnittstelle.

Dieses Projekt verfolgt daher das Ziel, eine **universelle Steuerungseinheit** auf Basis eines **ESP32-S3 DevKitC-1** zu entwickeln:
- Steuerung über **Ethernet (W5500 Chip, SPI, ioLibrary_Driver)**
- Kommunikation via **SCPI/TCP (Standard für Laborgeräte)**
- Modular erweiterbar für verschiedene Schaltkomponenten

## 🔌 Hardware / Software
- **Board:** ESP32-S3 DevKitC-1  
- **Framework:** Arduino (PlatformIO, VS Code)  
- **Netzwerk:** W5500 (SPI, ioLibrary_Driver)  
- **Speicher:** Preferences (NVS) für Netzwerkkonfiguration  
- **I/O (optional):** MCP23017 GPIO-Expander für Relais-/Schaltmatrizen

## Quickstart

### 1) Repository klonen

git clone [https://github.com/<dein-user>/esp32-s3-scpi-switch.git](https://github.com/LinSon8/-RF-Switch-Controller-.git)
cd rf-switch-controller

### 2) Build mit PlatformIO

pio run -t upload

### 3) Netzwerk konfigurieren

Die IP- und Subnet-Adressen werden beim ersten Start in Preferences (NVS) gespeichert.
Standardwerte:

IP: 192.168.2.150
Subnet: 255.255.255.0

Diese können im Code (ConfigManager.cpp) angepasst werden.


### SCPI-Befehle (Basis)

*IDN?
*RST
SPDTMULTI=
SP4TMULTI=
SP6TMULTI=
SP12TMULTI=
SPDT:STATE?
SP4T:STATE?
SP6T:STATE?
SP12T:STATE?
SPDT:STATE:
SP4T:STATE:
SP6T:STATE:
SP12T:STATE:
