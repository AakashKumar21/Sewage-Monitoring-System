/*********************************************************************************************
*
*
*	By : Aditya Kumar Mishra
*	Created on : 27/09/2020
*
*
*
**********************************************************************************************/
#ifndef GAS_MQ4_h
#define GAS_MQ4_h


#include "main.h"


class GAS_MQ4
{
  public:
    /**
     * Constructor
     */
    GAS_MQ4(ADC_HandleTypeDef* adc); // TODO
    
    /**
     * Returns the PPM concentration
     */
    int read();

    /**
     * Calibrates the start point of 400
     */    
    double calibrate();

    /**
     * Returns the voltage
     */
    double getVoltage();
  private:
    ADC_HandleTypeDef* const _adc;
};
#endif		/* GAS_MQ4_H */