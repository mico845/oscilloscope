//
// Created by Whisky on 2023/5/3.
//

#ifndef HELLOWORLD_GUI_H
#define HELLOWORLD_GUI_H
#include "main.h"
#include "my_dsp.h"

void gui_frame(u16 color);
void gui_clear_frame(u16 color);
void gui_ref_axis(u16 color);
void show_result(float32_t freq, float32_t THD, float32_t max, float32_t min, float32_t pp_value, float32_t rms, WAVEFORM_TYPE wave);
void gui_pwm_show_result(u32 high, u32 cycle, float64_t duty_ratio);
//--------连线画图
void graph(u32 *buffer, u32 max,uint64_t size, float scale,u16 color);


#endif //HELLOWORLD_GUI_H
