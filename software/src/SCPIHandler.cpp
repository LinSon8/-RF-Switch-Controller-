#include "SCPIHandler.h"

// Constructor with Initialization List
SCPIHandler::SCPIHandler(SwitchManager &manager) : switchManager(manager) {}

void SCPIHandler::processCommand(const String &cmd) 
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
        Serial.println("Restarting...");
        ESP.restart();
    } 

    // Hardware Identification Command
    else if (cmd.startsWith("*IDN?")) 
    {
        Serial.println("Test, 123456, Firmware 0.0.1");
    }

    // set multiple SPDT Switches
    else if (cmd.startsWith("SPDTMULTI=")) 
    {
        uint64_t state;
        char id = 'A';
        String switchName = "SPDT";
        String switchNameID;

        if (cmd.startsWith("0b", 10)) state = strtoull(cmd.substring(12).c_str(), NULL, 2);
        else state = cmd.substring(10).toInt();

        for (int i = 0; i < switchManager.getSwitchCount(switchName); i++)
        {
            switchNameID = switchName + String(id);
            switchManager.setSwitchState(switchNameID, state >> (i * 1) & 0x1);
            id++;
        }
        
        Serial.println("SPDT Switches set");
    }

    // set multiple SP4T Switches
    else if (cmd.startsWith("SP4TMULTI=")) 
    {
        uint64_t state;
        char id = 'A';
        String switchName = "SP4T";
        String switchNameID;

        if (cmd.startsWith("0b", 10)) state = strtoull(cmd.substring(12).c_str(), NULL, 2);
        else state = cmd.substring(10).toInt();
        
        for (int i = 0; i < switchManager.getSwitchCount(switchName); i++)
        {
            switchNameID = switchName + String(id);
            switchManager.setSwitchState(switchNameID, state >> (i * 4) & 0xF);
            id++;
        }
        Serial.println("SP4T Switches set");
    }

    // set multiple SP6T Switches
    else if (cmd.startsWith("SP6TMULTI=")) 
    {
        uint64_t state;
        char id = 'A';
        String switchName = "SP6T";
        String switchNameID;

        if (cmd.startsWith("0b", 10)) state = strtoull(cmd.substring(12).c_str(), NULL, 2);
        else state = cmd.substring(10).toInt();
        
        for (int i = 0; i < switchManager.getSwitchCount(switchName); i++)
        {
            switchNameID = switchName + String(id);
            switchManager.setSwitchState(switchNameID, state >> (i * 6) & 0x3F);
            id++;
        }
        Serial.println("SP6T Switches set");
    }

    // set multiple SP12T Switches
    else if (cmd.startsWith("SP12TMULTI=")) 
    {
        uint64_t state;
        char id = 'A';
        String switchName = "SP12T";
        String switchNameID;

        if (cmd.startsWith("0b", 11)) state = strtoull(cmd.substring(13).c_str(), NULL, 2);
        else state = cmd.substring(11).toInt();
        
        for (int i = 0; i < switchManager.getSwitchCount(switchName); i++)
        {
            switchNameID = switchName + String(id);
            switchManager.setSwitchState(switchNameID, state >> (i * 12) & 0xFFF);
            id++;
        }
        Serial.println("SP12T Switches set");
    }

    // Single SPDT Switch Command
    else if (cmd.startsWith("SPDT")) 
    {
        char switchID = cmd.charAt(4);
        String switchNameID = "SPDT" + String(switchID);

        if (!switchManager.hasSwitch(switchNameID)) 
        {
            Serial.println("ERROR: Switch not found");
            return;
        }
        // Zustand abfragen
        if (cmd.startsWith(":STATE?", 5))
        {
            Serial.print(switchNameID + ":STATE: ");
            Serial.println(switchManager.getSwitchState(switchNameID));
        }
        // Set state of a SPDT Switch
        else if (cmd.startsWith(":STATE:", 5))
        {
            uint16_t state;

            if (cmd.startsWith("0b", 12)) state = strtoull(cmd.substring(14).c_str(), NULL, 2);
            else state = cmd.substring(12).toInt();

            switchManager.setSwitchState(switchNameID, state & 0x1);
            Serial.println(switchNameID + " Switch state set");
        }

        else Serial.println("ERROR: SPDT Command not found");
    } 

    // Single SP4T Switch Command
    else if (cmd.startsWith("SP4T")) 
    {
        char switchID = cmd.charAt(4);
        String switchNameID = "SP4T" + String(switchID);

        if (!switchManager.hasSwitch(switchNameID)) 
        {
            Serial.println("ERROR: Switch not found");
            return;
        }
        // Zustand abfragen
        if (cmd.startsWith(":STATE?", 5))
        {
            Serial.print(switchNameID + ":STATE: ");
            Serial.println(switchManager.getSwitchState(switchNameID));
        }
        // Set state of a SP4T Switch
        else if (cmd.startsWith(":STATE:", 5))
        {
            uint16_t state;

            if (cmd.startsWith("0b", 12)) state = strtoull(cmd.substring(14).c_str(), NULL, 2);
            else state = cmd.substring(12).toInt();

            switchManager.setSwitchState(switchNameID, state & 0xF);
            Serial.println(switchNameID + " Switch state set");
        }

        else Serial.println("ERROR: SP4T Command not found");
    } 
    
    // Single SP6T Switch Command
    else if (cmd.startsWith("SP6T")) 
    {
        char switchID = cmd.charAt(4);
        String switchNameID = "SP6T" + String(switchID);

        if (!switchManager.hasSwitch(switchNameID)) 
        {
            Serial.println("ERROR: Switch not found");
            return;
        }
        // Zustand abfragen
        if (cmd.startsWith(":STATE?", 5))
        {
            Serial.print(switchNameID + ":STATE: ");
            Serial.println(switchManager.getSwitchState(switchNameID));
        }
        // Set state of a SP6T Switch
        else if (cmd.startsWith(":STATE:", 5))
        {
            uint16_t state;

            if (cmd.startsWith("0b", 12)) state = strtoull(cmd.substring(14).c_str(), NULL, 2);
            else state = cmd.substring(12).toInt();
            
            switchManager.setSwitchState(switchNameID, state & 0x3F);
            Serial.println(switchNameID + " Switch state set");
        }

        else Serial.println("ERROR: SP6T Command not found");
    } 

    // Single SP12T Switch Command
    else if (cmd.startsWith("SP12T")) 
    {
        char switchID = cmd.charAt(5);
        String switchNameID = "SP12T" + String(switchID);

        if (!switchManager.hasSwitch(switchNameID)) 
        {
            Serial.println("ERROR: Switch not found");
            return;
        }
        // Zustand abfragen
        if (cmd.startsWith(":STATE?", 6))
        {
            Serial.print(switchNameID + ":STATE: ");
            Serial.println(switchManager.getSwitchState(switchNameID));
        }
        // Set state of a SP12T Switch
        else if (cmd.startsWith(":STATE:", 6))
        {
            uint16_t state;

            if (cmd.startsWith("0b", 13)) state = strtoull(cmd.substring(15).c_str(), NULL, 2);
            else state = cmd.substring(13).toInt();
            
            switchManager.setSwitchState(switchNameID, state & 0xFFF);
            Serial.println(switchNameID + " Switch state set");
        }

        else Serial.println("ERROR: SP12T Command not found");
    }    
    else Serial.println("ERROR: Command not found");
}