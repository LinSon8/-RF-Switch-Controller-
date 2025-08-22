#pragma once

#include <stdint.h>

class ComponentController
{
    public:
        virtual void setState(uint16_t bitmask) = 0;
        virtual int getState() = 0;
        virtual void reset() = 0;
        virtual ~ComponentController() {}
};