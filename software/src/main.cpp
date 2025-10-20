#ifndef PIO_UNIT_TESTING
#include "WiznetWrapper.h"
#include "SCPIHandler.h"
#include "SPDTSwitch.h"
#include "SP4TSwitch.h"
#include "SP6TSwitch.h"
#include "SP12TSwitch.h"
#include "ConfigManager.h"
#include "SwitchManager.h"
#include "MCP23017Controller.h"

static ConnTimer conn[MAX_SOCKETS];

ConfigManager configManager;                // initialze ConfigManager
SwitchManager switchManager;                // initialze SwitchManager
SCPIHandler scpiHandler(switchManager);     // initialze SCPIHandler
MCP23017Controller mcp1(0x20);              // initialze MCP23017Controller

int sp12tAPins[SP12T_SIGNAL_COUNT] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
//int sp12tBPins[SP12T_SIGNAL_COUNT] = {14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25};
//int sp6tAPins[SP6T_SIGNAL_COUNT] = {26, 27, 28, 29, 30, 31};
//int sp4tAPins[SP4T_SIGNAL_COUNT] = {0, 1, 2, 3}; // GPIO Pins for SP4T Switch A
//int sp4tBPins[SP4T_SIGNAL_COUNT] = {4, 5, 6, 7}; // GPIO Pins for SP4T Switch B
//int sp4tCPins[SP4T_SIGNAL_COUNT] = {8, 9, 10, 11}; // GPIO Pins for SP4T Switch C
int sp4tDPins[SP4T_SIGNAL_COUNT] = {12, 13, 14, 15}; // GPIO Pins for SP4T Switch D
//int spdtAPin = 36;

static inline void touch(uint8_t sock) 
{
    conn[sock].lastActivityms = millis();
}

int8_t findClosedSocket() 
{
    for (uint8_t sock = 0; sock < MAX_SOCKETS; sock++) 
    {
        if (getSn_SR(sock) == SOCK_CLOSED) return sock;
    }
    return -1;
}

int8_t pickLRUVictim()
{
    uint8_t lruVicitim = 255;
    uint32_t oldest = 0;
    uint32_t now = millis();

    for (uint8_t sock = 0; sock < MAX_SOCKETS; sock++)
    {
        uint8_t sr = getSn_SR(sock);

        if (sr == SOCK_ESTABLISHED && !conn[sock].locked) 
        {
            uint32_t age = now - conn[sock].lastActivityms;
            if (age > oldest || lruVicitim == 255) 
            {
                oldest = age;
                lruVicitim = sock;
            }
        }
    }
    return (lruVicitim == 255) ? -1 : (int8_t)lruVicitim;
}

void  dropSocket(uint8_t sock) 
{
    Serial.print("Dropping socket ");
    Serial.println(sock);
    wiznet_close(sock);
    conn[sock] = ConnTimer{};
}

void ensureListener() 
{
    for (uint8_t sock = 0; sock < MAX_SOCKETS; sock++) 
    {
        //if (st != SOCK_CLOSED) if (getSn_PORT(s) == port) return true;
        // Extra Vorsicht: falls ein Socket gerade initialisiert, aber noch nicht LISTEN ist
        //if (st == SOCK_INIT && getSn_PORT(s) == port) return true;
        if (getSn_SR(sock) == SOCK_LISTEN) return;
    }
     // Find a closed socket to use for listening

    int8_t sock = findClosedSocket();
    if (sock < 0) 
    {
        Serial.println("No closed socket available, looking for LRU victim");
        int8_t sockVictim = pickLRUVictim();
        if (sockVictim >= 0) 
        {
            dropSocket((uint8_t) sockVictim);
            sock = findClosedSocket();
        }
        else
        {
            Serial.println("No socket available to listen");
            return;
        }
    }
    if (sock >= 0) 
    {  
        int test = wiznet_socket(sock, Sn_MR_TCP, TCP_PORT, 0);
        Serial.printf("socket(%d)->%d SR=0x%02X\n", sock, test, getSn_SR(sock));
        if (test == sock) 
        {
            Serial.print("Listening on socket ");
            Serial.println(sock);
            int listenResult = wiznet_listen(sock);
            if (listenResult != 1) 
            {
                Serial.print("listen() failed with code: ");
                Serial.println(listenResult);
                Serial.printf("listen(%d) failed, SR=0x%02X\n", sock, getSn_SR(sock));
                wiznet_close(sock);
                return;
            }
            conn[sock].lastActivityms = millis();
        }
        else 
        {
            Serial.println("socket() failed");
            uint8_t sr   = getSn_SR(sock);
            uint8_t rxkb = getSn_RXBUF_SIZE(sock);
            uint8_t txkb = getSn_TXBUF_SIZE(sock);
            printf("socket(%d) FAILED: ret=%d SR=0x%02X RX=%uKB TX=%uKB\r\n", sock, test, sr, rxkb, txkb);
        }
    }
}

void setup() 
{
    Serial.begin(115200);
    Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN); // Initialize I2C for MCP23017
    Wire.setClock(100000); // Set I2C clock speed to 100kHz

    // Setting up the SPI bus for W5500 and Network Settings
    configManager.setupEthernet(standardIP, standardSubnet);
    Serial.println("Ethernet started");

    mcp1.begin(); // Set all pins of MCP23017 as outputs
    Serial.println("nach mcp begin");

    // Start SCPI Server
    ensureListener();

    switchManager.addSwitch("SP12TA", new SP12TSwitch(sp12tAPins, mcp1));
    //switchManager.addSwitch("SP12TB", new SP12TSwitch(sp12tBPins));
    //switchManager.addSwitch("SP6TA", new SP6TSwitch(sp6tAPins));
    //switchManager.addSwitch("SP4TA", new SP4TSwitch(sp4tAPins, mcp1));
    //switchManager.addSwitch("SP4TB", new SP4TSwitch(sp4tBPins, mcp1));
    //switchManager.addSwitch("SP4TC", new SP4TSwitch(sp4tCPins, mcp1));
    switchManager.addSwitch("SP4TD", new SP4TSwitch(sp4tDPins, mcp1));
    //switchManager.addSwitch("SPDTA", new SPDTSwitch(spdtAPin));
}

void loop() 
{   
    //uint8_t ir = getSn_IR(SOCKET_NUM);
    //if (ir != 0) 
    //{
    //    Serial.print("Sn_IR: 0x"); 
    //    Serial.println(ir, HEX);
    //}
    
    // Get a client that is connected to the server and has data to read
    for (uint8_t sock = 0; sock < MAX_SOCKETS; sock++) 
    {
        //uint8_t socketStatus = getSn_SR(sock);
        //Serial.print("Socket: " + String(sock) + " - ");
        //Serial.printf("Socket state: 0x%02X\n", socketStatus);
        switch (getSn_SR(sock)) 
        {
            case SOCK_LISTEN:
                Serial.println("Waiting for client connection on socket: " + String(sock) + " ...");
                break;
            case SOCK_ESTABLISHED:
                if (getSn_IR(sock) & Sn_IR_CON) 
                {
                    setSn_IR(sock, Sn_IR_CON); // Clear the connection interrupt flag
                    Serial.println("Client connected on socket " + String(sock));

                    const char* banner = "SCPI-READY\r\n";
                    wiznet_send(sock, (uint8_t*)banner, strlen(banner));
                }
                else Serial.println("Client still connected on socket: " + String(sock)); 

                if (getSn_RX_RSR(sock) > 0) 
                {
                    // Read the data from the client
                    int len = wiznet_recv(sock, socketBuffer, sizeof(socketBuffer));
                    if (len > 0) 
                    {
                        conn[sock].locked = true; // lock the connection during command processing
                        String cmd = "";

                        for (int i = 0; i < len; i++) 
                        {
                            cmd += (char)socketBuffer[i]; // Convert byte to char and append to command string
                        }
                        // Process the received data
                        Serial.print("Received data: ");
                        Serial.println(cmd);
                        scpiHandler.processCommand(cmd, sock); // Process the command using SCPIHandler
                        const char* reply = "OK\r\n";
                        wiznet_send(sock, (uint8_t*)reply, strlen(reply));
                        touch(sock); // Update last activity time
                        conn[sock].locked = false; // unlock the connection after command processing
                    }
                }
                break;

            case SOCK_CLOSE_WAIT:
                int len = wiznet_recv(sock, socketBuffer, sizeof(socketBuffer));
                if (len > 0) 
                {
                    conn[sock].locked = true; // lock the connection during command processing
                    String cmd = "";

                    for (int i = 0; i < len; i++) 
                    {
                        cmd += (char)socketBuffer[i]; // Convert byte to char and append to command string
                    }

                    // Process the received data
                    Serial.print("Received data: ");
                    Serial.println(cmd);
                    scpiHandler.processCommand(cmd, sock); // Process the command using SCPIHandler
                    const char* reply = "OK\r\n";
                    wiznet_send(sock, (uint8_t*)reply, strlen(reply));
                    touch(sock); // Update last activity time
                    conn[sock].locked = false; // unlock the connection after command processing
                }

                const char* reply2 = "SOCKET CLOSED\r\n";
                wiznet_send(sock, (uint8_t*)reply2, strlen(reply2));
                Serial.println("Client disconnected, closing socket");
                // Close the socket if the client is disconnected
                dropSocket(sock);
                break;
                
            //case SOCK_CLOSED:
            //    wiznet_socket(SOCKET_NUM, Sn_MR_TCP, TCP_PORT, 0); // Create a new socket for TCP connection
            //    wiznet_listen(SOCKET_NUM); // Listen for incoming connections
            //    Serial.println("Server ready for new connections");
            //    Serial.printf("Server listening on Port %d", TCP_PORT);
            //    Serial.println("");
            //    break;
        }
    }
    ensureListener();
    delay(4000);
}

#endif