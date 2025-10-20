#pragma once

#include "SwitchController.h"
#include <Arduino.h>
#include <stdint.h>

const int SP4T_SIGNAL_COUNT = 4; // Amount of signals per SP4T-Switch

class SP4TSwitch : public SwitchController
{
    private:
        int pinMapping[SP4T_SIGNAL_COUNT]; // GPIO Pin-Mapping for the SP4T-Switch

    public:
        SP4TSwitch(const int pins[SP4T_SIGNAL_COUNT]);
        SP4TSwitch(const int pins[SP4T_SIGNAL_COUNT], MCP23017Controller &_mcp);

        uint16_t setState(uint16_t bitmask) override;
        uint16_t getState() override; 
        void reset() override;
};