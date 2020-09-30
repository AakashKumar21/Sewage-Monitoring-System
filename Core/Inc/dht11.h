/*
 * helper.h
 *
 *  Created on: 31-Aug-2020
 *      Author: Aakash Kumar
 */
#ifndef DHT11_DRIVER
#define DHT11_DRIVER


#include "main.h"
#include <stdint.h>

/***** Working ****
 * After calling read()
 * Call _request()
 * Call _checkReponse and check return
 * If OK _parse()
 * return _status
 * If OK Call getTemp() and getHumidity()
*/

enum class DHT11_Status{
	OK, 			// Data recieved and checksum is correct
	Timeout,		// Sensor not responding or not connected
	CheckSumError,	// Data recieved but checksum failed
	TooFast			// Sensor polled too fast
};

class DHT11{
public:
	DHT11(GPIO_TypeDef* port, uint32_t pin);
	/**
     * @brief Constructor 
	 * @param port pointer to GPIO Port where DHT11 is connected
	 * @param pin pin no. at that port
	 * @return void
     */

	DHT11_Status read();
	/**
     * @brief Reads data(temp & hum) from DHT11 sensor and saves to private member
	 * @return enum DHT11_Status{OK,Timeout,CheckSumError}
     */

	int8_t getTemp();
	/**
     * @brief Returns temperture retrieved from sensor 
	 * Note: read() must be called before calling this, else it will return -1,
	 * and return status(of read()) must be checked before using this 
	 * data(returned from this func)  
	 * 
	 * @return 8-bit integer of temperature in celcius 
     */

	uint8_t getHumidity(); // TODO
	/**
     * @brief Returns humidity retrieved from sensor 
	 * Note: read() must be called before calling this, else it will return -1,
	 * and return status(of read()) must be checked before using this 
	 * data(returned from this func)  
	 * 
	 * @return 8-bit integer of temperature in celcius 
     */

private:
	void _request();
	/**
     * @brief Request data from sensor
	 * 
	 * @return void
     */

	uint8_t _checkResponse(); // TODO
	/**
     * @brief Reads response send by dht11 after calling _request()
	 * 
	 * @return 1 if dht11 responds else 0 // TODO
     */

	void _parse();
	/**
     * @brief Parse and calculate checksum of data recieved from sensor
	 * 
	 * @return void
     */

	uint8_t _readByte();
	/**
     * @brief Reads a byte of data from sensor
	 * 
	 * @return byte of data
     */

	void _setOutput();
	/**
     * @brief Set sensor pin as output
	 * 
	 * @return void
     */

	void _setInput();
	/**
     * @brief Set sensor pin as input with pullup
	 * 
	 * @return void
     */

	GPIO_TypeDef* const _port; // TODO, can be optimized, this is using extra memory
	uint32_t _pin; // TODO, can be optimized, this is using extra memory
	int _temp;
	int _hum;
	DHT11_Status _status;
};


#endif