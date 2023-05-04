//
// Created by Whisky on 2023/4/22.
//
#include "SignalProcess_fft.h"
#include "SignalProcessConfig.h"
#include "retarget.h"
#include "arm_const_structs.h"
#include "delay.h"

__unused void rfft(float32_t *input, u32 fftSize, u8 ifftFlag)
{
    arm_rfft_fast_instance_f32 S;
    arm_rfft_fast_init_f32(&S, fftSize);
    /* input 处理*/
    float32_t fft_input[fftSize];
    for(u32 i = 0; i < fftSize; i++)
    {
        fft_input[i] = input[i];
    }
    float32_t fft_output[fftSize];
    arm_rfft_fast_f32(&S, fft_input, fft_output, ifftFlag);
    float32_t fft_output_maxval[fftSize];
    arm_cmplx_mag_f32(fft_output, fft_output_maxval, fftSize);
    for(u32 i = 0; i < fftSize; i++)
    {
        printf("%lu,%f\n", i, fft_output_maxval[i]);
    }
}

void cfft(float32_t *input, u32 fftSize, u8 ifftFlag, u8 bitReverseFlag, float32_t *fft_output_maxval)
{
    /* input 处理*/
    float32_t fft_input[fftSize * 2];
    for(u32 i = 0; i < fftSize; i++)
    {
        fft_input[i * 2] = input[i];
        fft_input[i * 2 + 1] = 0;
    }

    if (fftSize == 2048)
        arm_cfft_f32(&arm_cfft_sR_f32_len2048, fft_input, ifftFlag, bitReverseFlag);
    else if (fftSize == 1024)
        arm_cfft_f32(&arm_cfft_sR_f32_len1024, fft_input, ifftFlag, bitReverseFlag);
    else if (fftSize == 512)
        arm_cfft_f32(&arm_cfft_sR_f32_len512, fft_input, ifftFlag, bitReverseFlag);
    else if (fftSize == 4096)
        arm_cfft_f32(&arm_cfft_sR_f32_len4096, fft_input, ifftFlag, bitReverseFlag);
    else if (fftSize == 256)
        arm_cfft_f32(&arm_cfft_sR_f32_len256, fft_input, ifftFlag, bitReverseFlag);
    else if (fftSize == 128)
        arm_cfft_f32(&arm_cfft_sR_f32_len128, fft_input, ifftFlag, bitReverseFlag);
    else if (fftSize == 64)
        arm_cfft_f32(&arm_cfft_sR_f32_len64, fft_input, ifftFlag, bitReverseFlag);
    else if (fftSize == 32)
        arm_cfft_f32(&arm_cfft_sR_f32_len32, fft_input, ifftFlag, bitReverseFlag);
    else if (fftSize == 16)
        arm_cfft_f32(&arm_cfft_sR_f32_len16, fft_input, ifftFlag, bitReverseFlag);
    else
    {
        printf("error : fft_size set error\r\n");
        return;
    }
    arm_cmplx_mag_f32(fft_input, fft_output_maxval, fftSize);
#ifdef FFT_DEBUG
    for(u32 i = 0; i < fftSize; i++)
    {
        printf("%lu,%f\n", i, fft_output_maxval[i]);
        delay_ms(1);
    }
#endif
}

float32_t fft_max_value = 0;

float32_t fft_get_freq(float32_t *fft_output_maxval, u32 fftSize)
{
    float32_t max_value = 0;
    u32 max_index;
// 寻找缓冲区中最大值及其位置
    for (u32 i = 1; i <  fftSize / 2; i++) {

        if (fabs(fft_output_maxval[i]) > max_value) {

            max_value = fabs(fft_output_maxval[i]);
            fft_max_value = max_value;
            max_index = i;
        }
    }
// 计算最大值所在的频率
    float32_t freq = (float32_t) ((max_index * (float32_t)((float32_t)(SAMPLERATE) / (float32_t)fftSize) ));
    return freq;
}

__unused void cfft_radix4(float32_t *input, u32 fftSize, u8 ifftFlag, u8 bitReverseFlag, float32_t *fft_output_maxval)
{
    arm_cfft_radix4_instance_f32 S;
    arm_cfft_radix4_init_f32(&S, fftSize, ifftFlag, bitReverseFlag);
    float32_t fft_input[fftSize * 2];
    for(u32 i = 0; i < fftSize; i++)
    {
        fft_input[i * 2] = input[i];
        fft_input[i * 2 + 1] = 0;
    }
    arm_cfft_radix4_f32(&S, fft_input);
    arm_cmplx_mag_f32(fft_input, fft_output_maxval, fftSize);
#ifdef FFT_DEBUG
    for(u32 i = 0; i < fftSize; i++)
    {
        printf("%lu,%f\n", i, fft_output_maxval[i]);
        delay_ms(1);
    }
#endif
}

float32_t fft_get_THD(float32_t *fft_output_maxval, u32 fftSize, u8 range)
{
    float32_t max_value = 0;
    u32 max_index;
    u32 max_index_3th;
    u32 max_index_5th;

// 寻找缓冲区中最大值及其位置
    for (u32 i = 1; i <  fftSize / 2; i++) {

        if (fabs(fft_output_maxval[i]) > max_value) {

            max_value = fabs(fft_output_maxval[i]);
            max_index = i;
        }
    }
// 计算最大值所在的频率
    max_index_3th = max_index * 3;
    max_index_5th = max_index * 5;

    float32_t sum_1th = 0;
    float32_t sum_3th = 0;
    float32_t sum_5th = 0;
// 能量泄漏范围为 range， 例如range = 5， 即将一个坐标周围即本身（-2到2）一共5个点求和

    // 计算3次谐波和5次谐波分量的幅度和
    for (int i = -range/2; i <= range/2; i++) {
        if (max_index + i >= 0 && max_index + i <= fftSize)
        {
            sum_1th += fft_output_maxval[max_index + i];
        }
        if (max_index_3th + i >= 0 && max_index_3th + i <= fftSize)
        {
            sum_3th += fft_output_maxval[max_index_3th + i];
        }
        if (max_index_5th + i >= 0 && max_index_5th + i <= fftSize)
        {
            sum_5th += fft_output_maxval[max_index_5th + i];
        }

    }

    // 计算总谐波失真度
    float32_t THD = (sqrt(pow(sum_3th, 2) + pow(sum_5th, 2)) / fabs(sum_1th)) * 100;

    return THD;

}

WAVEFORM_TYPE fft_recognize_wave(float32_t *fft_output_maxval, u32 fftSize, u8 range)
{
    float32_t max_value = 0;
    u32 max_index;
    u32 max_index_3th;

// 寻找缓冲区中最大值及其位置
    for (u32 i = 1; i <  fftSize / 2; i++) {

        if (fabs(fft_output_maxval[i]) > max_value) {

            max_value = fabs(fft_output_maxval[i]);
            max_index = i;
        }
    }
// 计算最大值所在的频率
    max_index_3th = max_index * 3;

    float32_t sum_1th = 0;
    float32_t sum_3th = 0;

// 能量泄漏范围为 range， 例如range = 5， 即将一个坐标周围即本身（-2到2）一共5个点求和
    // 计算3次谐波
    for (int i = -range/2; i <= range/2; i++) {
        if (max_index + i >= 0 && max_index + i <= fftSize)
        {
            sum_1th += fft_output_maxval[max_index + i];
        }
        if (max_index_3th + i >= 0 && max_index_3th + i <= fftSize)
        {
            sum_3th += fft_output_maxval[max_index_3th + i];
        }
    }
    float32_t k = sum_1th/sum_3th;

  // printf("k: %f\n",k);

    if (k < 4 && k > 2)         //方波的 基波幅度 是三次谐波幅度的 三倍。 实际测量在2~4之间浮动
        return WAVEFORM_SQUARE;

    else if (k < 12 && k > 6)       //三角波的基波幅度是三次谐波幅度的 九倍。  实际测量在 6 ~ 12 间浮动
        return WAVEFORM_TRIANGLE;

    else if (k > 13)                //正弦波理论上三次谐波幅度是0，k值会非常大
        return WAVEFORM_SINE;
    else
        return WAVEFORM_UNKNOWN;
}




