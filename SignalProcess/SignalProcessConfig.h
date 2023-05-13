//
// Created by Whisky on 2023/4/22.
//

#ifndef HELLOWORLD_SIGNALPROCESSCONFIG_H
#define HELLOWORLD_SIGNALPROCESSCONFIG_H
#include "tim.h"

/**********************************/
//DEBUG
/**********************************/
//#define DEBUG
//#define FFT_DEBUG
//#define ADC_DEBUG
//#define FILTER_DEBUG
//#define PWM_DEBUG
//#define WINDOWS_DEBUG

/**********************************/
//DAC
/**********************************/
#define MAX_VOL 3300

#define DAC_BUFFER_SIZE 144 // DAC输出缓冲区大小
#define sample DAC_BUFFER_SIZE

#define DAC_HANDLE hdac
#define TIM_DAC htim8
#define TIM_DAC_CLK 144000000

#define DMA_DAC1 hdma_dac1
#define DMA_DAC2 hdma_dac2

/**********************************/
//ADC
/**********************************/
#define ADC_DMA_BUF_SIZE (4096 * 2)
#define FFT_SIZE (4096)

#define ADC_TIM htim2
#define ADC_TIM_CLK 72000000

#define PCLK2_DIV (72000000/2)
#define SAMPLETIMING (3)
#define SAMPLERATE (ADC_TIM_CLK/(ADC_TIM.Instance->PSC + 1)/(ADC_TIM.Instance->ARR + 1))

extern u32 adc_buf[ADC_DMA_BUF_SIZE];
extern ADC_HandleTypeDef *ADC_HANDLE;


/**********************************/
//FILTER
/**********************************/
#define FILTER_LENGTH_SAMPLES ADC_DMA_BUF_SIZE     /* 采样点数 */
#define BLOCK_SIZE 32                           /* 调用一次arm_fir_f32处理的采样点个数 */
#define NUM_TAPS ((100)+1)                             /* 滤波器系数个数 */


/**********************************/
//PWM
/**********************************/
#define PWM_TIM htim9
#endif //HELLOWORLD_SIGNALPROCESSCONFIG_H
