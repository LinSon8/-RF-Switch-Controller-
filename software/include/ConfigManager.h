#pragma once

#include <Arduino.h>
#include <Preferences.h>
#include <SPI.h>
#include "WiznetWrapper.h"

#ifdef MR
  #undef MR
#endif

#define _WIZCHIP_              W5500
#define _WIZCHIP_IO_MODE_      _WIZCHIP_IO_MODE_SPI

extern "C" 
{
    #include "wizchip_conf.h"
    #include "w5500.h"
}

const int TCP_PORT = 5025; // SCPI TCP Port
const int SOCKET_NUM = 0; // Socket number for SCPI TCP connection
const int CS_PIN = 10; // Chip Select pin for W5500
const int MOSI_PIN = 11; // SPI MOSI pin
const int SCLK_PIN = 12; // SPI Clock pin
const int MISO_PIN = 13; // SPI MISO pin
extern uint8_t socketBuffer[2048]; // Buffer for socket data


class ConfigManager
{
    private:
        Preferences prefs;

    public:
        void saveNetworkSettings(IPAddress ip, IPAddress subnet);
        void loadNetworkSettings(IPAddress &ip, IPAddress &subnet);
        void setupEthernet();
};

// SPI-Callbackfunktionen
void wizchip_select();
void wizchip_deselect();
uint8_t wizchip_read();
void wizchip_write(uint8_t byte);