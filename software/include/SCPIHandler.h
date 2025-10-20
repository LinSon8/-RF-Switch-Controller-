#pragma once

#include <Arduino.h>
#include <stdlib.h>
#include "SwitchManager.h"
#include "ConfigManager.h"
#include "WiznetWrapper.h"

// using configManager Instance of main.cpp with "extern"
extern ConfigManager configManager;

class SCPIHandler 
{
    private:
        SwitchManager &switchManager;

    public:
        SCPIHandler(SwitchManager &manager);

        void processCommand(const String &CMD, uint8_t sock);
        void getBinString(char* binStr, size_t binStrSize, uint16_t state, uint8_t bitWidth);
};