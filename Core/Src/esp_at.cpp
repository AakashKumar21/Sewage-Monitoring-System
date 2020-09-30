#include "esp_at.h"

enum index{
    Mode,
    Connect,
    Reset,
    Disconnect,
    AT,
    TCP_no_of_conn,
    StartTCP,
    TCPMode,
    SendTCP,
};

char* commands[] = { // Replace CUR with DEF to save in flash
    "CWMODE_CUR=", 
    "CWJAP_CUR",
    "RST",
    "CWQAP"
    "AT+",
    "CIPMUX=",
    "CIPSTART="
    "\"TCP\"",
    "CIPSEND="
};

ESP_AT::ESP_AT(UART_HandleTypeDef* uart):
_uart(uart)
{
    // Set station pt as default
    // set single tcp connection
    // tx: AT+
    HAL_UART_Transmit(_uart,(uint8_t*)commands[index::AT],   // TODO, not DRY
                    strlen(commands[index::AT]),10);
    // tx: CIPMUX=
    HAL_UART_Transmit(_uart,(uint8_t*)commands[index::TCP_no_of_conn],  
                               strlen(commands[index::TCP_no_of_conn]),20);
    // tx: 0
    HAL_UART_Transmit(_uart,(uint8_t*)"0", 1,2);
}

bool ESP_AT::connect(char* ssid, char *pass){
    int len; // strlen
    uint16_t t; // timeout for uart tx

    // tx: AT+
    HAL_UART_Transmit(_uart,(uint8_t*)commands[index::AT], // TODO, not DRY
                     strlen(commands[index::AT]),10);
    // tx: CWJAP_CUR=
    HAL_UART_Transmit(_uart,(uint8_t*)commands[index::Connect], 
                      strlen(commands[index::Connect]),20); 
    // tx: ssid
    len = strlen(ssid);
    HAL_UART_Transmit(_uart,(uint8_t*)ssid, len, len*2);
    // tx: ,
    HAL_UART_Transmit(_uart,(uint8_t*)",", 1,2); 
    // tx: pass
    len = strlen(pass);
    HAL_UART_Transmit(_uart,(uint8_t*)pass, len, len*2);
    // tx: \n
    HAL_UART_Transmit(_uart,(uint8_t*)"\n", 1,2); 
}

bool ESP_AT::updateValue(int field, int data){
    // tx: AT+
    HAL_UART_Transmit(_uart,(uint8_t*)commands[index::AT], // TODO, not DRY
                     strlen(commands[index::AT]),10);

    // tx: CIPSTART=
    HAL_UART_Transmit(_uart,(uint8_t*)commands[index::StartTCP],
                               strlen(commands[index::StartTCP]),20);
    // tx: "TCP"
    HAL_UART_Transmit(_uart,(uint8_t*)commands[index::TCPMode],
                               strlen(commands[index::TCPMode]),20);
    // ,
    HAL_UART_Transmit(_uart,(uint8_t*)",",1,2);
    // "api.thingspeak.com"
    HAL_UART_Transmit(_uart,(uint8_t*)_host, strlen(_host),40);
    // ,
    HAL_UART_Transmit(_uart,(uint8_t*)",",1,2);
    // 80
    HAL_UART_Transmit(_uart,(uint8_t*)"80",2,4);
    // tx: \n
    HAL_UART_Transmit(_uart,(uint8_t*)"\n", 1,2);


    // AT+CIPSEND=
    HAL_UART_Transmit(_uart,(uint8_t*)commands[index::SendTCP], // TODO, not DRY
                               strlen(commands[index::SendTCP]),10);
}
bool disconnect(){};
bool restart(){};