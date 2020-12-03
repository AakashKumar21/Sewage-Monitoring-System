/***********************************************************************************************
 * 
 * 
 * @Author  : ADITYA-KUMAR-MISHRA
 * @DATE    :  22/11/2020  
 * 
 * 
 * 
 * ************************************************************************************************/
#include "distance.h"
//function to calculate and return the distance of the object
// No input
// Returns distance as float in cm
//Set TRIG_PORT,TRIG_PIN, ECHO_PORT, ECHO_PIN in main.cpp
uint8_t distance(void){
    uint32_t local_time;
    // float distance;
    local_time=0;
    HAL_GPIO_WritePin(TRIG_GPIO_Port, TRIG_Pin, GPIO_PIN_RESET);  // pull the TRIG pin HIGH
    delay_ms(2);  // wait for 2 us


    HAL_GPIO_WritePin(TRIG_GPIO_Port, TRIG_Pin, GPIO_PIN_SET);  // pull the TRIG pin HIGH
    delay_ms(10);  // wait for 10 us
    HAL_GPIO_WritePin(TRIG_GPIO_Port, TRIG_Pin, GPIO_PIN_RESET);  // pull the TRIG pin low

    // read the time for which the pin is high

    while (!(HAL_GPIO_ReadPin(ECHO_GPIO_Port, ECHO_Pin)));  // wait for the ECHO pin to go high
    while (HAL_GPIO_ReadPin(ECHO_GPIO_Port, ECHO_Pin))    // while the pin is high
    {
        local_time++;   // measure time for which the pin is high
        delay_ms (1);
    }
    // float dist_f = (local_time * 0.343/2);
    uint8_t dist_i = (local_time * 0.343/2);
//    return (int)(local_time * 0.343/2);
    return (dist_i);
}
