#include <Arduino.h>
#include <unity.h>
#include <NetworkManager.h>

NetworkManager networkManager;
IPAddress ip(192, 168, 1, 50);
IPAddress subnet(255, 255, 200, 0);

void test_saveLoadNetworkSettings()
{
    networkManager.saveNetworkSettings(ip, subnet);

    IPAddress loadedIP, loadedSubnet;
    networkManager.loadNetworkSettings(loadedIP, loadedSubnet);

    TEST_ASSERT_EQUAL_UINT32(ip, loadedIP);
    TEST_ASSERT_EQUAL_UINT32(subnet, loadedSubnet);
}

void test_setupEthernet()
{
    EthernetServer server(5025);
    
    networkManager.setupEthernet();

    server.begin();
    TEST_ASSERT_TRUE(server);
}

void setup()
{
    delay(2000);
    UNITY_BEGIN();
    RUN_TEST(test_saveLoadNetworkSettings);
    RUN_TEST(test_setupEthernet);
    UNITY_END();
}

void loop()
{
}