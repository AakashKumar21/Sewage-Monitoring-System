/*
 * helper.h
 *
 *  Created on: 31-Aug-2020
 *      Author: Aakash Kumar
 */
#ifndef DHT11_DRIVER
#define DHT11_DRIVER


#include "main.h"

typedef enum{
	OK,
	Timeout,
	CheckSumError
}DHT11_Status;

//public
void DHT11_conf(GPIO_TypeDef* port, uint32_t pin);
DHT11_Status read();
int8_t getTemp();
uint8_t getHumidity();
uint8_t fullData[5];
//private
GPIO_TypeDef* _port; // TODO, can be optimized, this is using extra memory
uint32_t _pin; // TODO, can be optimized, this is using extra memory
int _temp;
int _hum;
void _request();
uint8_t _checkResponse();
void _parse();
uint8_t _readByte();
void _setOutput();
void _setInput();
DHT11_Status _status;



#endif
