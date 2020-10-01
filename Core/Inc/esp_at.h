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
    char *_ssid;
    char *_pass;
    void _setSingleConn();

    public:
    ESP_AT(UART_HandleTypeDef* uart);
    ESP_AT(UART_HandleTypeDef* uart, char* ssid, char*pass, char* apiKey);
    bool WifiConnect(char* ssid, char *pass);
    void setApiKey(char *key);
    bool updateValue(uint8_t field, int16_t data);
    bool WifiDisconnect();
    bool restart();
};