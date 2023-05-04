//
// Created by Whisky on 2023/4/29.
//

#include "SignalProcess_filter.h"
#include "fdacoefs.h"
#include "SignalProcessConfig.h"
#include "retarget.h"
#include "delay.h"
uint32_t blockSize = BLOCK_SIZE;
uint32_t numBlocks = FILTER_LENGTH_SAMPLES/BLOCK_SIZE; /* 需要调用arm_fir_f32的次数 */
static float32_t firStateF32[BLOCK_SIZE + NUM_TAPS - 1]; /* 状态缓存，大小numTaps + blockSize - 1*/

void filter_fir(float32_t *inputF32, float32_t *outputF32)
{
    arm_fir_instance_f32 S;
    arm_fir_init_f32(&S, BL, (float32_t *)&B[0], &firStateF32[0], blockSize);
    for(u32 i = 0; i < numBlocks; ++i)
    {
        arm_fir_f32(&S, inputF32 + (i * blockSize), outputF32 + (i * blockSize), blockSize);
    }
#ifdef FILTER_DEBUG
    /* 打印滤波后结果 */
    for(u32 i=0; i<FILTER_LENGTH_SAMPLES; i++)
    {
        printf("%lu,%f\n", i,outputF32[i]);
        delay_ms(1);
    }
#endif
}