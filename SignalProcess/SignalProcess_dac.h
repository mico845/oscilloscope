//
// Created by Whisky on 2023/4/29.
//

#ifndef HELLOWORLD_SIGNALPROCESS_DAC_H
#define HELLOWORLD_SIGNALPROCESS_DAC_H
#include "SignalProcess.h"


typedef enum {
    CHANNEL_1 = DAC_CHANNEL_1,
    CHANNEL_2 = DAC_CHANNEL_2,
    CHANNEL_1_2   ,
    CHANNEL_NO
} dac_channel;

/**
 * DC
 * **/
//void dac_set_channel(u32 _channel);
//void dac_set_voltage(u16 vol);


__unused float32_t get_active_voltage(u16 active_voltage);

void dac_set(WAVEFORM_TYPE mode, float phase, float32_t  maxval,u32 channel);
void dac_out(uint64_t f_out);
void dac_scan(u32 begin, u32 end, u32 step, u32 delay_time_us);

#endif //HELLOWORLD_SIGNALPROCESS_DAC_H
