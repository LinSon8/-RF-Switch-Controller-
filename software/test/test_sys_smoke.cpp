#include <unity.h>
#include "SCPIHandler.h"
#include "MCP23017Controller.h"
#include "ConfigManager.h"

bool i2cDevicePresent(uint8_t addr) 
{
    Wire.beginTransmission(addr);
    return (Wire.endTransmission() == 0);
}

bool hasMCP23017() {
    return i2cDevicePresent(0x20) || i2cDevicePresent(0x21) || 
           i2cDevicePresent(0x22) || i2cDevicePresent(0x23) ||
           i2cDevicePresent(0x24) || i2cDevicePresent(0x25) ||
           i2cDevicePresent(0x26) || i2cDevicePresent(0x27);
}

void setUp()
{

}

void tearDown()
{

}


void test_device_boots_and_has_heap()
{
    TEST_ASSERT_TRUE_MESSAGE(true, "Firmware läuft");
    TEST_ASSERT_GREATER_THAN(50 * 1024, ESP.getFreeHeap());
}

void test_i2c_bus_and_mcp23017()
{
    TEST_ASSERT_TRUE_MESSAGE(Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN), "Failed to start I2C"); // Initialize I2C for MCP23017
    delay(100); // Wait for I2C bus to stabilize

    bool anyDevice = false;
    for (uint8_t addr = 0x08; addr <= 0x77; addr++) 
    {
        if (i2cDevicePresent(addr)) { anyDevice = true; break;}
    }

    TEST_ASSERT_TRUE_MESSAGE(anyDevice, "No I2C Device found - check Cable/Power");

    // Check for MCP23017 specifically
    TEST_ASSERT_TRUE_MESSAGE(hasMCP23017(), "MCP23017 not found at any address 0x20-0x27");
}

void test_spi_w5500()
{
    ConfigManager configManager;
    TEST_ASSERT_TRUE_MESSAGE(configManager.setupEthernet(standardIP, standardSubnet), "W5500 not found or failed to initialize");
}

void setup()
{
    delay(1500); // Wait for Serial to initialize
    Serial.begin(115200);

    UNITY_BEGIN();
    RUN_TEST(test_device_boots_and_has_heap);
    RUN_TEST(test_i2c_bus_and_mcp23017);
    RUN_TEST(test_spi_w5500);
    UNITY_END();

    vTaskDelay(portMAX_DELAY); 
}

void loop()
{

}