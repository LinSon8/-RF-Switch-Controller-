#pragma once

#include "SwitchController.h"
#include <Arduino.h>
#include <stdint.h>

class SPDTSwitch : public SwitchController
{
    private:
        int pin; // GPIO Pin for the SPDT-Switch

    public:
        SPDTSwitch(const int pin);
        SPDTSwitch(const int pin, MCP23017Controller &_mcp);

        uint16_t setState(uint16_t bitmask) override;
        uint16_t getState() override; 
        void reset() override;
};