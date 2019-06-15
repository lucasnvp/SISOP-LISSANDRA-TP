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
    uint32_t datos_size = sizeof(registro_tad) + (strlen(registro->value) + 1);
    void* ENVIAR = malloc(datos_size);
    uint32_t offset = 0;
    uint32_t size_to_send;

    size_to_send = sizeof(registro->timestamp);
    memcpy(ENVIAR + offset, &(registro->timestamp), size_to_send);
    offset += size_to_send;

    size_to_send = sizeof(registro->key);
    memcpy(ENVIAR + offset, &(registro->key), size_to_send);
    offset += size_to_send;

    size_to_send = strlen(registro->value) + 1;
    memcpy(ENVIAR + offset, registro->value, size_to_send);
    offset += size_to_send;

    serializar_int(socket, offset);
    send_data(socket, ENVIAR, offset);
    free(ENVIAR);
}

registro_tad* deserializar_registro(uint32_t socket){
    uint32_t buffer_size = deserializar_int(socket);
    void* buffer = malloc(buffer_size);
    registro_tad* registro = malloc(sizeof(registro_tad));
    uint32_t offset = 0;
    uint32_t size_to_recive;

    recive_data(socket, buffer, buffer_size);

    size_to_recive = sizeof(registro->timestamp);
    memcpy(&registro->timestamp, buffer + offset, size_to_recive);
    offset += size_to_recive;

    size_to_recive = sizeof(registro->key);
    memcpy(&registro->key, buffer + offset, size_to_recive);
    offset += size_to_recive;

    registro->value = strdup(buffer + offset);
    offset += strlen(registro->value) + 1;

    free(buffer);
    return registro;
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

t_stream* serializar_table(struct table_tad* table) {
    uint32_t datos_size = sizeof(struct table_tad) + (strlen(table->nameTable) + 1) + (strlen(table->consistencia) + 1);
    t_stream* ENVIAR = stream_create(datos_size);
    uint32_t offset = 0;
    uint32_t size_to_send;

    size_to_send = strlen(table->nameTable) + 1;
    memcpy(ENVIAR->data + offset, table->nameTable, size_to_send);
    offset += size_to_send;

    size_to_send = strlen(table->consistencia) + 1;
    memcpy(ENVIAR->data + offset, table->consistencia, size_to_send);
    offset += size_to_send;

    size_to_send = sizeof(table->compactacion);
    memcpy(ENVIAR->data + offset, &(table->compactacion),size_to_send);
    offset += size_to_send;

    size_to_send = sizeof(table->particiones);
    memcpy(ENVIAR->data + offset, &(table->particiones),size_to_send);
    offset += size_to_send;

    return ENVIAR;
}

struct table_tad* deserializar_table(char* stream, int* size) {
    struct table_tad* table = malloc(sizeof(struct table_tad));
    uint32_t offset = 0;
    uint32_t size_to_recive;

    table->nameTable = strdup(stream + offset);
    offset += strlen(table->nameTable) + 1;

    table->consistencia = strdup(stream + offset);
    offset += strlen(table->consistencia) + 1;

    size_to_recive = sizeof(table->compactacion);
    memcpy(&table->compactacion, stream + offset, size_to_recive);
    offset += size_to_recive;

    size_to_recive = sizeof(table->particiones);
    memcpy(&table->particiones, stream + offset, size_to_recive);
    offset += size_to_recive;

    *size = sizeof(struct table_tad) + (strlen(table->nameTable) + 1) + (strlen(table->consistencia) + 1);

    return table;
}

void serializar_create(uint32_t socket, create_tad* create) {
    t_stream* stream = serializar_table(create);
    serializar_int(socket, stream->size);
    send_data(socket, stream->data, stream->size);
    stream_destroy(stream);
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

void serializar_describe(uint32_t socket, describe_tad* describe) {
    t_stream* stream = serializar_table(describe);
    serializar_int(socket, stream->size);
    send_data(socket, stream->data, stream->size);
    stream_destroy(stream);
}

describe_tad* deserializar_describe(uint32_t socket) {
    uint32_t buffer_size = deserializar_int(socket);
    void* buffer = malloc(buffer_size);
    describe_tad* describe = malloc(sizeof(describe_tad));
    uint32_t offset = 0;
    uint32_t size_to_recive;

    recive_data(socket, buffer, buffer_size);

    describe->nameTable = strdup(buffer + offset);
    offset += strlen(describe->nameTable) + 1;

    describe->consistencia = strdup(buffer + offset);
    offset += strlen(describe->consistencia) + 1;

    size_to_recive = sizeof(describe->compactacion);
    memcpy(&describe->compactacion, buffer + offset, size_to_recive);
    offset += size_to_recive;

    size_to_recive = sizeof(describe->particiones);
    memcpy(&describe->particiones, buffer + offset, size_to_recive);
    offset += size_to_recive;

    free(buffer);
    return describe;
}

void serializar_describe_all(uint32_t socket, t_list* describe_all) {
    uint32_t datos_size = sizeof(uint32_t);
    t_stream* ENVIAR = stream_create(datos_size);
    uint32_t offset = 0;
    uint32_t size_to_send;

    void serialize_element_stack(void* element){
        describe_tad* lineStack = element;
        t_stream* stream_lineStack = serializar_table(lineStack);
        ENVIAR->data = realloc(ENVIAR->data, ENVIAR->size + stream_lineStack->size);
        memcpy(ENVIAR->data + offset, stream_lineStack->data, stream_lineStack->size);
        ENVIAR->size += stream_lineStack->size;
        offset += stream_lineStack->size;
        stream_destroy(stream_lineStack);
    }

    uint32_t count_line_stack = list_size(describe_all);
    size_to_send = sizeof(count_line_stack);
    memcpy(ENVIAR->data + offset, &(count_line_stack),size_to_send);
    offset += size_to_send;

    list_iterate(describe_all, serialize_element_stack);

    serializar_int(socket, ENVIAR->size);
    send_data(socket, ENVIAR->data, ENVIAR->size);
    free(ENVIAR);
}

t_list* deserializar_describe_all(uint32_t socket) {
    uint32_t buffer_size = deserializar_int(socket);
    void* buffer = malloc(buffer_size);
    t_list* describe_list = list_create();
    uint32_t tmp_size = 0;
    uint32_t offset = 0;
    uint32_t size_to_recive;

    recive_data(socket, buffer, buffer_size);

    uint32_t count_line_stack = 0;
    size_to_recive = sizeof(count_line_stack);
    memcpy(&count_line_stack, buffer + offset, size_to_recive);
    offset += size_to_recive;

    for(i = 0; i < count_line_stack; ++i){
        describe_tad* lineStack = deserializar_table(buffer + offset, &tmp_size);
        offset += tmp_size;
        list_add(describe_list, lineStack);
    }

    free(buffer);
    return describe_list;
}

void serializar_memory_info(uint32_t socket, memory_info_tad* memory) {
    uint32_t datos_size = sizeof(memory_info_tad);
    void* ENVIAR = malloc(datos_size);
    uint32_t offset = 0;
    uint32_t size_to_send;

    size_to_send = sizeof(memory->MEMORY_NUMBER);
    memcpy(ENVIAR + offset, &(memory->MEMORY_NUMBER), size_to_send);
    offset += size_to_send;

    size_to_send = sizeof(memory->RETARDO_GOSSIPING);
    memcpy(ENVIAR + offset, &(memory->RETARDO_GOSSIPING), size_to_send);
    offset += size_to_send;

    serializar_int(socket, offset);
    send_data(socket, ENVIAR, offset);
    free(ENVIAR);
}

memory_info_tad* deserializar_memory_info(uint32_t socket) {
    uint32_t buffer_size = deserializar_int(socket);
    void* buffer = malloc(buffer_size);
    memory_info_tad* memory = malloc(sizeof(memory_info_tad));
    uint32_t offset = 0;
    uint32_t size_to_recive;

    recive_data(socket, buffer, buffer_size);

    size_to_recive = sizeof(memory->MEMORY_NUMBER);
    memcpy(&memory->MEMORY_NUMBER, buffer + offset, size_to_recive);
    offset += size_to_recive;

    size_to_recive = sizeof(memory->RETARDO_GOSSIPING);
    memcpy(&memory->RETARDO_GOSSIPING, buffer + offset, size_to_recive);
    offset += size_to_recive;

    free(buffer);
    return memory;
}

t_stream* serializar_gossip(gossip_tad* gossip) {
    uint32_t datos_size = sizeof(gossip_tad) + (strlen(gossip->IP) + 1);
    t_stream* ENVIAR = stream_create(datos_size);
    uint32_t offset = 0;
    uint32_t size_to_send;

    size_to_send = strlen(gossip->IP) + 1;
    memcpy(ENVIAR->data + offset, gossip->IP, size_to_send);
    offset += size_to_send;

    size_to_send = sizeof(gossip->PORT);
    memcpy(ENVIAR->data + offset, &(gossip->PORT),size_to_send);
    offset += size_to_send;

    return ENVIAR;
}

gossip_tad* deserializar_gossip(char* stream, int* size) {
    gossip_tad* gossip = malloc(sizeof(gossip_tad));
    uint32_t offset = 0;
    uint32_t size_to_recive;

    gossip->IP = strdup(stream + offset);
    offset += strlen(gossip->IP) + 1;

    size_to_recive = sizeof(gossip->PORT);
    memcpy(&gossip->PORT, stream + offset, size_to_recive);
    offset += size_to_recive;

    *size = sizeof(gossip_tad) + (strlen(gossip->IP) + 1);

    return gossip;
}

void serializar_gossip_table(uint32_t socket, t_list* gossip) {
    uint32_t datos_size = sizeof(uint32_t);
    t_stream* ENVIAR = stream_create(datos_size);
    uint32_t offset = 0;
    uint32_t size_to_send;

    void serialize_element_stack(void* element){
        gossip_tad* lineStack = element;
        t_stream* stream_lineStack = serializar_gossip(lineStack);
        ENVIAR->data = realloc(ENVIAR->data, ENVIAR->size + stream_lineStack->size);
        memcpy(ENVIAR->data + offset, stream_lineStack->data, stream_lineStack->size);
        ENVIAR->size += stream_lineStack->size;
        offset += stream_lineStack->size;
        stream_destroy(stream_lineStack);
    }

    uint32_t count_line_stack = list_size(gossip);
    size_to_send = sizeof(count_line_stack);
    memcpy(ENVIAR->data + offset, &(count_line_stack),size_to_send);
    offset += size_to_send;

    list_iterate(gossip, serialize_element_stack);

    serializar_int(socket, ENVIAR->size);
    send_data(socket, ENVIAR->data, ENVIAR->size);
    free(ENVIAR);
}

t_list* deserializar_gossip_table(uint32_t socket) {
    uint32_t buffer_size = deserializar_int(socket);
    void* buffer = malloc(buffer_size);
    t_list* gossip_list = list_create();
    uint32_t tmp_size = 0;
    uint32_t offset = 0;
    uint32_t size_to_recive;

    recive_data(socket, buffer, buffer_size);

    uint32_t count_line_stack = 0;
    size_to_recive = sizeof(count_line_stack);
    memcpy(&count_line_stack, buffer + offset, size_to_recive);
    offset += size_to_recive;

    for(i = 0; i < count_line_stack; ++i){
        gossip_tad* lineStack = deserializar_gossip(buffer + offset, &tmp_size);
        offset += tmp_size;
        list_add(gossip_list, lineStack);
    }

    free(buffer);
    return gossip_list;
}