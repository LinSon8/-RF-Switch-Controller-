#include <Arduino.h>
#include <unity.h>
#include <SCPIHandler.h>
#include <Ethernet.h>

EthernetClient client;

void setUp()
{

}

void tearDown()
{

}

void test_SCPI_IDN()
{
    //String response = SCPIHandler::processCommand("*IDN?", client);
    //TEST_ASSERT_EQUAL_STRING("Test, 123456, Firmware 0.1", response.c_str());
}

void setup()
{
    UNITY_BEGIN();

    UNITY_END();
}

void loop()
{

}