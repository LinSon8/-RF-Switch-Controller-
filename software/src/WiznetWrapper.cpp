#include "WiznetWrapper.h"
/*
    * WiznetWrapper.cpp avoids collisions between C and C++ functions of the W5500 library socket.h
    * The "::" of "::close()",  forces the compiler to use the C-Functions
*/

//#define close wiznet_close
//#define listen wiznet_listen
//#define socket wiznet_socket
//#define recv wiznet_recv

extern "C" {
    #include "Ethernet/socket.h"
}
 
// Wrapper functions for close()
int8_t wiznet_close(uint8_t sn) 
{
    return ::close(sn);
}
// Wrapper functions for socket()
int8_t wiznet_socket(uint8_t sn, uint8_t protocol, uint16_t port, uint8_t flag) 
{
    return ::socket(sn, protocol, port, flag);
}
// Wrapper functions for listen()
int8_t wiznet_listen(uint8_t sn) 
{
    return ::listen(sn);
}
// Wrapper functions for recv()
int8_t wiznet_recv(uint8_t sn, uint8_t * buf, uint16_t len) 
{
    return ::recv(sn, buf, len);
}