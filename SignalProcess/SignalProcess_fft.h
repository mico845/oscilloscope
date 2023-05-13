//
// Created by Whisky on 2023/4/29.
//

#ifndef HELLOWORLD_SIGNALPROCESS_FFT_H
#define HELLOWORLD_SIGNALPROCESS_FFT_H
#include "SignalProcess.h"

extern float32_t fft_max_value;

//ifftFlag 0:正变换 1:逆变换
__unused void rfft(float32_t *input, u32 fftSize, u8 ifftFlag);

//ifftFlag 0:正变换 1:逆变换
// bitReverseFlag : 1:反转   0: 不反转
// FFT算法中的输入序列与输出序列在存储方式上存在差异，一般输入序列是按照时间域的顺序存储，而输出序列则是按照频域的顺序存储，因此在进行FFT计算后需要对输出序列进行位反转以获得正确的结果。
void cfft(float32_t *input, u32 fftSize, u8 ifftFlag, u8 bitReverseFlag, float32_t *fft_output_maxval);

float32_t fft_get_freq(float32_t *fft_output_maxval, u32 fftSize);
float32_t fft_get_THD(float32_t *fft_output_maxval, u32 fftSize, u8 range);
WAVEFORM_TYPE fft_recognize_wave(float32_t *fft_output_maxval, u32 fftSize, u8 range);

#endif //HELLOWORLD_SIGNALPROCESS_FFT_H
