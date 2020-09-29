#include "timer.h"

Timer::Timer(TIM_HandleTypeDef& tim): 
_tim(tim)
{}
void Timer::start(){
    __HAL_TIM_SET_COUNTER(&_tim,0);
    HAL_TIM_Base_Start(&_tim);
}

void Timer::stop(){
    HAL_TIM_Base_Stop(&_tim);
}
void Timer::reset(){
    __HAL_TIM_SET_COUNTER(&_tim,0);
}
uint16_t Timer::getTick() const{
    return __HAL_TIM_GET_COUNTER(&_tim);
}