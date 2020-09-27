/*
 * helper.h
 *
 *  Created on: 31-Aug-2020
 *      Author: Aakash Kumar
 */
#ifndef DHT11_DRIVER
#define DHT11_DRIVER

#include <stdint.h>
#include "main.h"
#include "helper.h"

typedef enum{
	OK,
	Timeout,
	CheckSumError
}DHT11_Status;

extern void delay_ms(size_t time); // TODO, test working

class DHT11{
public:
	DHT11(GPIO_TypeDef* port, int pin);
	void read();
	int8_t getTemp();
	uint8_t getHumidity();

private:
	GPIO_TypeDef* const _port; // TODO, can be optimized, this is using extra memory
	const uint16_t _pin; // TODO, can be optimized, this is using extra memory
	int _temp;
	int _hum;
	void _request();
	uint8_t _checkResponse();
	void _parse();
	uint8_t _readByte();
	void _setOutput();
	void _setInput();
};

#endif
