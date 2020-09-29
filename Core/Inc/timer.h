#ifndef TIMER_
#define TIMER_

#include "main.h"

extern TIM_HandleTypeDef htim2;
class Timer{
    TIM_HandleTypeDef& _tim;

    public:
    Timer(TIM_HandleTypeDef& tim);
    void start();
    void stop();
    void reset();
    uint16_t getTick() const;
};

static auto timer2 = Timer(htim2);
#endif