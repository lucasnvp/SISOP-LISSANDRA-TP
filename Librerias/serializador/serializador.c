#include "serializador.h"

t_stream* stream_create(int size) {
	t_stream* stream = malloc(sizeof(t_stream));
	stream->size = size;
	stream->data = malloc(size);
	return stream;
}

void stream_destroy(t_stream* stream) {
	free(stream->data);
	free(stream);
}

void serializar_int(uint32_t socket, uint32_t number){
	send_data(socket, &number, sizeof(uint32_t));
}

uint32_t deserializar_int(uint32_t socket){
	uint32_t aux;
	uint32_t bytesRecibidos = recive_data(socket,&aux,sizeof(uint32_t));
	if(bytesRecibidos <= 0){
		aux = bytesRecibidos;
	}
	return aux;
}

void serializar_string(int client, char* stringToSend){
	void* ENVIAR = malloc(strlen(stringToSend) + 1);
	uint32_t offset = 0;
	uint32_t size_to_send;

	size_to_send = strlen(stringToSend) + 1;
	memcpy(ENVIAR + offset, stringToSend, size_to_send);
	offset += size_to_send;

	serializar_int(client, offset);
	send_data(client, ENVIAR, offset);
	free(ENVIAR);
}

char* deserializar_string(int servidor){
	uint32_t buffer_size = deserializar_int(servidor);
	void* buffer = malloc(buffer_size);
	recive_data(servidor, buffer, buffer_size);
	char* stringToRecived = strdup(buffer);
	free(buffer);
	return stringToRecived;
}

void serializar_esi(uint32_t socket, t_esi* esi){
	uint32_t datos_size = sizeof(t_esi) +
			(strlen(esi->nombre) + 1) +
			(strlen(esi->comando) + 1) +
			(strlen(esi->clave) + 1) +
			(strlen(esi->valor) + 1);
	void* ENVIAR = malloc(datos_size);
	uint32_t offset = 0;
	uint32_t size_to_send;

	size_to_send = strlen(esi->nombre) + 1;
	memcpy(ENVIAR + offset, esi->nombre, size_to_send);
	offset += size_to_send;

	size_to_send = strlen(esi->comando) + 1;
	memcpy(ENVIAR + offset, esi->comando, size_to_send);
	offset += size_to_send;

	size_to_send = strlen(esi->clave) + 1;
	memcpy(ENVIAR + offset, esi->clave, size_to_send);
	offset += size_to_send;

	size_to_send = strlen(esi->valor) + 1;
	memcpy(ENVIAR + offset, esi->valor, size_to_send);
	offset += size_to_send;

	serializar_int(socket, offset);
	send_data(socket, ENVIAR, offset);
	free(ENVIAR);
}

t_esi* deserializar_esi(uint32_t socket){
	uint32_t buffer_size = deserializar_int(socket);
	void* buffer = malloc(buffer_size);
	t_esi* esi = malloc(sizeof(t_esi));
	uint32_t offset = 0;
	uint32_t size_to_recive;

	recive_data(socket, buffer, buffer_size);

	esi->nombre = strdup(buffer + offset);
	offset += strlen(esi->nombre) + 1;

	esi->comando = strdup(buffer + offset);
	offset += strlen(esi->comando) + 1;

	esi->clave = strdup(buffer + offset);
	offset += strlen(esi->clave) + 1;

	esi->valor = strdup(buffer + offset);
	offset += strlen(esi->valor) + 1;

	free(buffer);
	return esi;
}

