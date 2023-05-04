//
// Created by Whisky on 2023/4/22.
//
#include "SignalProcess_dac.h"
#include "SignalProcessConfig.h"
#include "dac.h"
#include "delay.h"
#include <string.h>



u16 buf1[sample];
u16 buf2[sample];
u32 __channel ;

extern DMA_HandleTypeDef DMA_DAC1;
extern DMA_HandleTypeDef DMA_DAC2;

void dac_set_channel(u32 _channel)
{
    __channel = _channel;
    HAL_DAC_Start(&DAC_HANDLE, __channel);
}

void dac_init_dma(u32 _channel)
{
    __channel = _channel;
    if(__channel == DAC_CHANNEL_1)
        HAL_DMA_Start(&DMA_DAC1,(u32) buf1, (u32)&DAC_HANDLE.Instance->DHR12R1, 0);
    else if(__channel == DAC_CHANNEL_2)
        HAL_DMA_Start(&DMA_DAC2, (u32) buf2, (u32)&DAC_HANDLE.Instance->DHR12R2, 0);
}

//vol:0-3300
void dac_set_voltage(u16 vol)
{
    double temp = vol;
    temp /= 1000;
    temp = temp * 4096 / 3.3;
    if(temp >= 4096) temp = 4095;
    HAL_DAC_SetValue(&DAC_HANDLE, __channel, DAC_ALIGN_12B_R, (u16)temp);
}

//生成正弦波数组
void dac_creat_sin_buf(u16 *buf, float32_t maxval, u32 samples, float phase_degrees)
{
    float32_t outdata;
    float32_t phase_radians = phase_degrees * PI / 180.0; // 将相位从角度转换为弧度
    float32_t inc = (2 * PI) / samples;
    float32_t amplitude = maxval / 2.0;  // 控制正弦波的幅度
    for(u32 i = 0; i < samples; i++)
    {
        // 正弦函数解析式：y= Asin(wx + k)
        outdata = amplitude * arm_sin_f32(inc * i + phase_radians) + amplitude; // 计算正弦波值
        outdata = outdata * 4096 / MAX_VOL; // 映射到DAC的范围
        u16 temp = round(outdata);
        if (temp >= 4096) temp = 4095;  // 确保输出不超过最大值
        buf[i] = (u16)(temp);

    }
}

// 生成三角波数组
void dac_creat_tri_buf(u16 *buf, float32_t maxval, u32 samples, u16 phase_time)
{
    float32_t outdata;
    float32_t inc = (maxval) / (samples / 2 );
    for(u32 i = 0 ; i < samples; i++)
    {
        // 三角函数解析式：y= Atri(2πx/λ + k) + b
        if (i < (samples )/2) {
            outdata = i * inc ;
        } else {
            outdata = (samples-i) * inc ;
        }
        outdata = outdata * 4096 / MAX_VOL; // 映射到 DAC 的范围
        u16 temp = round(outdata);
        if (temp >= 4096) temp = 4095;  // 确保输出不超过最大值
        buf[i] = (u16)(temp);
    }
}


bool both_channel = false;
u32 __dac_channel = CHANNEL_NO;

#include "retarget.h"

void dac_dma_enable(u16 cndtr, u16 arr, u16 psc)
{
    HAL_TIM_Base_Stop(&htim8);
    TIM_DAC.Instance->CNT = 0;
    TIM_DAC.Instance->PSC = psc;
    TIM_DAC.Instance->ARR = arr;
    HAL_DAC_Stop_DMA(&DAC_HANDLE, DAC_CHANNEL_1);
    HAL_DAC_Stop_DMA(&DAC_HANDLE, DAC_CHANNEL_2);

    if (__dac_channel == CHANNEL_1_2 || both_channel == true){
        HAL_DAC_Start_DMA(&DAC_HANDLE, DAC_CHANNEL_1, (u32 *)buf1, cndtr, DAC_ALIGN_12B_R);
        HAL_DAC_Start_DMA(&DAC_HANDLE, DAC_CHANNEL_2, (u32 *)buf2, cndtr, DAC_ALIGN_12B_R);
    }
    else if (__dac_channel == CHANNEL_2)
    {
        HAL_DAC_Start_DMA(&DAC_HANDLE, DAC_CHANNEL_2, (u32 *)buf2, cndtr, DAC_ALIGN_12B_R);
    }
    else if (__dac_channel == CHANNEL_1)
    {
        HAL_DAC_Start_DMA(&DAC_HANDLE, DAC_CHANNEL_1, (u32 *)buf1, cndtr, DAC_ALIGN_12B_R);
    }

    HAL_TIM_Base_Start(&TIM_DAC);
}

void dac_set(WAVEFORM_TYPE mode, float phase, float32_t maxval,u32 channel)
{

    if (__dac_channel != channel)
        both_channel = true;

    __dac_channel = channel;

    if (mode == WAVEFORM_SINE)
    {

        if (__dac_channel == CHANNEL_2)
        {
            memset(buf2, 0, sample);
            dac_creat_sin_buf(buf2, maxval, sample, phase);
            dac_init_dma(DAC_CHANNEL_2);
        }
        else if (__dac_channel == CHANNEL_1)
        {
            memset(buf1, 0, sample);
            dac_creat_sin_buf(buf1, maxval, sample, phase);
            dac_init_dma(DAC_CHANNEL_1);
        }
        else
        {
            memset(buf1, 0, sample);
            memset(buf2, 0, sample);
            dac_creat_sin_buf(buf1, maxval, sample, 0);
            dac_creat_sin_buf(buf2, maxval, sample, phase);
            dac_init_dma(DAC_CHANNEL_1);
            dac_init_dma(DAC_CHANNEL_2);
        }
    }

    if (mode == WAVEFORM_TRIANGLE)
    {



        if (__dac_channel == CHANNEL_2)
        {
            memset(buf2, 0, sample);
            dac_creat_tri_buf(buf2, maxval, sample, phase);
            dac_init_dma(DAC_CHANNEL_2);
        }
        else if (__dac_channel == CHANNEL_1)
        {
            memset(buf1, 0, sample);
            dac_creat_tri_buf(buf1, maxval, sample, phase);
            dac_init_dma(DAC_CHANNEL_1);
        }
        else
        {
            memset(buf1, 0, sample);
            memset(buf2, 0, sample);
            dac_creat_tri_buf(buf1, maxval, sample, 0);
            dac_creat_tri_buf(buf2, maxval, sample, phase);
            dac_init_dma(DAC_CHANNEL_1);
            dac_init_dma(DAC_CHANNEL_2);
        }
    }


}

void dac_out(uint64_t f_out)
{
    uint16_t arr, psc;
    calc_tim_arr_psc(TIM_DAC_CLK/sample, f_out, &arr, &psc);
    dac_dma_enable(sample, arr, psc);
}

void dac_scan(u32 begin, u32 end, u32 step, u32 delay_time_us)
{
    for (u32 i = begin; i < end; i += step) {
        uint16_t arr, psc;
        calc_tim_arr_psc(TIM_DAC_CLK/sample, i, &arr, &psc);
        TIM_DAC.Instance->CNT = 0;
        TIM_DAC.Instance->PSC = psc;
        TIM_DAC.Instance->ARR = arr;
        if (__dac_channel == CHANNEL_2)
        {
            HAL_DAC_Start_DMA(&DAC_HANDLE, DAC_CHANNEL_2, (u32 *)buf2, sample, DAC_ALIGN_12B_R);
        }
        else if (__dac_channel == CHANNEL_1)
        {
            HAL_DAC_Start_DMA(&DAC_HANDLE, DAC_CHANNEL_1, (u32 *)buf1, sample, DAC_ALIGN_12B_R);
        }
        else {
            HAL_DAC_Start_DMA(&DAC_HANDLE, DAC_CHANNEL_1, (u32 *)buf1, sample, DAC_ALIGN_12B_R);
            HAL_DAC_Start_DMA(&DAC_HANDLE, DAC_CHANNEL_2, (u32 *)buf2, sample, DAC_ALIGN_12B_R);
        }
        delay_us(delay_time_us);
    }
}

void volt_freq_scan(u32 begin, u32 end, u32 step, u32 delay_time_us)
{
    for (u32 i = begin; i < end; i += step) {
        uint16_t arr, psc;
        calc_tim_arr_psc(TIM_DAC_CLK/sample, i, &arr, &psc);
        TIM_DAC.Instance->CNT = 0;
        TIM_DAC.Instance->PSC = psc;
        TIM_DAC.Instance->ARR = arr;
        if (__dac_channel == CHANNEL_2)
        {
            HAL_DAC_Start_DMA(&DAC_HANDLE, DAC_CHANNEL_2, (u32 *)buf2, sample, DAC_ALIGN_12B_R);
        }
        else if (__dac_channel == CHANNEL_1)
        {
            HAL_DAC_Start_DMA(&DAC_HANDLE, DAC_CHANNEL_1, (u32 *)buf1, sample, DAC_ALIGN_12B_R);
        }
        else {
            HAL_DAC_Start_DMA(&DAC_HANDLE, DAC_CHANNEL_1, (u32 *)buf1, sample, DAC_ALIGN_12B_R);
            HAL_DAC_Start_DMA(&DAC_HANDLE, DAC_CHANNEL_2, (u32 *)buf2, sample, DAC_ALIGN_12B_R);
        }
        HAL_ADC_Start_DMA(ADC_HANDLE, adc_buf, ADC_DMA_BUF_SIZE);
        delay_us(delay_time_us);
    }
}
