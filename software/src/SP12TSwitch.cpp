#include "SP12TSwitch.h"

SP12TSwitch::SP12TSwitch(const int pins[SP12T_SIGNAL_COUNT])
{
    for (int i = 0; i < SP12T_SIGNAL_COUNT; i++)
    {
        pinMapping[i] = pins[i];
        pinMode(pinMapping[i], OUTPUT);
        digitalWrite(pinMapping[i], LOW);
    }
}

SP12TSwitch::SP12TSwitch(const int pins[SP12T_SIGNAL_COUNT], MCP23017Controller &_mcp)
{
    isMCPUsed = true;
    mcp = &_mcp;

    for (int i = 0; i < SP12T_SIGNAL_COUNT; i++)
    {
        pinMapping[i] = pins[i];
        mcp->pinMode(pinMapping[i], OUTPUT);
        mcp->digitalWrite(pinMapping[i], LOW);
    }
}

void SP12TSwitch::setState(uint16_t bitmask)
{
    // Checking if the state is valid
    if (bitmask > 0xFFF) 
    {
        Serial.println("ERROR: Ungültiger Zustand");
        return;
    }
    // Set State
    currentState = bitmask;

    // Set GPIO Pins
    if (isMCPUsed)
    {
        for (int i = 0; i < SP12T_SIGNAL_COUNT; i++)
        {
            Serial.print("Setting pin ");
            Serial.println(pinMapping[i]);
            Serial.print(" to state: ");
            Serial.println((bitmask >> i) & 1);
            mcp->digitalWrite(pinMapping[i], (bitmask >> i) & 1);
        }
    }
    else
    {
        for (int i = 0; i < SP12T_SIGNAL_COUNT; i++)
        {
            Serial.print("Setting pin ");
            Serial.println(pinMapping[i]);
            Serial.print(" to state: ");
            Serial.println((bitmask >> i) & 1);
            digitalWrite(pinMapping[i], (bitmask >> i) & 1);
        }
    }
    
    Serial.print("Current State: ");
    Serial.println(currentState, BIN);
}

// Get current state
int SP12TSwitch::getState()
{
    return currentState;
}

// reset the switch
void SP12TSwitch::reset()
{
    setState(0);
}