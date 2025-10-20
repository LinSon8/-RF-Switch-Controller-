#pragma once

#include <Arduino.h>

extern const int TCP_PORT;
extern const int SOCKET_NUM;
//extern const int MAX_SOCKETS;
#define MAX_SOCKETS 8
extern uint8_t socketBuffer[2048];

extern uint8_t mac[6];
extern IPAddress standardIP;
extern IPAddress standardSubnet;

struct ConnTimer {
    uint32_t lastActivityms = 0;
    bool locked = false;
};