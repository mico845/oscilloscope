//
// Created by Whisky on 2023/5/4.
//

#ifndef HELLOWORLD_SIGNALPROCESS_PWM_H
#define HELLOWORLD_SIGNALPROCESS_PWM_H
#include "main.h"

typedef struct {
    uint64_t fclk;
} dsp_args;

#define pwm_out(htim, channel, f_out, ...) _pwm_out(htim, channel, f_out, (dsp_args){__VA_ARGS__})
#define pwm_input(htim, channel, f_s, ...) _pwm_input(htim, channel, f_s, (dsp_args){__VA_ARGS__})

void _pwm_input(TIM_HandleTypeDef *htim, u32 channel,uint64_t f_s, dsp_args in);
void _pwm_out(TIM_HandleTypeDef *htim, u32 channel,uint64_t f_out, dsp_args in);


#endif //HELLOWORLD_SIGNALPROCESS_PWM_H
