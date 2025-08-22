#pragma once

#include <Wire.h>
#include <Arduino.h>

const int I2C_SDA_PIN = 8;  // I2C SDA pin
const int I2C_SCL_PIN = 9;  // I2C SCL pin

class MCP23017Controller 
{
public:
    explicit MCP23017Controller(uint8_t address = 0x20);

    bool begin();
    void pinMode(uint8_t pin, bool isOutput); 
    void digitalWrite(uint8_t pin, bool value);
    bool digitalRead(uint8_t pin);

private:
    uint8_t _address;                                   // I2C address of the MCP23017

    void writeRegister(uint8_t reg, uint8_t value);
    uint8_t readRegister(uint8_t reg);
};