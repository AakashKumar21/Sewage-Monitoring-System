/*
 * utils.h
 *
 *  Created on: Sep 25, 2020
 *      Author: aakash
 */

#ifndef INC_HELPER_H_
#define INC_HELPER_H_

#include "main.h"

void delay_ms(size_t time);

HAL_StatusTypeDef serialPrint(char* data);

HAL_StatusTypeDef _serialPrint(int16_t data);

HAL_StatusTypeDef serialPrint(int data);

void _swap(char &a, char &b);

void _reverse(char str[], int length);

void itoa(int16_t num, char* str, int base=10);

void delay_ms(size_t time);


#endif /* INC_HELPER_H_ */
