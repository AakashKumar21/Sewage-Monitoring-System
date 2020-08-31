#include <stdint.h>
#include "main.h"

extern inline void delay_ms(size_t time); // FIX

class DHT11{
public:
	DHT11(GPIO_TypeDef* port, int pin);
	void read();
	int8_t getTemp();
	uint8_t getHumidity();

private:
	GPIO_TypeDef* const _port;
	const uint16_t _pin;
	int _temp;
	int _hum;
	void _request();
	uint8_t _checkResponse();
	void _parse();
	uint8_t _readByte();
	void _setOutput();
	void _setInput();
};
