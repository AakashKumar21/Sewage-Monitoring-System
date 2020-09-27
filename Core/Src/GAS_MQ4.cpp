/***********************************************************************************
*
*
* @Author - Aditya Kumar Mishra
* @Date	- 27/09/2020
*
*
*
*************************************************************************************/
#include "GAS_MQ4.h"

namespace AnalogSensor {
  GAS_MQ4::GAS_MQ4(int pin) {
    this->_pin = pin;
  }
  
  const float R0 = 11.820;
  const float m = -0.318;
  const float b = 1.133;

  const float VOLT_RESOLUTION = 5.0; // if 3.3v use 3.3
  const int ADC_RESOLUTION = 10; // for 10bit analog to digital converter.

  const int retries = 50;
  const int retry_interval = 20;

  double GAS_MQ4::calibrate() {
      double sensor_volt = this->getVoltage(); //Define variable for sensor voltage
      double RS_air; //Define variable for sensor resistance
      double R0; //Define variable for R0

      RS_air = ((5.0 * 10.0) / sensor_volt) - 10.0; //Calculate RS in fresh air
      R0 = RS_air / 4.4; //Calculate R0

      return R0;
  }

  double GAS_MQ4::getVoltage() {
    double avg = 0.0;
    for (int i = 0; i < retries; i ++) {
      avg += analogRead(this->_pin) / retries;
      delay(retry_interval);
    }

    double voltage = avg * VOLT_RESOLUTION / (pow(2, ADC_RESOLUTION) - 1);

    return voltage;
  }
    
    /**
     * Returns the PPM concentration
     */
    int GAS_MQ4::read() {
      double sensor_volt = this->getVoltage();
      double RS_gas; //Define variable for sensor resistance
      double ratio; //Define variable for ratio
  
      RS_gas = ((5.0 * 10.0) / sensor_volt) - 10.0; //Get value of RS in a gas
  
      ratio = RS_gas / R0;   // Get ratio RS_gas/RS_air

      double ppm_log = (log10(ratio) - b) / m; //Get ppm value in linear scale according to the the ratio value
      double ppm = pow(10, ppm_log); //Convert ppm value to log scale
      return floor(ppm);
  }
}