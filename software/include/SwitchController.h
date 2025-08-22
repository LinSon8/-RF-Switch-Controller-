#pragma once

#include <Arduino.h>
#include <ComponentController.h>
#include <MCP23017Controller.h>

class SwitchController : public ComponentController
{
    protected:
        uint16_t currentState;
        MCP23017Controller *mcp; 

    public:
        bool isMCPUsed = false; // Flag to indicate if MCP23017 is used

        SwitchController();
        virtual void setState(uint16_t bitmask) override = 0;
        virtual int getState() override = 0;
        virtual void reset() override = 0;
};