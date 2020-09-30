#include "main.h"
#include <stdint.h>
#include <string.h>
   

class ESP_AT{
    UART_HandleTypeDef* _uart;
    char _buff[100];
    char* _host = "\"api.thingspeak.com\"";

    public:
    ESP_AT(UART_HandleTypeDef* uart);
    bool connect(char* ssid, char *pass);
    bool updateValue(int field, int data);
    bool disconnect();
    bool restart();
};