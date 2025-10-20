#include "Config.h"

const int TCP_PORT = 5025; // SCPI TCP Port
const int SOCKET_NUM = 0; // Socket number for SCPI TCP connection
//const int MAX_SOCKETS = 8; // Maximum number of sockets for W5500
uint8_t socketBuffer[2048]; // Buffer for socket data

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress standardIP = {10, 0, 0, 220}; 
IPAddress standardSubnet = {255, 255, 255, 0}; 