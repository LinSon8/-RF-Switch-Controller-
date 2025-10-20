#include "SCPIHandler.h"

// Constructor with Initialization List
SCPIHandler::SCPIHandler(SwitchManager &manager) : switchManager(manager) {}

char reply[64];
const char* reply2;

void SCPIHandler::getBinString(char* binStr, size_t binStrSize, uint16_t state, uint8_t bitWidth) 
{
    for (int i = bitWidth - 1; i >= 0; i--) 
    {
        binStr[bitWidth - 1 - i] = (state & (1 << i)) ? '1' : '0';
    }
    binStr[binStrSize - 1] = '\0'; // Null-terminate the string
}

void SCPIHandler::processCommand(const String &cmd, uint8_t sock) 
{
    // Reset Command: set all switches to Factory Settings except IP and Subnet
    if (cmd.startsWith("*RST")) 
    {
        Serial.println("Reset to Factory Settings (except IP and Netmask)");
        // Save IP and Subnet into the NVS-Storage
        wiz_NetInfo netInfo;
        wizchip_getnetinfo(&netInfo);

        IPAddress ip(netInfo.ip[0], netInfo.ip[1], netInfo.ip[2], netInfo.ip[3]);
        IPAddress subnet(netInfo.sn[0], netInfo.sn[1], netInfo.sn[2], netInfo.sn[3]);

        configManager.saveNetworkSettings(ip, subnet);

        delay(500);
        reply2 = "RESTARTING\r\n";
        Serial.println(reply2);
        wiznet_send(sock, (uint8_t*) reply, strlen(reply));
        ESP.restart();
    } 

    // Hardware Identification Command
    else if (cmd.startsWith("*IDN?")) 
    {
        Serial.println("Test, 123456, Firmware 0.0.1");
        reply2 = "Test, 123456, Firmware 0.0.1\r\n";
        wiznet_send(sock, (uint8_t*) reply, strlen(reply));
        return;
    }

    // set multiple SPDT Switches
    else if (cmd.startsWith("SPDTMULTI=")) 
    {
        uint64_t state;
        char id = 'A';
        String switchName = "SPDT";
        String switchNameID;
        uint16_t currentState;

        if (cmd.startsWith("0b", 10)) state = strtoull(cmd.substring(12).c_str(), NULL, 2);
        else state = cmd.substring(10).toInt();

        for (int i = 0; i < switchManager.getSwitchCount(switchName); i++)
        {
            switchNameID = switchName + String(id);
            currentState = switchManager.setSwitchState(switchNameID, state >> (i * 1) & 0x1);

            if (currentState == 0xFFFF) 
            {
                reply2 = "ERROR: Invalid state\r\n";

                Serial.println(reply2);
                wiznet_send(sock, (uint8_t*)reply2, strlen(reply2));
                return;
            }
            else 
            {
                char binStr[2]; // 1 bits + null terminator

                getBinString(binStr, sizeof(binStr), currentState, 1);
                snprintf(reply, sizeof(reply), "%s Switch state set to 0b%s\r\n",switchNameID , binStr);
                Serial.println(switchNameID + " Switch state set to " + String(currentState, BIN));
                wiznet_send(sock, (uint8_t*)reply, strlen(reply));
            }

            id++;
        }
    }

    // set multiple SP4T Switches
    else if (cmd.startsWith("SP4TMULTI=")) 
    {
        uint64_t state;
        char id = 'A';
        String switchName = "SP4T";
        String switchNameID;
        uint16_t currentState;

        if (cmd.startsWith("0b", 10)) state = strtoull(cmd.substring(12).c_str(), NULL, 2);
        else state = cmd.substring(10).toInt();
        
        for (int i = 0; i < switchManager.getSwitchCount(switchName); i++)
        {
            switchNameID = switchName + String(id);
            currentState = switchManager.setSwitchState(switchNameID, state >> (i * 4) & 0xF);

            if (currentState == 0xFFFF) 
            {
                reply2 = "ERROR: Invalid state\r\n";

                Serial.println(reply2);
                wiznet_send(sock, (uint8_t*)reply2, strlen(reply2));
                return;
            }
            else 
            {
                char binStr[5]; // 4 bits + null terminator

                getBinString(binStr, sizeof(binStr), currentState, 4);
                snprintf(reply, sizeof(reply), "%s Switch state set to 0b%s\r\n",switchNameID , binStr);
                Serial.println(switchNameID + " Switch state set to " + String(currentState, BIN));
                wiznet_send(sock, (uint8_t*)reply, strlen(reply));
            }

            id++;
        }
    }

    // set multiple SP6T Switches
    else if (cmd.startsWith("SP6TMULTI=")) 
    {
        uint64_t state;
        char id = 'A';
        String switchName = "SP6T";
        String switchNameID;
        uint16_t currentState;

        if (cmd.startsWith("0b", 10)) state = strtoull(cmd.substring(12).c_str(), NULL, 2);
        else state = cmd.substring(10).toInt();
        
        for (int i = 0; i < switchManager.getSwitchCount(switchName); i++)
        {
            switchNameID = switchName + String(id);
            currentState = switchManager.setSwitchState(switchNameID, state >> (i * 6) & 0x3F);

            if (currentState == 0xFFFF) 
            {
                reply2 = "ERROR: Invalid state\r\n";

                Serial.println(reply2);
                wiznet_send(sock, (uint8_t*)reply2, strlen(reply2));
                return;
            }
            else 
            {
                char binStr[7]; // 6 bits + null terminator

                getBinString(binStr, sizeof(binStr), currentState, 6);
                snprintf(reply, sizeof(reply), "%s Switch state set to 0b%s\r\n",switchNameID , binStr);
                Serial.println(switchNameID + " Switch state set to " + String(currentState, BIN));
                wiznet_send(sock, (uint8_t*)reply, strlen(reply));
            }

            id++;
        }
    }

    // set multiple SP12T Switches
    else if (cmd.startsWith("SP12TMULTI=")) 
    {
        uint64_t state;
        char id = 'A';
        String switchName = "SP12T";
        String switchNameID;
        uint16_t currentState;

        if (cmd.startsWith("0b", 11)) state = strtoull(cmd.substring(13).c_str(), NULL, 2);
        else state = cmd.substring(11).toInt();
        
        for (int i = 0; i < switchManager.getSwitchCount(switchName); i++)
        {
            switchNameID = switchName + String(id);
            currentState = switchManager.setSwitchState(switchNameID, state >> (i * 12) & 0xFFF);

            if (currentState == 0xFFFF) 
            {
                reply2 = "ERROR: Invalid state\r\n";

                Serial.println(reply2);
                wiznet_send(sock, (uint8_t*)reply2, strlen(reply2));
                return;
            }
            else 
            {
                char binStr[13]; // 12 bits + null terminator

                getBinString(binStr, sizeof(binStr), currentState, 12);
                snprintf(reply, sizeof(reply), "%s Switch state set to 0b%s\r\n",switchNameID , binStr);
                Serial.println(switchNameID + " Switch state set to " + String(currentState, BIN));
                wiznet_send(sock, (uint8_t*)reply, strlen(reply));
            }

            id++;
        }
    }

    // Single SPDT Switch Command
    else if (cmd.startsWith("SPDT")) 
    {
        char switchID = cmd.charAt(4);
        String switchNameID = "SPDT" + String(switchID);

        if (!switchManager.hasSwitch(switchNameID)) 
        {
            reply2 = "ERROR: Switch not found\r\n";
            Serial.println(reply2);
            wiznet_send(sock, (uint8_t*)reply2, strlen(reply2));
            return;
        }
        // get state
        if (cmd.startsWith(":STATE?", 5))
        {
            uint16_t currentState = switchManager.getSwitchState(switchNameID);
            char binStr[1]; // 1 bits + null terminator

            getBinString(binStr, sizeof(binStr), currentState, 1);
            snprintf(reply, sizeof(reply), "%s:STATE:0b%s\r\n",switchNameID , binStr);
            Serial.println(switchNameID + ":STATE:" + String(currentState, BIN));
            wiznet_send(sock, (uint8_t*)reply, strlen(reply));
        }
        // Set state of a SPDT Switch
        else if (cmd.startsWith(":STATE:", 5))
        {
            uint16_t state;
            uint16_t currentState;

            if (cmd.startsWith("0b", 12)) state = strtoull(cmd.substring(14).c_str(), NULL, 2);
            else state = cmd.substring(12).toInt();

            currentState = switchManager.setSwitchState(switchNameID, state & 0x1);
            
            if (currentState == 0xFFFF) 
            {
                reply2 = "ERROR: Invalid state\r\n";

                Serial.println(reply2);
                wiznet_send(sock, (uint8_t*)reply2, strlen(reply2));
                return;
            }
            else 
            {
                char binStr[2]; // 1 bits + null terminator

                getBinString(binStr, sizeof(binStr), currentState, 1);
                snprintf(reply, sizeof(reply), "%s Switch state set to 0b%s\r\n",switchNameID , binStr);
                Serial.println(switchNameID + " Switch state set to " + String(currentState, BIN));
                wiznet_send(sock, (uint8_t*)reply, strlen(reply));
            }
        }

        else
        {
            reply2 = "ERROR: SPDT Command not found\r\n";

            Serial.println(reply2);
            wiznet_send(sock, (uint8_t*)reply2, strlen(reply2));
            return;
        }    
    } 

    // Single SP4T Switch Command
    else if (cmd.startsWith("SP4T")) 
    {
        char switchID = cmd.charAt(4);
        String switchNameID = "SP4T" + String(switchID);

        if (!switchManager.hasSwitch(switchNameID)) 
        {
            reply2 = "ERROR: Switch not found\r\n";

            Serial.println(reply2);
            wiznet_send(sock, (uint8_t*)reply2, strlen(reply2));
            return;
        }
        // get state
        if (cmd.startsWith(":STATE?", 5))
        {
            uint16_t currentState = switchManager.getSwitchState(switchNameID);
            char binStr[5]; // 4 bits + null terminator

            getBinString(binStr, sizeof(binStr), currentState, 4);
            snprintf(reply, sizeof(reply), "%s:STATE:0b%s\r\n",switchNameID , binStr);
            Serial.println(switchNameID + ":STATE:" + String(currentState, BIN));
            wiznet_send(sock, (uint8_t*)reply, strlen(reply));
        }
        // Set state of a SP4T Switch
        else if (cmd.startsWith(":STATE:", 5))
        {
            uint16_t state;
            uint16_t currentState;

            if (cmd.startsWith("0b", 12)) state = strtoull(cmd.substring(14).c_str(), NULL, 2);
            else state = cmd.substring(12).toInt();

            currentState = switchManager.setSwitchState(switchNameID, state & 0xF);
            if (currentState == 0xFFFF) 
            {
                reply2 = "ERROR: Invalid state\r\n";

                Serial.println(reply2);
                wiznet_send(sock, (uint8_t*)reply2, strlen(reply2));
                return;
            }
            else 
            {
                char binStr[5]; // 4 bits + null terminator

                getBinString(binStr, sizeof(binStr), currentState, 4);
                snprintf(reply, sizeof(reply), "%s Switch state set to 0b%s\r\n",switchNameID , binStr);
                Serial.println(switchNameID + " Switch state set to " + String(currentState, BIN));
                wiznet_send(sock, (uint8_t*)reply, strlen(reply));
            }
        }

        else 
        {
            reply2 = "ERROR: SP4T Command not found\r\n";

            Serial.println(reply2);
            wiznet_send(sock, (uint8_t*)reply2, strlen(reply2));
            return;
        }
    } 
    
    // Single SP6T Switch Command
    else if (cmd.startsWith("SP6T")) 
    {
        char switchID = cmd.charAt(4);
        String switchNameID = "SP6T" + String(switchID);

        if (!switchManager.hasSwitch(switchNameID)) 
        {
            reply2 = "ERROR: Switch not found\r\n";

            Serial.println(reply2);
            wiznet_send(sock, (uint8_t*)reply2, strlen(reply2));
            return;
        }
        // get state
        if (cmd.startsWith(":STATE?", 5))
        {
            uint16_t currentState = switchManager.getSwitchState(switchNameID);
            char binStr[7]; // 6 bits + null terminator

            getBinString(binStr, sizeof(binStr), currentState, 6);
            snprintf(reply, sizeof(reply), "%s:STATE:0b%s\r\n",switchNameID , binStr);
            Serial.println(switchNameID + ":STATE:" + String(currentState, BIN));
            wiznet_send(sock, (uint8_t*)reply, strlen(reply));
        }
        // Set state of a SP6T Switch
        else if (cmd.startsWith(":STATE:", 5))
        {
            uint16_t state;
            uint16_t currentState;

            if (cmd.startsWith("0b", 12)) state = strtoull(cmd.substring(14).c_str(), NULL, 2);
            else state = cmd.substring(12).toInt();
            
            currentState = switchManager.setSwitchState(switchNameID, state & 0x3F);

            if (currentState == 0xFFFF) 
            {
                reply2 = "ERROR: Invalid state\r\n";

                Serial.println(reply2);
                wiznet_send(sock, (uint8_t*)reply2, strlen(reply2));
                return;
            }
            else 
            {
                char binStr[7]; // 6 bits + null terminator

                getBinString(binStr, sizeof(binStr), currentState, 6);
                snprintf(reply, sizeof(reply), "%s Switch state set to 0b%s\r\n",switchNameID , binStr);
                Serial.println(switchNameID + " Switch state set to " + String(currentState, BIN));
                wiznet_send(sock, (uint8_t*)reply, strlen(reply));
            }
        }

        else 
        {
            reply2 = "ERROR: SP6T Command not found\r\n";

            Serial.println(reply2);
            wiznet_send(sock, (uint8_t*)reply2, strlen(reply2));
            return;
        }
    } 

    // Single SP12T Switch Command
    else if (cmd.startsWith("SP12T")) 
    {
        char switchID = cmd.charAt(5);
        String switchNameID = "SP12T" + String(switchID);

        if (!switchManager.hasSwitch(switchNameID)) 
        {
            reply2 = "ERROR: Switch not found\r\n";

            Serial.println(reply2);
            wiznet_send(sock, (uint8_t*)reply2, strlen(reply2));
            return;
        }
        // get state
        if (cmd.startsWith(":STATE?", 6))
        {
            uint16_t currentState = switchManager.getSwitchState(switchNameID);
            char binStr[13]; // 12 bits + null terminator

            getBinString(binStr, sizeof(binStr), currentState, 12);
            snprintf(reply, sizeof(reply), "%s:STATE:0b%s\r\n",switchNameID , binStr);
            Serial.println(switchNameID + ":STATE:" + String(currentState, BIN));
            wiznet_send(sock, (uint8_t*)reply, strlen(reply));
        }
        // Set state of a SP12T Switch
        else if (cmd.startsWith(":STATE:", 6))
        {
            uint16_t state;
            uint16_t currentState;

            if (cmd.startsWith("0b", 13)) state = strtoull(cmd.substring(15).c_str(), NULL, 2);
            else state = cmd.substring(13).toInt();
            
            currentState = switchManager.setSwitchState(switchNameID, state & 0xFFF);

            if (currentState == 0xFFFF) 
            {
                reply2 = "ERROR: Invalid state\r\n";

                Serial.println(reply2);
                wiznet_send(sock, (uint8_t*)reply2, strlen(reply2));
                return;
            }
            else 
            {
                char binStr[13]; // 12 bits + null terminator

                getBinString(binStr, sizeof(binStr), currentState, 12);
                snprintf(reply, sizeof(reply), "%s Switch state set to 0b%s\r\n",switchNameID , binStr);
                Serial.println(switchNameID + " Switch state set to " + String(currentState, BIN));
                wiznet_send(sock, (uint8_t*)reply, strlen(reply));
            }
        }
        else 
        {
            reply2 = "ERROR: SP12T Command not found\r\n";

            Serial.println(reply2);
            wiznet_send(sock, (uint8_t*)reply2, strlen(reply2));
        }
    }    
    else 
    {
        reply2 = "ERROR: Command not found\r\n";

        Serial.println(reply2);
        wiznet_send(sock, (uint8_t*)reply2, strlen(reply2));
    }
}