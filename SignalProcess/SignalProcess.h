//
// Created by Whisky on 2023/4/5.
//

#ifndef HELLOWORLD_SIGNALPROCESS_H
#define HELLOWORLD_SIGNALPROCESS_H
#include "main.h"
#include "arm_math.h"
#include <stdbool.h>



typedef enum {
    WAVEFORM_UNKNOWN = 0,   // 未知波形
    WAVEFORM_SQUARE,        // 方波
    WAVEFORM_TRIANGLE,      // 三角波
    WAVEFORM_SINE           // 正弦波
} WAVEFORM_TYPE;

/*
 * FUNCTION
 * */



void calc_tim_arr_psc(uint32_t f_in, uint32_t f_out, uint16_t *arr, uint16_t *psc);
void freq_to_arr_psc(float32_t freq, float32_t clk, uint16_t* arr, uint16_t* psc);

__unused float32_t get_active_voltage(u16 active_voltage);
u32 digital_to_analog_mv(float32_t num);

float32_t get_signal_max(float32_t *signal, u32 buffer_size);
float32_t get_signal_min(float32_t *signal, u32 buffer_size);
float32_t get_signal_peak_to_peak(float32_t *signal, u32 buffer_size);


//--------adc 处理函数
void adc_process(void);

#endif //HELLOWORLD_SIGNALPROCESS_H
