#pragma once

#include <stdint.h>

class ComponentController
{
    public:
        virtual uint16_t setState(uint16_t bitmask) = 0;
        virtual uint16_t getState() = 0;
        virtual void reset() = 0;
        virtual ~ComponentController() {}
};