#ifndef SERVIDOR_H_
#define SERVIDOR_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

uint32_t connect_server(char* IP , uint32_t Port);
uint32_t build_server(uint32_t Port, uint32_t quantityConexions);
uint32_t accept_conexion(uint32_t servidor);
uint32_t recive_data(uint32_t cliente, void* buf, uint32_t bytesToRecive);

void set_listen(uint32_t servidor, uint32_t sizeConexiones);
void send_data(uint32_t servidor, void* dataToSend, uint32_t bytesToSend);
void send_string(uint32_t socket, char* mensaje);
void massive_send(uint32_t fdmax, fd_set *master, void* buffer, uint32_t cantToSend, uint32_t servidor);


#endif
