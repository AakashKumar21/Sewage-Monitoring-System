#include "esp_at.h"

enum index{
    Mode,
    Connect,
    Reset,
    Disconnect,
    AT
};

char* commands[] = { // Replace CUR with DEF to save in flash
    "CWMODE_CUR=", 
    "CWJAP_CUR",
    "RST",
    "CWQAP"
    "AT+",
};

ESP_AT::ESP_AT(UART_HandleTypeDef* uart):
_uart(uart)
{
    // Set station pt as default
    // set single tcp connection
}

bool ESP_AT::connect(char* ssid, char *pass){
    int len; // strlen
    uint16_t t; // timeout for uart tx

    // tx: AT+
    HAL_UART_Transmit(_uart,(uint8_t*)commands[index::AT], 
                     strlen(commands[index::AT]),10);
    // tx: CWJAP_CUR=
    HAL_UART_Transmit(_uart,(uint8_t*)commands[index::Connect], 
                      strlen(commands[index::Connect]),20); 
    // tx: ssid
    len = strlen(ssid);
    HAL_UART_Transmit(_uart,(uint8_t*)ssid, len, len*2);
    // tx: pass
    len = strlen(pass);
    HAL_UART_Transmit(_uart,(uint8_t*)pass, len, len*2);
    // tx: \n
    HAL_UART_Transmit(_uart,(uint8_t*)"\n", 1,2); 
}
bool disconnect(){};
bool restart(){};