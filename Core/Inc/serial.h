#ifndef SERIAL
#define SERIAL


#include <string.h>
#include "string_methods.h"
extern UART_HandleTypeDef huart3;

const uint16_t time_multiplier = 1;

static HAL_StatusTypeDef serialPrint(char* data){
    int len = strlen(data);
    HAL_UART_Transmit(&huart3,(uint8_t*)data,len,len*time_multiplier);
}

static HAL_StatusTypeDef _serialPrint(int16_t data){
    const int len = 7;
    char buff[len]{};  // max int len = 6 + null char
    itoa(data,buff,10);
    HAL_UART_Transmit(&huart3,(uint8_t*)buff,len,len*time_multiplier);
}

static HAL_StatusTypeDef serialPrint(int data){
    _serialPrint((int16_t)data);
}


#endif