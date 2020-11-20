#include "main.h"
#include <stdint.h>
#include <string.h>

class ESP_AT{
    UART_HandleTypeDef* _uart;
    char *_ssid;
    char *_pass;
    char* _apiKey;
    char _host[23]; // \"api.thingspeak.com\"\0
    static const uint8_t _bufferSize = 50;
	uint8_t _buffer[_bufferSize]; // to store serial data coming from esp8266
    void _setSingleConn();
    const size_t _timeout = 50; // 2 sec
    const size_t _timeoutN = 100;
    const size_t _maxRetries = 4;

    public:
    ESP_AT(UART_HandleTypeDef* uart);
    ESP_AT(UART_HandleTypeDef* uart, char* ssid, char*pass, char* apiKey);
    bool WifiConnect(char* ssid, char *pass);
    void setApiKey(char *key);
    bool updateValue(uint8_t field, int16_t data);
    bool WifiDisconnect();
    bool restart();
};
