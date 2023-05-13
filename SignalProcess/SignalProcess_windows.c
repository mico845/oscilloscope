//
// Created by Whisky on 2023/5/13.
//

#include "SignalProcess_windows.h"
#include "SignalProcessConfig.h"


//加窗
void windows(float32_t *buf, u32 fftSize)
{
    float32_t win[fftSize];
    arm_hamming_f32(win, fftSize);
    for (u32 i = 0; i < fftSize; ++i)
        buf[i] = buf[i] * win[i];
#ifdef WINDOWS_DEBUG
    for (u32 i = 0; i < fftSize; ++i)
        printf("%lu,%f\n", i, buf[i]);
#endif
}