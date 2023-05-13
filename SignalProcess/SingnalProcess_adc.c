#include <argz.h>
//
// Created by Whisky on 2023/4/22.
//
#include "SingnalProcess_adc.h"
#include "SignalProcessConfig.h"
#include "delay.h"
#include "retarget.h"
#include "lcd.h"

u32 adc_buf[ADC_DMA_BUF_SIZE] = {0};
ADC_HandleTypeDef *ADC_HANDLE;

bool adc_check()
{
    if (SAMPLERATE > PCLK2_DIV/(SAMPLETIMING + 12))
    {
        printf("error: SAMPLERATE can't satisfy adc sampletiming\n");
        printf("SAMPLERATE: %lu ADC TIME: %d\n", SAMPLERATE, PCLK2_DIV/(SAMPLETIMING + 12));
        return false;
    }
    else
        return true;
}

void adc_init_dma(u32 *mar,u16 cndtr)
{
    if(!adc_check())
        return;
    HAL_ADC_Start_DMA(ADC_HANDLE, mar, cndtr);
    HAL_TIM_Base_Start(&ADC_TIM);
}

void adc_start(ADC_HandleTypeDef *hadc, uint32_t arr, uint16_t psc)
{
    ADC_HANDLE = hadc;
    ADC_TIM.Instance->CNT = 0;
    ADC_TIM.Instance->ARR = arr;
    ADC_TIM.Instance->PSC = psc;
    adc_init_dma(adc_buf, ADC_DMA_BUF_SIZE);
}

void adc_start_auto(ADC_HandleTypeDef *hadc, uint64_t f_out)
{
    ADC_HANDLE = hadc;
    uint16_t arr, psc;
    calc_tim_arr_psc(ADC_TIM_CLK, f_out, &arr, &psc);
    ADC_TIM.Instance->CNT = 0;
    ADC_TIM.Instance->ARR = arr;
    ADC_TIM.Instance->PSC = psc;
    adc_init_dma(adc_buf, ADC_DMA_BUF_SIZE);
}

#include <string.h>

void adc_stop(ADC_HandleTypeDef *hadc)
{
    HAL_TIM_Base_Stop(&ADC_TIM);
    HAL_ADC_Stop_DMA(hadc);
    memset(adc_buf, 0, sample);
}

__unused u32 adc_average_voltage(u32 n)
{
    uint64_t sum = 0;
    for (u32 i = 0; i < n; ++i) {
        sum += adc_buf[i];
    }
    sum = (u32)round((double )sum / (double)n);
    u32 volt = 3300 * sum;
    volt = volt >> 12;
    return volt;
}

bool adc_dma_finished = false;


// 定义中断处理函数，处理DMA传输完成中断
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    HAL_ADC_Stop_DMA(hadc);
    HAL_TIM_Base_Stop(&ADC_TIM);
    if (hadc->Instance == ADC1)
    {
        adc_dma_finished = true;
    }

}

