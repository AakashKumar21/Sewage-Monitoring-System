#include "esp_at.h"
#include "string_methods.h"
#include "debug_gpio.h"
#include "main.h"

extern UART_HandleTypeDef huart3;

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
    ThingspeakApi_1,
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
    "CIPSEND=",
    "GET /update?api_key="
};

ESP_AT::ESP_AT(UART_HandleTypeDef* uart,char *api):
_uart(uart),
_apiKey(api)
{
    // Set station pt as default
    // set single tcp connection
    // tx: AT+
    DGPIO(High);
    DGPIO(Low);
    DGPIO(High);
    HAL_UART_Transmit(&huart3,(uint8_t*)"AT+",3,10);
    // tx: CIPMUX=
    HAL_UART_Transmit(_uart,(uint8_t*)commands[index::TCP_no_of_conn],  
                               strlen(commands[index::TCP_no_of_conn]),20);
    // tx: 0
    HAL_UART_Transmit(_uart,(uint8_t*)"0", 1,2);
    DGPIO(Low);
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

bool ESP_AT::updateValue(uint8_t field, int16_t data){
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
    HAL_UART_Transmit(_uart,(uint8_t*)commands[index::AT],
                     strlen(commands[index::AT]),10);
    // int: size
    char data_buff[10];
    char chId_buff[10];
    char payload_len_buf[10];
    itoa(data, data_buff, 10);
    itoa(field, chId_buff, 10);
    // size of "GET /update?api_key=XXXXXXXXXXXXXXXX&field#=" is 44 
    int payload_len = 44 + strlen(data_buff) + strlen(chId_buff);
    itoa(payload_len, payload_len_buf, 10);

    // GET /update?api_key=
    HAL_UART_Transmit(_uart,(uint8_t*)commands[index::ThingspeakApi_1],
                               strlen(commands[index::ThingspeakApi_1]),10);
    // XXXXXXXXXXXXXXXX
    HAL_UART_Transmit(_uart,(uint8_t*)_apiKey,KeyLength,30);
    // field#
    HAL_UART_Transmit(_uart,(uint8_t*)chId_buff, strlen(chId_buff),20);
    // =
    HAL_UART_Transmit(_uart,(uint8_t*)"=",1,2);
    // int: data
    HAL_UART_Transmit(_uart,(uint8_t*)data_buff, strlen(data_buff),20);
    // tx: \n
    HAL_UART_Transmit(_uart,(uint8_t*)"\n", 1,2);
}
bool disconnect(){};
bool restart(){};