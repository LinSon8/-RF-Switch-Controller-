#include "MCP23017Controller.h"

#define IODIRA 0x00
#define IODIRB 0x01
#define GPIOA  0x12
#define GPIOB  0x13
#define OLATA  0x14
#define OLATB  0x15

MCP23017Controller::MCP23017Controller(uint8_t address) : _address(address) {}

bool MCP23017Controller::begin() 
{
    Wire.beginTransmission(_address);
    // Check if the device is connected
    if (Wire.endTransmission() == 0) 
    {
        Serial.println("MCP23017 found at address " + String(_address, HEX));
        writeRegister(IODIRA, 0x0); // Set all pins of port A as outputs
        writeRegister(IODIRB, 0x0); // Set all pins of port B as outputs
        return true; 
    } 
    else if (Wire.endTransmission() == 1) 
    {
        Serial.println("Error 1: MCP23017 not found at address " + String(_address, HEX));
        return false; 
    } 
    else if (Wire.endTransmission() == 2) 
    {
        Serial.println("Error 2: MCP23017 not found at address " + String(_address, HEX));
        return false; 
    } 
    else if (Wire.endTransmission() == 3) 
    {
        Serial.println("Error 3: MCP23017 not found at address " + String(_address, HEX));
        return false; 
    }
    else if (Wire.endTransmission() == 4) 
    {
        Serial.println("Error 4: MCP23017 not found at address " + String(_address, HEX));
        return false; 
    } 
    else if (Wire.endTransmission() == 5) 
    {
        Serial.println("Error 5: MCP23017 not found at address " + String(_address, HEX));
        return false; 
    } 
    else 
    {
        Serial.println("MCP23017 not found at address " + String(_address, HEX));
        return false; 
    }
}

void MCP23017Controller::pinMode(uint8_t pin, bool isOutput) 
{   
    Serial.println("pinMode");
    Serial.printf("Pin: %d, Value: %d\n", pin, isOutput);
    uint8_t reg = (pin < 8) ? IODIRA : IODIRB;  // Determine which register to write to
    Serial.printf("Register: 0x%02X\n", reg);
    uint8_t bit = pin % 8;                      // Get the bit position within the register
    Serial.printf("Bit: %d\n", bit);
    uint8_t val = readRegister(reg);            // Read the current value of the register
    for (int i = 7; i >= 0; i--) 
    {
        Serial.print((val >> i) & 1);
    }

    if (isOutput) val &= ~(1 << bit);           // Clear the bit to set it as output  
    else val |= (1 << bit);                     // Set the bit to input

    writeRegister(reg, val);                    // Write the new value back to the register
}

void MCP23017Controller::digitalWrite(uint8_t pin, bool value) 
{
    Serial.println("digitalWrite");
    Serial.printf("Pin: %d, Value: %d\n", pin, value);
    uint8_t reg = (pin < 8) ? OLATA : OLATB;    // Determine which register to write to
    Serial.printf("Register: 0x%02X\n", reg);
    uint8_t bit = pin % 8;                      // Get the bit position within the register
    Serial.printf("Bit: %d\n", bit);
    uint8_t val = readRegister(reg);            // Read the current value of the register
    for (int i = 7; i >= 0; i--) 
    {
        Serial.print((val >> i) & 1);
    }
    Serial.println();

    if (value) val |= (1 << bit);               // Set the bit to high
    else val &= ~(1 << bit);                    // Clear the bit to low

    writeRegister(reg, val);                    // Write the new value back to the register
}

bool MCP23017Controller::digitalRead(uint8_t pin) 
{
    uint8_t reg = (pin < 8) ? GPIOA : GPIOB;    // Determine which register to read from
    uint8_t bit = pin % 8;                      // Get the bit position within the register
    uint8_t val = readRegister(reg);            // Read the current value of the register

    return (val & (1 << bit)) != 0;             // Return the state of the specified pin
}

uint8_t MCP23017Controller::readRegister(uint8_t reg) 
{   
    Serial.printf("Reading register: 0x%02X\n", reg);
    Serial.printf("I2C address: 0x%02X\n", _address);
    Wire.beginTransmission(_address);           // Start I2C transmission
    Wire.write(reg);                            // Send the register address
    Wire.endTransmission();                     // End transmission
    Serial.println("Requesting data from device...");
    Wire.requestFrom(_address, (uint8_t) 1);    // Request 1 byte from the device
    Serial.println("Data received from device.");
    return Wire.available() ? Wire.read() : 0;  // Read and return the value, or 0 if no data available
}

void MCP23017Controller::writeRegister(uint8_t reg, uint8_t value) 
{
    Wire.beginTransmission(_address);           // Start I2C transmission
    Wire.write(reg);                            // Send the register address
    Wire.write(value);                          // Send the value to write
    Wire.endTransmission();                     // End transmission
}