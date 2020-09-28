#include "dht11.h"
#include "helper.h"

// comment below line to turn off debugging
#define DEBUG_BY_GPIO
#if defined(DEBUG_BY_GPIO)
#define DPORT LED_GPIO_Port
#define DPIN LED_Pin
#define High GPIO_PIN_SET
#define Low GPIO_PIN_RESET
#define DGPIO(x) HAL_GPIO_WritePin(DPORT, DPIN, x)
#endif

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
	uint8_t response = 0;
	delay_ms(40);
	if (!(HAL_GPIO_ReadPin (_port, _pin)))
	{
		delay_ms(80);
		if ((HAL_GPIO_ReadPin (_port, _pin))) response = 1;
		else response = -1; // 255
	}
	// Start timer
	__HAL_TIM_SET_COUNTER(&htim1,0);
	HAL_TIM_Base_Start(&htim1);
	// wait for the pin to go low
	while (HAL_GPIO_ReadPin(_port, _pin) &&
		   __HAL_TIM_GET_COUNTER(&htim1) < 150);   
		
	if (__HAL_TIM_GET_COUNTER(&htim1) > 150){
		response = 0;
	}
	// Stop Timer
	__HAL_TIM_SET_COUNTER(&htim1,0);
	HAL_TIM_Base_Stop(&htim1);
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

uint8_t DHT11:: _readByte(){
	uint8_t byte;
	// HAL_TIM_Base_Start(&htim1);
	for (int i=0; i<8; i++)
	{
		// Reset timer cnt
		// __HAL_TIM_SET_COUNTER(&htim1,0);
		// wait for the pin to go high
		while (!(HAL_GPIO_ReadPin (_port, _pin)));
		// wait for 40 us
		delay_ms (40);
		// if the pin is low
		DGPIO(High); // to check sampling points
		if (!(HAL_GPIO_ReadPin (_port, _pin))){
			// write 0
			byte&= ~(1<<(7-i));
		}
		// if the pin is high, write 1
		else byte|= (1<<(7-i));
		DGPIO(Low);
		// reset timer counter
		// __HAL_TIM_SET_COUNTER(&htim1,0);
		while (HAL_GPIO_ReadPin (_port, _pin) );  // wait for the pin to go low TODO
	}
	HAL_TIM_Base_Stop(&htim1);
	return byte;
}

void DHT11:: _parse(){
	uint8_t _hum = _readByte();
	_readByte();  // Discard Decimal RH
	uint8_t _temp = _readByte();
	_readByte();  // Discard Decimal Temp
	uint8_t checkSum = _readByte();
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
