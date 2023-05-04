//
// Created by Whisky on 2023/4/29.
//

#ifndef HELLOWORLD_SINGNALPROCESS_ADC_H
#define HELLOWORLD_SINGNALPROCESS_ADC_H
#include "SignalProcess.h"

extern bool adc_dma_finished;

void adc_start(ADC_HandleTypeDef *hadc, uint32_t arr, uint16_t psc);
void adc_start_auto(ADC_HandleTypeDef *hadc, uint64_t f_out);
void adc_stop(ADC_HandleTypeDef *hadc);

__unused u32 adc_average_voltage(u32 n);
void volt_freq_scan(u32 begin, u32 end, u32 step, u32 delay_time_us);

#endif //HELLOWORLD_SINGNALPROCESS_ADC_H
