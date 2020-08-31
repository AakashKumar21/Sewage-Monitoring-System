/*
 * helper.h
 *
 *  Created on: 31-Aug-2020
 *      Author: Aakash Kumar
 */

#ifndef INC_HELPER_H_
#define INC_HELPER_H_

extern TIM_HandleTypeDef htim1;

#define TIMER &htim1
inline void delay_ms(size_t time){
	__HAL_TIM_SET_COUNTER(TIMER, 0);
	while(__HAL_TIM_GET_COUNTER(TIMER) < time);
}


#endif /* INC_HELPER_H_ */
