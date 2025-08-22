#pragma once

#include <SwitchController.h>
#include <SPDTSwitch.h>
#include <SP4TSwitch.h>
#include <SP6TSwitch.h>
#include <SP12TSwitch.h>
#include <map>

class SwitchManager
{
    private:
        std::map<String, SwitchController*> switchMap;
        std::map<String, int> switchCountMap;

    public:
        SwitchManager();
        ~SwitchManager();
        void addSwitch(const String &switchName, SwitchController *newSwitch);
        void removeSwitch(const String &switchName);
        void setSwitchState(const String &switchName, uint16_t bitmask);
        int getSwitchState(const String &switchName);  
        int getSwitchCount(const String &switchName);
        boolean hasSwitch(const String &switchName);
        void resetSwitch(const String &switchName);
        void resetAllSwitches();
};