#pragma once

#include "SwitchController.h"
#include <Arduino.h>
#include <stdint.h>

const int SP12T_SIGNAL_COUNT = 12;   // Amount of signals per SP12T-Switch

class SP12TSwitch : public SwitchController
{
    private:
        int pinMapping[SP12T_SIGNAL_COUNT]; // GPIO Pin-Mapping for the SP12T-Switch

    public:
        SP12TSwitch(const int pins[SP12T_SIGNAL_COUNT]);
        SP12TSwitch(const int pins[SP12T_SIGNAL_COUNT], MCP23017Controller &_mcp);

        void setState(uint16_t bitmask) override;
        int getState() override; 
        void reset() override;
};