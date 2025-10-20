#pragma once

#include <Arduino.h>
#include <stdint.h>
#include "MCP23017Controller.h"

class AttenuatorController
{
    protected:
        float currentAttenuation;
        MCP23017Controller *mcp; 

    public:
        bool isMCPUsed = false; // Flag to indicate if MCP23017 is used
        
        AttenuatorController();
        virtual float setAttenuation(int* dB) = 0;
        virtual float getAttenuation() = 0;
        virtual void reset() = 0;
};