/*
 * utils.h
 *
 *  Created on: Sep 25, 2020
 *      Author: aakash
 */

#ifndef INC_HELPER_H_
#define INC_HELPER_H_

#include "main.h"

extern TIM_HandleTypeDef htim1;

void delay_ms(size_t time){
	if (time> UINT16_MAX) time = UINT16_MAX; // May cause too much overhead
											// tradeoff for safety
	HAL_TIM_Base_Start(&htim1);
	while(__HAL_TIM_GET_COUNTER(&htim1) < time);
	HAL_TIM_Base_Stop(&htim1);
	htim1.Instance->CNT = 0;
}


#endif /* INC_HELPER_H_ */
