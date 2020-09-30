#include "main.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define KeyLength 16


class ESP_AT{
    UART_HandleTypeDef* _uart;
    char _buff[100];
    char* _host = "\"api.thingspeak.com\"";
    char* _apiKey;

    public:
    ESP_AT(UART_HandleTypeDef* uart, char* key);
    bool connect(char* ssid, char *pass);
    bool updateValue(uint8_t field, int16_t data);
    bool disconnect();
    bool restart();
};