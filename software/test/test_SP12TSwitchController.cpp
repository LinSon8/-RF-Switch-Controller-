#include <Arduino.h>
#include <unity.h>
#include <SP12TSwitchController.h>

int switchAPins[SP12T_SIGNAL_COUNT] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
SP12TSwitchController switchA(0, switchAPins);

void test_initialState()
{
    TEST_ASSERT_EQUAL_INT(0, switchA.getState());
}

void test_setState()
{
    switchA.setState(0x3);
    TEST_ASSERT_EQUAL_INT(0x3, switchA.getState());
}

void test_resetState()
{
    switchA.setState(0x3);
    switchA.reset();
    TEST_ASSERT_EQUAL_INT(0, switchA.getState());
}

void setup()
{
    delay(2000);
    UNITY_BEGIN();
    RUN_TEST(test_initialState);
    RUN_TEST(test_setState);
    RUN_TEST(test_resetState);
    UNITY_END();
}

void loop()
{
}