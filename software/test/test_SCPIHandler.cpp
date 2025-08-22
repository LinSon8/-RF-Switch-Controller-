#include <Arduino.h>
#include <unity.h>
#include <SCPIHandler.h>
#include <SP12TSwitchController.h>
#include <NetworkManager.h>

int switchAPins[SP12T_SIGNAL_COUNT] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
int switchBPins[SP12T_SIGNAL_COUNT] = {14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25};

SP12TSwitchController switchA(0, switchAPins);
SP12TSwitchController switchB(1, switchBPins);
SP12TSwitchController switchArray[] = {switchA, switchB};
SCPIHandler scpiHandler(switchArray, 2);
//NetworkManager networkManager;

void test_determineSwitches()
{
    int activeSwitches[2];
    determineSwitches(activeSwitches, 0x3, 2);
    TEST_ASSERT_EQUAL_INT(1, activeSwitches[0]);
    TEST_ASSERT_EQUAL_INT(1, activeSwitches[1]);
}

void test_scpiIDN()
{
    scpiHandler.processCommand("*IDN?");
    TEST_ASSERT_EQUAL_STRING("Test, 123456, Firmware 0.0.1", Serial.read());
}

void test_scpiSetGetState()
{
    scpiHandler.processCommand("SP12TA:STATE?");
    TEST_ASSERT_EQUAL_STRING("0", Serial.read());
    scpiHandler.processCommand("SP12TA:STATE:0x3");
    scpiHandler.processCommand("SP12TA:STATE?");
    TEST_ASSERT_EQUAL_STRING("3", Serial.read());
}

void test_scpiSetMultipleSwitches()
{
    // Test 0000 0000 0001   1001 0000 1001   0011 = 0x19093
    scpiHandler.processCommand("SP12TALL=0x19093");
    scpiHandler.processCommand("SP12TA:STATE?");
    TEST_ASSERT_EQUAL_STRING("2313", Serial.read());
    scpiHandler.processCommand("SP12TB:STATE?");
    TEST_ASSERT_EQUAL_STRING("1", Serial.read());
}

void setup()
{
    delay(2000);
    UNITY_BEGIN();
    RUN_TEST(test_determineSwitches);
    RUN_TEST(test_scpiIDN);
    RUN_TEST(test_scpiSetGetState);
    RUN_TEST(test_scpiSetMultipleSwitches);
    UNITY_END();
}

void loop()
{
}