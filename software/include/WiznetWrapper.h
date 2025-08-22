#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h> 

int8_t wiznet_close(uint8_t sn);
int8_t wiznet_socket(uint8_t sn, uint8_t protocol, uint16_t port, uint8_t flag);
int8_t wiznet_listen(uint8_t sn);
int8_t wiznet_recv(uint8_t sn, uint8_t * buf, uint16_t len);

#ifdef __cplusplus
}
#endif
