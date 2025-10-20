# include <Arduino.h>
#include <unity.h>
#include "ConfigManager.h"


ConfigManager configManager;

void test_network_up()
{
    bool ok = configManager.setupEthernet(standardIP, standardSubnet);
    TEST_ASSERT_TRUE_MESSAGE(ok, "W5500 not found or failed to initialize network");
}

void test_has_valid_ip_subnet()
{
    configManager.saveNetworkSettings(standardIP, standardSubnet);
    TEST_ASSERT_TRUE_MESSAGE(standardIP[0] != 0 || standardIP[1] != 0 || standardIP[2] != 0 || standardIP[3] != 0, "Loaded IP is invalid");
    TEST_ASSERT_TRUE_MESSAGE(standardSubnet[0] != 0 || standardSubnet[1] != 0 || standardSubnet[2] != 0 || standardSubnet[3] != 0, "Loaded Subnet is invalid");
}

void test_network_persistence()
{
    IPAddress testIP, testSubnet;
    configManager.setupEthernet(standardIP, standardSubnet);
    configManager.loadNetworkSettings(testIP, testSubnet);
    TEST_ASSERT_EQUAL_MESSAGE(standardIP, testIP, "Loaded IP does not match standard IP");
    TEST_ASSERT_EQUAL_MESSAGE(standardSubnet, testSubnet, "Loaded Subnet does not match standard Subnet");

    configManager.saveNetworkSettings(standardIP, standardSubnet);
    configManager.loadNetworkSettings(testIP, testSubnet);
    TEST_ASSERT_EQUAL_MESSAGE(standardIP, testIP, "Loaded IP does not match saved IP");
    TEST_ASSERT_EQUAL_MESSAGE(standardSubnet, testSubnet, "Loaded Subnet does not match saved Subnet");
}

//void setup()
//{
//    delay(2000);
//    UNITY_BEGIN();
//    RUN_TEST(test_network_up);
//    RUN_TEST(test_has_valid_ip_subnet);
//    RUN_TEST(test_network_persistence);
//    UNITY_END();
//}
//
//void loop()
//{
//}