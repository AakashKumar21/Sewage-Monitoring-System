// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "helper.h"
#include <stdint.h>
#include <string.h>

extern UART_HandleTypeDef huart3;
extern TIM_HandleTypeDef htim1;

const uint16_t time_multiplier = 1;

void delay_ms(size_t time){
	if (time> UINT16_MAX) time = UINT16_MAX; // May cause too much overhead
											// tradeoff for safety
	HAL_TIM_Base_Start(&htim1);
	while(__HAL_TIM_GET_COUNTER(&htim1) < time);
	HAL_TIM_Base_Stop(&htim1);
	htim1.Instance->CNT = 0;
}


HAL_StatusTypeDef serialPrint(char* data){
    int len = strlen(data);
    return HAL_UART_Transmit(&huart3,(uint8_t*)data,len,len*time_multiplier);
}

HAL_StatusTypeDef _serialPrint(int16_t data){
    const int len = 7;
    char buff[len]{};  // max int len = 6 + null char
    itoa(data,buff);
    return HAL_UART_Transmit(&huart3,(uint8_t*)buff,len,len*time_multiplier);
}

HAL_StatusTypeDef serialPrint(int data){
    return _serialPrint((int16_t)data);
}

void _swap(char &a, char &b){
    char temp = a;
    a = b;
    b = temp;
}

void _reverse(char *str, int length){ 
    int start = 0; 
    int end = length - 1; 
    while (start < end){ 
        _swap(*(str+start), *(str+end)); 
        start++; 
        end--; 
    } 
} 

void itoa(int16_t num, char* str, int base){ 
    int i = 0; 
    bool isNegative = false; 
  
    /* Handle 0 explicitely, otherwise empty string is printed for 0 */
    if (num == 0){ 
        str[i++] = '0'; 
        str[i] = '\0'; 
        return; 
    } 
  
    // In standard itoa(), negative numbers are handled only with  
    // base 10. Otherwise numbers are considered unsigned. 
    if (num < 0 && base == 10){ 
        isNegative = true; 
        num = -num; 
    } 
  
    // Process individual digits 
    while (num != 0){ 
        int rem = num % base; 
        str[i++] = (rem > 9)? (rem-10) + 'a' : rem + '0'; 
        num = num/base; 
    } 
  
    // If number is negative, append '-' 
    if (isNegative) 
        str[i++] = '-'; 
  
    str[i] = '\0'; // Append string terminator 
  
    // Reverse the string 
    _reverse(str, i); 
}