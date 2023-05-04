#include <argz.h>
//
// Created by Whisky on 2023/4/5.
//
#include "SignalProcessConfig.h"
#include "my_dsp.h"
#include "math.h"
#include "retarget.h"
#include "delay.h"
#include "lcd.h"
#include "gui.h"

float graph_x_scale = 16.0;



//输入频率f_in，要求输出频率f_out，满足公式f_in/ (arr * psc) = f_out,计算arr和psc
void calc_tim_arr_psc(uint32_t f_in, uint32_t f_out, uint16_t *arr, uint16_t *psc)
{
    // Calculate the timer period and prescaler values
    double ratio = (double)f_in / (double)f_out;
    double f_arr = ratio - 1.0f;
    uint32_t _arr = round(f_arr);
    *psc = 0;
    if (_arr > 65535) {
        _arr /= 2;
        (*psc)++;
    }
    if (_arr < 1) {
        *arr = 1;
        *psc = 0;
    } else {
        *arr = (uint16_t)_arr;
    }
#ifdef DEBUG
    printf("f_out: %lu\n", f_out);
    printf("f_in: %llu\n", (uint64_t)f_in);
    printf("*psc: %d\n", *psc);
    printf("*arr: %d\n", *arr);
#endif
}


void freq_to_arr_psc(float32_t freq, float32_t clk, uint16_t* arr, uint16_t* psc)
{
    uint32_t prescaler_max = 65535;
    uint32_t period_max = 65535;
    uint32_t period_min = 1;
    uint32_t prescaler = 0;
    uint32_t period = 0;
    float32_t f;
    float32_t error;
    float32_t error_min = 1e6;

    for (uint32_t i = 1; i <= prescaler_max; i++)
    {
        uint32_t j = (uint32_t)(clk / (freq * (i + 1))) - 1;
        if (j < period_min) j = period_min;
        if (j > period_max) j = period_max;
        f = clk / (float32_t)((j + 1) * i);
        error = fabsf(f - freq);
        if (error < error_min)
        {
            error_min = error;
            prescaler = i - 1;
            period = j;
            if (error_min == 0.0f) break;
        }
    }

    *psc = (uint16_t)prescaler;
    *arr = (uint16_t)period;
}

__unused float32_t get_active_voltage(u16 active_voltage)
{
    float32_t volt = active_voltage;
    float32_t result_volt;
    arm_scale_f32(&volt, (1.4142135623), &result_volt,1);
    return result_volt;
}

u32 digital_to_analog_mv(float32_t num)
{
    u32 volt = 3300 * num;
    volt = volt >> 12;
    return volt;
}

//信号的最大值
float32_t get_signal_max(float32_t *signal, u32 buffer_size)
{
    float32_t max = 0.0f;
    arm_max_f32(signal, buffer_size, &max, NULL);
    return max;
}

//信号的最小值
float32_t get_signal_min(float32_t *signal, u32 buffer_size)
{
    float32_t min = 0.0f;
    arm_min_f32(signal, buffer_size, &min, NULL);
    return min;
}

//信号的峰峰值
float32_t get_signal_peak_to_peak(float32_t *signal, u32 buffer_size)
{
    float32_t min = 0.0f;
    float32_t max = 0.0f;
    arm_max_f32(signal, buffer_size, &max, NULL);
    arm_min_f32(signal, buffer_size, &min, NULL);
    return (max - min);
}

//信号的有效值
float32_t get_signal_rms(float32_t *signal, u32 buffer_size)
{
    float32_t rms_value = 0.0f;
    arm_rms_f32(signal, buffer_size, &rms_value);
    return rms_value;
}


///--------------------
///结果处理
float32_t estimation(float32_t x)
{
//    float32_t y;
//    if (x <= 14047.85)
//        y = 0.9751*x + 301.84;
//    else
//        y = 0.9752*x + 399.99;
//    return y;
}


uint64_t  auto_samplerate = 409600/2;
void adc_process(void)
{
    /* 滤波  */
#if 0
    float32_t fir_buf[ADC_DMA_BUF_SIZE];
    float32_t signal[ADC_DMA_BUF_SIZE];
    for (u32 i = 0; i < ADC_DMA_BUF_SIZE; ++i) {
        signal[i] = adc_buf[i];
    }
    filter_fir(signal, fir_buf);
    cfft(fir_buf, FFT_SIZE, 0, 1, fir_buf);
    float32_t freq = fft_get_freq(fir_buf, FFT_SIZE);
    printf("freq: %f\n",freq);
#endif
    /* 基本测量 */
#if 1
    float32_t signal[FFT_SIZE];
    for (u32 i = 0; i < FFT_SIZE; ++i)
    {
        signal[i] = adc_buf[i];
#ifdef ADC_DEBUG
        printf("%lu,%lu\n", i, adc_buf[i]);
#endif
    }
#endif

//----------gui
    gui_clear_frame(LGRAY);
    gui_ref_axis(LIGHTGREEN);

//----------连线绘图
    graph((u32 *)adc_buf, 4096, ADC_DMA_BUF_SIZE, graph_x_scale, YELLOW);
#if 1
//----------基本测量
    float32_t max = get_signal_max(signal, FFT_SIZE);
    float32_t min= get_signal_min(signal, FFT_SIZE);
    float32_t pp_value= get_signal_peak_to_peak(signal, FFT_SIZE);
    float32_t rms= get_signal_rms(signal, FFT_SIZE);

//----------fft处理
    cfft(signal, FFT_SIZE, 0, 1, signal);
//----------signal 变为 fft结果
    float32_t freq = fft_get_freq(signal, FFT_SIZE);
#ifdef AUTO_SAMPLERATE
    if (adc_check( freq * 10))
        auto_samplerate = freq * 10;
#endif
    float32_t THD = fft_get_THD(signal, FFT_SIZE, 5);

    WAVEFORM_TYPE wave = fft_recognize_wave(signal, FFT_SIZE, 5);

    show_result(freq, THD, max, min, pp_value, rms, wave);

#endif
    //show_result(1000,  0.01, 3299, 0, 3299, 2000);
}