#include "WiznetWrapper.h"
#include "SCPIHandler.h"
#include "SPDTSwitch.h"
#include "SP4TSwitch.h"
#include "SP6TSwitch.h"
#include "SP12TSwitch.h"
#include "ConfigManager.h"
#include "SwitchManager.h"
#include "MCP23017Controller.h"

ConfigManager configManager;                // initialze ConfigManager
SwitchManager switchManager;                // initialze SwitchManager
SCPIHandler scpiHandler(switchManager);     // initialze SCPIHandler
MCP23017Controller mcp1(0x20);              // initialze MCP23017Controller

//int sp12tAPins[SP12T_SIGNAL_COUNT] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
//int sp12tBPins[SP12T_SIGNAL_COUNT] = {14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25};
//int sp6tAPins[SP6T_SIGNAL_COUNT] = {26, 27, 28, 29, 30, 31};
int sp4tAPins[SP4T_SIGNAL_COUNT] = {0, 1, 2, 3}; // GPIO Pins for SP4T Switch A
int sp4tBPins[SP4T_SIGNAL_COUNT] = {4, 5, 6, 7}; // GPIO Pins for SP4T Switch B
int sp4tCPins[SP4T_SIGNAL_COUNT] = {8, 9, 10, 11}; // GPIO Pins for SP4T Switch C
int sp4tDPins[SP4T_SIGNAL_COUNT] = {12, 13, 14, 15}; // GPIO Pins for SP4T Switch D
//int spdtAPin = 36;

void setup() 
{
    Serial.begin(115200);
    Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN); // Initialize I2C for MCP23017
    Wire.setClock(100000); // Set I2C clock speed to 100kHz

    // Setting up the SPI bus for W5500 and Network Settings
    configManager.setupEthernet();
    Serial.println("Ethernet started");

    mcp1.begin(); // Set all pins of MCP23017 as outputs
    Serial.println("nach mcp begin");

    // Start SCPI Server
    int socketStatus = wiznet_socket(SOCKET_NUM, Sn_MR_TCP, TCP_PORT, 0);
    if (socketStatus != 0) 
    {
        Serial.print("Socket creation failed with code:");
        Serial.println(socketStatus);
    }
    
    int listenResult = wiznet_listen(SOCKET_NUM); // Listen for incoming connections
    if (listenResult != 1) 
    {
        Serial.print("listen() failed with code: ");
        Serial.println(listenResult);
    }

    //switchManager.addSwitch("SP12TA", new SP12TSwitch(sp12tAPins));
    //switchManager.addSwitch("SP12TB", new SP12TSwitch(sp12tBPins));
    //switchManager.addSwitch("SP6TA", new SP6TSwitch(sp6tAPins));
    switchManager.addSwitch("SP4TA", new SP4TSwitch(sp4tAPins, mcp1));
    switchManager.addSwitch("SP4TB", new SP4TSwitch(sp4tBPins, mcp1));
    switchManager.addSwitch("SP4TC", new SP4TSwitch(sp4tCPins, mcp1));
    switchManager.addSwitch("SP4TD", new SP4TSwitch(sp4tDPins, mcp1));
    //switchManager.addSwitch("SPDTA", new SPDTSwitch(spdtAPin));
}

void loop() 
{   
    uint8_t socketStatus = getSn_SR(SOCKET_NUM);
    Serial.printf("Socket state: 0x%02X\n", socketStatus);

    uint8_t ir = getSn_IR(SOCKET_NUM);
    if (ir != 0) 
    {
        Serial.print("Sn_IR: 0x"); 
        Serial.println(ir, HEX);
    }
        // Get a client that is connected to the server and has data to read
    // Remains connected until client.stop() is called, even if the client is no longer connected
    switch (getSn_SR(SOCKET_NUM)) 
    {
        case SOCK_LISTEN:
            Serial.println("Waiting for client connection...");
            break;
        case SOCK_ESTABLISHED:
            if (getSn_IR(SOCKET_NUM) & Sn_IR_CON) 
            {
                setSn_IR(SOCKET_NUM, Sn_IR_CON); // Clear the connection interrupt flag
                Serial.println("Client connected");
            }
            else { Serial.println("Client not connected"); }

            if (getSn_RX_RSR(SOCKET_NUM) > 0) 
            {
                // Read the data from the client
                int len = wiznet_recv(SOCKET_NUM, socketBuffer, sizeof(socketBuffer));
                if (len > 0) 
                {
                    String cmd = "";
                    for (int i = 0; i < len; i++) 
                    {
                        cmd += (char)socketBuffer[i]; // Convert byte to char and append to command string
                    }
                    // Process the received data
                    Serial.print("Received data: ");
                    Serial.println(cmd);
                    scpiHandler.processCommand(cmd); // Process the command using SCPIHandler
                }

                if (len == 0) 
                {
                    Serial.println("No data send, closing socket");
                    wiznet_close(SOCKET_NUM); 
                }
            }
            break;

        case SOCK_CLOSE_WAIT:
            Serial.println("Client disconnected, closing socket");
            // Close the socket if the client is disconnected
            wiznet_close(SOCKET_NUM);
            break;
            
        case SOCK_CLOSED:
            wiznet_socket(SOCKET_NUM, Sn_MR_TCP, TCP_PORT, 0); // Create a new socket for TCP connection
            wiznet_listen(SOCKET_NUM); // Listen for incoming connections
            Serial.println("Server ready for new connections");
            Serial.printf("Server listening on Port %d", TCP_PORT);
            Serial.println("");
            break;
    }
    delay(5000);
}