#include "ConfigManager.h"

#define RW_MODE false
#define RO_MODE true

uint8_t socketBuffer[2048]; // Buffer for socket data

// Standard MAC-Address
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress standardIP = {192, 168, 2, 150}; 
IPAddress standardSubnet = {255, 255, 255, 0}; 

// SPI-Callback functions for W5500
void wizchip_select()               { digitalWrite(CS_PIN, LOW); } 
void wizchip_deselect()             { digitalWrite(CS_PIN, HIGH); } 
uint8_t wizchip_read()              { return SPI.transfer(0x00); }
void wizchip_write(uint8_t byte)    { SPI.transfer(byte); }

// Save the IP & Subnet into the NVS-Storage (Non-volatile storage)
void ConfigManager::saveNetworkSettings(IPAddress ip, IPAddress subnet)
{
    // Open the NVS-Storage (Read-Write-Access)
    prefs.begin("network", RW_MODE);

    if (prefs.isKey("ip") && prefs.isKey("subnet"))
    {
        Serial.println("IP Address and Subnet already exist, overwriting...");
    } 
    else if (prefs.isKey("ip") || prefs.isKey("subnet")) 
    {
        Serial.println("IP Address or Subnet already exist, overwriting...");
    } 
    else
    {
        Serial.println("Saving IP Address and Subnet to NVS-Storage...");
    }

    prefs.putUInt("ip", (uint32_t) ip);
    prefs.putUInt("subnet", (uint32_t) subnet);

    // Close the Storage
    prefs.end();

    Serial.print("Saved IP Address: ");
    Serial.println(ip);
    Serial.print("Subnet: ");
    Serial.println(subnet);
    Serial.println("Netzwerkeinstellungen gespeichert!");
}

// Load saved IP & Subnet from the NVS-Storage or use the standard IP & Subnet
void ConfigManager::loadNetworkSettings(IPAddress &ip, IPAddress &subnet)
{
    // Open NVS-Storage (Read-Only)
    prefs.begin("network", RO_MODE);

    if (prefs.isKey("ip") && prefs.isKey("subnet")) 
    {
        Serial.println("IP Address and Subnet found in NVS-Storage, loading saved values...");
    } 
    else
    {
        Serial.println("IP Address and Subnet not found in NVS-Storage, loading standard values...");
    } 

    uint32_t storedIP = prefs.getUInt("ip", (uint32_t) standardIP);
    uint32_t storedSubnet = prefs.getUInt("subnet", (uint32_t) standardSubnet);

    prefs.end();

    ip = IPAddress(storedIP);
    subnet = IPAddress(storedSubnet);

    Serial.print("Loaded IP Address: ");
    Serial.println(ip);
    Serial.print("Subnet: ");
    Serial.println(subnet);
}

// Start Ethernet server with saved network settings or default settings
void ConfigManager::setupEthernet()
{
    IPAddress ip, subnet;
    // load network data from NVS-Storage if available, otherwise load the standard data
    loadNetworkSettings(ip, subnet);

    // start Ethernet
    pinMode(CS_PIN, OUTPUT); 
    wizchip_deselect(); // set CS pin high (not active)
    delay(100); 
    Serial.println("Configuring CS pin for W5500...");

    SPI.begin(SCLK_PIN, MISO_PIN, MOSI_PIN, CS_PIN); // Initialize SPI with custom pins
    Serial.println("SPI initialized for W5500...");
    
    SPI.setFrequency(500000); // Set SPI frequency to 
    Serial.println("SPI frequency set to 1MHz...");

    reg_wizchip_cs_cbfunc(wizchip_select, wizchip_deselect); // Register CS callback functions
    Serial.println("CS callback functions registered...");
    reg_wizchip_spi_cbfunc(wizchip_read, wizchip_write); // Register SPI callback functions
    Serial.println("SPI callback functions registered...");

    wizchip_sw_reset();
    delay(100); // Wait for W5500 to reset
    Serial.println("W5500 reset complete...");
    Serial.println(getVERSIONR(), HEX); // Print W5500 version
    for (int i = 0; i < _WIZCHIP_SOCK_NUM_; i++) {
    wiznet_close(i);
    setSn_CR(i, Sn_CR_CLOSE);
    }

    uint8_t txsize[1] = {2}; // 2KB TX buffer size
    uint8_t rxsize[1] = {2}; // 2KB RX buffer size
    Serial.println("Setting TX and RX buffer sizes...");
    if (wizchip_init(txsize, rxsize) == 0) // Initialize W5500 with buffer sizes
    {
        Serial.println("W5500 initialization successful!");
    }
    else
    {
        Serial.println("W5500 initialization failed!");
    }

    for (int i = 0; i < 8; i++) 
    {
        Serial.print("Socket ");
        Serial.print(i);
        Serial.print(" Status: 0x");
        Serial.println(getSn_SR(i), HEX);
    }

    wiz_PhyConf phyconf;

    Serial.print("PHY Link vorher: ");
    Serial.println(phyconf.link == PHY_LINK_ON ? "ON" : "OFF");

    phyconf.duplex = PHY_DUPLEX_FULL;
    phyconf.speed = PHY_SPEED_100;
    phyconf.link = PHY_LINK_ON;
    ctlwizchip(CW_GET_PHYCONF, (void*)&phyconf);

    Serial.print("PHY Link nachher: ");
    Serial.println(phyconf.link == PHY_LINK_ON ? "ON" : "OFF");

    uint8_t version = 0xFF;

    digitalWrite(CS_PIN, LOW);
    SPI.beginTransaction(SPISettings(500000, MSBFIRST, SPI_MODE0));
    SPI.transfer(0x00);  // Address byte high
    SPI.transfer(0x39);  // Address byte low (VERSIONR = 0x0039)
    SPI.transfer(0x00);  // Read command
    version = SPI.transfer(0x00);  // Read data
    SPI.endTransaction();
    digitalWrite(CS_PIN, HIGH);

    Serial.printf("Read VERSIONR manually: 0x%02X\n", version);

    version = getVERSIONR();
    Serial.printf("W5500 Version: 0x%02X\n", version);

    // Set network information
    wiz_NetInfo netInfo;
    memcpy(netInfo.mac, mac, 6); 
    for (int i = 0; i < 4; i++)
    {
        netInfo.ip[i] = ip[i]; 
        netInfo.sn[i] = subnet[i]; 
    }
    netInfo.dhcp = NETINFO_STATIC; // Set to static mode
    wizchip_setnetinfo(&netInfo);

    Serial.print("Started Ethernet with IP-Adress: ");
    for (byte b = 0; b < 4; b++)
    {
        Serial.print(netInfo.ip[b], DEC);
        if (b < 3) Serial.print(".");
    }
    Serial.println();
    // Für test zwecke
    //Serial.print("MAC-Adress: ");
    //for (byte b = 0; b < 6; b++)
    //{
    //    Serial.print(netInfo.mac[b], HEX);
    //    if (b < 5) Serial.print("-");
    //}
    //Serial.println();
}