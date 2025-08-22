#pragma once

#include "SwitchController.h"
#include <Arduino.h>
#include <stdint.h>

const int SP6T_SIGNAL_COUNT = 6; // Amount of signals per SP4T-Switch

class SP6TSwitch : public SwitchController
{
    private:
        int pinMapping[SP6T_SIGNAL_COUNT]; // GPIO Pin-Mapping for the SP4T-Switch

    public:
        SP6TSwitch(const int pins[SP6T_SIGNAL_COUNT]);
        SP6TSwitch(const int pins[SP6T_SIGNAL_COUNT], MCP23017Controller &_mcp);

        void setState(uint16_t bitmask) override;
        int getState() override; 
        void reset() override;
};