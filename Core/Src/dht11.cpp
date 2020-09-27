#include "dht11.h"
#include "helper.h"

DHT11::DHT11(GPIO_TypeDef* port,uint32_t pin):
_port(port),
_pin(pin),
_temp(-1),
_hum(-1)
{}

#define dht11_data_size 40

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
	  HAL_GPIO_Init(_port, &GPIO_InitStruct);
}

uint8_t DHT11::_checkResponse(){
	volatile uint8_t response;
	delay_ms(40);
	if(! HAL_GPIO_ReadPin(_port, _pin)){
		delay_ms(50);
		if (HAL_GPIO_ReadPin(_port, _pin))
			response = 1;
		else
			response = 0;
	}
	__HAL_TIM_SET_COUNTER(&htim1,0);
	HAL_TIM_Base_Start(&htim1);
	while(HAL_GPIO_ReadPin(_port, _pin) && 
		  __HAL_TIM_GET_COUNTER(&htim1) < 150); //Wait till line goes low

	if (__HAL_TIM_GET_COUNTER(&htim1) > 150){
		response = 0;
	}
	__HAL_TIM_SET_COUNTER(&htim1,0);
	HAL_TIM_Base_Stop(&htim1);
	return response;
}

void DHT11::_request(){
	_setOutput();
    HAL_GPIO_WritePin(_port, _pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
    delay_ms(18000);
    HAL_GPIO_WritePin(_port, _pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
    delay_ms(20);
    _setInput();
}

uint8_t DHT11:: _readByte(){
	uint8_t byte = 0;
	__HAL_TIM_SET_COUNTER(&htim1,0);
	HAL_TIM_Base_Start(&htim1);
	for(uint8_t i=7; i>=0; i--){
		while ( (HAL_GPIO_ReadPin(_port,_pin) == 0) && // Wait for pin to go high
				__HAL_TIM_GET_COUNTER(&htim1) < 3800 );

		delay_ms(40);
		HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
		byte |= HAL_GPIO_ReadPin(_port, _pin) << i;
		HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
		while ((HAL_GPIO_ReadPin (_port, _pin)));
	}
	if(__HAL_TIM_GET_COUNTER(&htim1) > 3800) _status = DHT11_Status::Timeout;
	HAL_TIM_Base_Stop(&htim1);
	__HAL_TIM_SET_COUNTER(&htim1,0);
	return byte;
}

void DHT11:: _parse(){
//	uint8_t _hum = _readByte();
//	uint8_t decRH = _readByte();
//	uint8_t _temp = _readByte();
//	uint8_t checkSum = _readByte();
	for(int i=0; i<5;i++){
		fullData[i] = _readByte();
	}
}


DHT11_Status DHT11::read(){
	// Steps to Read
	// 1. Check Presence
	// 2. IF Present: Parse Data
	_request();
	if (_checkResponse()) _parse();
	else _status = DHT11_Status::Timeout;
	return _status;
	// TODO
	// else the values will not change
	// and there is no indication of error
}

int8_t DHT11::getTemp(){
	return _temp;
}

uint8_t DHT11::getHumidity(){
	return _hum;
}
