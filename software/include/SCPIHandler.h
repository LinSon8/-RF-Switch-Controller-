#pragma once

#include <Arduino.h>
#include <stdlib.h>
#include "SwitchManager.h"
#include "ConfigManager.h"

// using configManager Instance of main.cpp with "extern"
extern ConfigManager configManager;

class SCPIHandler 
{
    private:
        SwitchManager &switchManager;

    public:
        SCPIHandler(SwitchManager &manager);

        void processCommand(const String &CMD);
};