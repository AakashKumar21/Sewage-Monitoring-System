// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "dht11.h"
#include "helper.h"
#include "timer.h"

// Comment/uncomment below line to turn off/on debugging
// #define DEBUG_BY_GPIO
#if defined(DEBUG_BY_GPIO)
#define DGPIO(x) HAL_GPIO_WritePin(DPORT, DPIN, x)
#endif
#if !defined(DEBUG_BY_GPIO)
#define DGPIO(x)
#endif

#define DPORT LED_GPIO_Port
#define DPIN LED_Pin
#define High GPIO_PIN_SET
#define Low GPIO_PIN_RESET

#define TIMEOUT 150

DHT11::DHT11(GPIO_TypeDef* port,uint32_t pin):
_port(port),
_pin(pin),
_temp(-1),
_hum(-1)
{}


void DHT11::_setOutput() const{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = _pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(_port, &GPIO_InitStruct);
}

void DHT11::_setInput() const{
	  GPIO_InitTypeDef GPIO_InitStruct = {0};
	  GPIO_InitStruct.Pin = ECHO_Pin;
	  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	  GPIO_InitStruct.Pull = GPIO_PULLUP;
	  HAL_GPIO_Init(_port, &GPIO_InitStruct);
}

uint8_t DHT11::_checkResponse(){
	DHT11_Status _status{};
	uint8_t response = 0;
	delay_ms(40);
	if (!(HAL_GPIO_ReadPin (_port, _pin))){
		delay_ms(80);
		if ((HAL_GPIO_ReadPin (_port, _pin))) response = 1;
		else response = -1;  // TODO
	}

	timer2.start();
	// wait for the pin to go low
	while (HAL_GPIO_ReadPin(_port, _pin) &&
		   timer2.getTick() < TIMEOUT);   
		
	if (timer2.getTick() > TIMEOUT){
		_status = DHT11_Status::Timeout;
		response = 0;
	}
	timer2.stop();
	return response;
}

void DHT11::_request() const{
	_setOutput();
    HAL_GPIO_WritePin(_port, _pin, GPIO_PIN_RESET);
    delay_ms(18000);
    HAL_GPIO_WritePin(_port, _pin, GPIO_PIN_SET);
    delay_ms(20);
    _setInput();
}

uint8_t DHT11:: _readByte(){
	DHT11_Status _status{};
	volatile uint8_t byte;
	for (int i=0; i<8; i++)
	{
		timer2.start();
		// wait for the pin to go high
		while (!(HAL_GPIO_ReadPin (_port, _pin)) &&
				timer2.getTick() < TIMEOUT );
		if(timer2.getTick() > TIMEOUT) _status = DHT11_Status::Timeout;
		timer2.stop();
		// wait for 40 us
		delay_ms (40);
		DGPIO(High); // to check sampling points
		if (!(HAL_GPIO_ReadPin (_port, _pin)) && // if the pin is low
			timer2.getTick() < TIMEOUT ){ 
			byte&= ~(1<<(7-i)); // write 0
		}
		else byte|= (1<<(7-i)); // if the pin is high, write 1
		DGPIO(Low);
		timer2.start();
		while (HAL_GPIO_ReadPin (_port, _pin) && // wait for the pin to go low TODO
				timer2.getTick() < TIMEOUT);  
		if(timer2.getTick() > TIMEOUT) _status = DHT11_Status::Timeout;
		timer2.stop();
	}
	return byte;
}

void DHT11:: _parse(){
	DHT11_Status _status{};
	_hum = _readByte();
	auto byte2 = _readByte();  // Discard Decimal RH
	_temp = _readByte();
	auto byte4 = _readByte();  // Discard Decimal Temp
	uint8_t checkSum = _readByte();

	// Sum of all bytes must be equal to checksum for data integrity
	if((_hum + _temp + byte2 + byte4) == checkSum){
		_status = DHT11_Status::OK;
	}
}


DHT11_Status DHT11::read(){
	DHT11_Status _status{};
	_request(); // Request temp and hum data
	if (_checkResponse()) _parse(); // If response is OK then parse data
	else _status = DHT11_Status::Timeout; // If not OK means its timeout error(DHT11 absent/ not resp)
	
	return _status;
}

int8_t DHT11::getTemp(){
	return _temp;
}

uint8_t DHT11::getHumidity(){
	return _hum;
}
