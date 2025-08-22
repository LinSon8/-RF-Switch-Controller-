#include "SPDTSwitch.h"

SPDTSwitch::SPDTSwitch(const int controlPin) : pin(controlPin)
{
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
}

SPDTSwitch::SPDTSwitch(const int controlPin, MCP23017Controller &_mcp) : pin(controlPin)
{
    mcp = &_mcp;
    isMCPUsed = true;
    mcp->pinMode(pin, OUTPUT);
    mcp->digitalWrite(pin, LOW);
}

void SPDTSwitch::setState(uint16_t bitmask)
{
    // Checking if the state is valid
    if (bitmask > 0x1) 
    {
        Serial.println("ERROR: Ungültiger Zustand");
        return;
    }
    // Set State
    currentState = bitmask;

    // Set GPIO Pin either directly or through MCP23017
    if (isMCPUsed) mcp->digitalWrite(pin, bitmask);
    else digitalWrite(pin, bitmask);

    Serial.print("Current State: ");
    Serial.println(currentState, BIN);
}

// Get current state
int SPDTSwitch::getState()
{
    return currentState;
}

// reset the switch
void SPDTSwitch::reset()
{
    setState(0);
}