#include "dht11.h"


DHT11::DHT11(GPIO_TypeDef* port,int pin):
_port(port),
_pin(pin),
_temp(-1),
_hum(-1)
{}

void DHT11::_setOutput(){
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = _pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(_port, &GPIO_InitStruct);
}

void DHT11::_setInput(){
	  GPIO_InitTypeDef GPIO_InitStruct = {0};
	  GPIO_InitStruct.Pin = ECHO_Pin;
	  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	  GPIO_InitStruct.Pull = GPIO_PULLUP;
	  HAL_GPIO_Init(ECHO_GPIO_Port, &GPIO_InitStruct);
}

uint8_t DHT11::_checkResponse(){
	uint8_t response;
	delay_ms(40);
	if(HAL_GPIO_ReadPin(_port, _pin) == 0){
		delay_ms(80);
		if (HAL_GPIO_ReadPin(_port, _pin))
			response = 1;
		else
			response = 0;
	}
	while(HAL_GPIO_ReadPin(_port, _pin)); // PROGRAM MAY DIE HERE
	return response;
}

void DHT11::_request(){
	_setOutput();
    HAL_GPIO_WritePin(_port, _pin, GPIO_PIN_RESET);
    delay_ms(18000);
    HAL_GPIO_WritePin(_port, _pin, GPIO_PIN_SET);
    delay_ms(20);
    _setInput();
}

uint8_t DHT11::_readByte(){
	uint8_t temp = 0;
	for(int i=0; i<8;i++){
		while(!HAL_GPIO_ReadPin(_port, _pin)){
			delay_ms(40);
			if(HAL_GPIO_ReadPin(_port, _pin))
				temp |= 1 << (8-i);
			else;
				//Nothing
		}
	}
	return temp;
}

void DHT11:: _parse(){
	_readByte(); // Integral RH
	_hum = _readByte(); // Decimal RH
	int temp_integral = _readByte(); // Integral T
	_temp = _readByte(); // Decimal T
	if (temp_integral < 0) _temp *= -1;
}


void DHT11::read(){
	// Steps to Read
	// 1. Check Presence
	// 2. IF Present: Parse Data
	_request();
	if (_checkResponse()) _parse();
}

int8_t DHT11::getTemp(){
	return _temp;
}

uint8_t DHT11::getHumidity(){
	return _hum;
}
