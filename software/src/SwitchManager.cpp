#include <SwitchManager.h>

// initialize the switchCountMap with the different types of switches
SwitchManager::SwitchManager() 
{
    switchCountMap["SPDT"] = 0;
    switchCountMap["SP4T"] = 0;
    switchCountMap["SP6T"] = 0;
    switchCountMap["SP12T"] = 0;
}

// delete all switches
SwitchManager::~SwitchManager() 
{
    for (auto it = switchMap.begin(); it != switchMap.end(); it++)
    {
        delete it->second;
    }
    switchMap.clear();
}
 // add a new switch to the switchMap and increment the switchCountMap
void SwitchManager::addSwitch(const String &switchName, SwitchController *newSwitch)
{
    switchMap[switchName] = newSwitch;

    if (switchName.startsWith("SPDT"))
    {
        switchCountMap["SPDT"]++;
    }

    else if (switchName.startsWith("SP4T"))
    {
        switchCountMap["SP4T"]++;
    }

    else if (switchName.startsWith("SP6T"))
    {
        switchCountMap["SP6T"]++;
    }

    else if (switchName.startsWith("SP12T"))
    {
        switchCountMap["SP12T"]++;
    }
}
 // remove a switch from the switchMap and decrement the switchCountMap
void SwitchManager::removeSwitch(const String &switchName)
{
    delete switchMap[switchName];
    switchMap.erase(switchName);

    if (switchName.startsWith("SPDT"))
    {
        switchCountMap["SPDT"]--;
    }

    else if (switchName.startsWith("SP4T"))
    {
        switchCountMap["SP4T"]--;
    }

    else if (switchName.startsWith("SP6T"))
    {
        switchCountMap["SP6T"]--;
    }

    else if (switchName.startsWith("SP12T"))
    {
        switchCountMap["SP12T"]--;
    }
}

// set the state of a switch
void SwitchManager::setSwitchState(const String &switchName, uint16_t bitmask)
{
    switchMap[switchName]->setState(bitmask);
}

// get the state of a switch
int SwitchManager::getSwitchState(const String &switchName)
{
    return switchMap[switchName]->getState();
}

// get the count of a switch
int SwitchManager::getSwitchCount(const String &switchName)
{
    return switchCountMap[switchName];
}

// check if a switch exists
boolean SwitchManager::hasSwitch(const String &switchName)
{
    return switchMap.find(switchName) != switchMap.end();
}

// reset a switch
void SwitchManager::resetSwitch(const String &switchName)
{
    switchMap[switchName]->reset();
}

// reset all switches
void SwitchManager::resetAllSwitches()
{
    for (auto it = switchMap.begin(); it != switchMap.end(); it++)
    {
        it->second->reset();
    }
}