#include "main.h"
#include <stdint.h>
#include <string.h>
   

class ESP_AT{
    UART_HandleTypeDef* _uart;
    char *_ssid;
    char *_pass;
    char* _apiKey;
    char _host[23]; // \"api.thingspeak.com\"\0

    public:
    ESP_AT(UART_HandleTypeDef* uart);
    ESP_AT(UART_HandleTypeDef* uart, char* ssid, char*pass, char* apiKey);
    bool WifiConnect(char* ssid, char *pass);
    void setApiKey(char *key);
    bool updateValue(uint8_t field1, int16_t data1,uint8_t field2, int16_t data2,uint8_t field3, int16_t data3,uint8_t field4, int16_t data4 );
    bool WifiDisconnect();
    bool restart();
    void setSingleConn();
};