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

void serializar_registro(uint32_t socket, registro_tad* registro){

}

registro_tad* deserializar_registro(uint32_t socket){

}

void serializar_insert(uint32_t socket, insert_tad* insert) {
    uint32_t datos_size = sizeof(insert_tad) + (strlen(insert->nameTable) + 1) + (strlen(insert->value) + 1);
    void* ENVIAR = malloc(datos_size);
    uint32_t offset = 0;
    uint32_t size_to_send;

    size_to_send = strlen(insert->nameTable) + 1;
    memcpy(ENVIAR + offset, insert->nameTable, size_to_send);
    offset += size_to_send;

    size_to_send = sizeof(insert->key);
    memcpy(ENVIAR + offset, &(insert->key),size_to_send);
    offset += size_to_send;

    size_to_send = strlen(insert->value) + 1;
    memcpy(ENVIAR + offset, insert->value, size_to_send);
    offset += size_to_send;

    serializar_int(socket, offset);
    send_data(socket, ENVIAR, offset);
    free(ENVIAR);
}

insert_tad* deserializar_insert(uint32_t socket) {
    uint32_t buffer_size = deserializar_int(socket);
    void* buffer = malloc(buffer_size);
    insert_tad* insert = malloc(sizeof(insert_tad));
    uint32_t offset = 0;
    uint32_t size_to_recive;

    recive_data(socket, buffer, buffer_size);

    insert->nameTable = strdup(buffer + offset);
    offset += strlen(insert->nameTable) + 1;

    size_to_recive = sizeof(insert->key);
    memcpy(&insert->key, buffer + offset, size_to_recive);
    offset += size_to_recive;

    insert->value = strdup(buffer + offset);
    offset += strlen(insert->value) + 1;

    free(buffer);
    return insert;
}

void serializar_select(uint32_t socket, select_tad* select) {
    uint32_t datos_size = sizeof(select_tad) + (strlen(select->nameTable) + 1);
    void* ENVIAR = malloc(datos_size);
    uint32_t offset = 0;
    uint32_t size_to_send;

    size_to_send = strlen(select->nameTable) + 1;
    memcpy(ENVIAR + offset, select->nameTable, size_to_send);
    offset += size_to_send;

    size_to_send = sizeof(select->key);
    memcpy(ENVIAR + offset, &(select->key),size_to_send);
    offset += size_to_send;

    serializar_int(socket, offset);
    send_data(socket, ENVIAR, offset);
    free(ENVIAR);
}

select_tad* deserializar_select(uint32_t socket) {
    uint32_t buffer_size = deserializar_int(socket);
    void* buffer = malloc(buffer_size);
    select_tad* select = malloc(sizeof(select_tad));
    uint32_t offset = 0;
    uint32_t size_to_recive;

    recive_data(socket, buffer, buffer_size);

    select->nameTable = strdup(buffer + offset);
    offset += strlen(select->nameTable) + 1;

    size_to_recive = sizeof(select->key);
    memcpy(&select->key, buffer + offset, size_to_recive);
    offset += size_to_recive;

    free(buffer);
    return select;
}

void serializar_create(uint32_t socket, create_tad* create) {
    uint32_t datos_size = sizeof(create_tad) + (strlen(create->nameTable) + 1) + (strlen(create->consistencia) + 1);
    void* ENVIAR = malloc(datos_size);
    uint32_t offset = 0;
    uint32_t size_to_send;

    size_to_send = strlen(create->nameTable) + 1;
    memcpy(ENVIAR + offset, create->nameTable, size_to_send);
    offset += size_to_send;

    size_to_send = strlen(create->consistencia) + 1;
    memcpy(ENVIAR + offset, create->consistencia, size_to_send);
    offset += size_to_send;

    size_to_send = sizeof(create->compactacion);
    memcpy(ENVIAR + offset, &(create->compactacion),size_to_send);
    offset += size_to_send;

    size_to_send = sizeof(create->particiones);
    memcpy(ENVIAR + offset, &(create->particiones),size_to_send);
    offset += size_to_send;

    serializar_int(socket, offset);
    send_data(socket, ENVIAR, offset);
    free(ENVIAR);
}

create_tad* deserializar_create(uint32_t socket) {
    uint32_t buffer_size = deserializar_int(socket);
    void* buffer = malloc(buffer_size);
    create_tad* create = malloc(sizeof(create_tad));
    uint32_t offset = 0;
    uint32_t size_to_recive;

    recive_data(socket, buffer, buffer_size);

    create->nameTable = strdup(buffer + offset);
    offset += strlen(create->nameTable) + 1;

    create->consistencia = strdup(buffer + offset);
    offset += strlen(create->consistencia) + 1;

    size_to_recive = sizeof(create->compactacion);
    memcpy(&create->compactacion, buffer + offset, size_to_recive);
    offset += size_to_recive;

    size_to_recive = sizeof(create->particiones);
    memcpy(&create->particiones, buffer + offset, size_to_recive);
    offset += size_to_recive;

    free(buffer);
    return create;
}