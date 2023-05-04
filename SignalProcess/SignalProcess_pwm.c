//
// Created by Whisky on 2023/5/4.
//

#include "SignalProcess_pwm.h"
#include "SignalProcess.h"
#include "SignalProcessConfig.h"
#include "retarget.h"
#include "delay.h"
#include "gui.h"

u32 pwm_channel = TIM_CHANNEL_1;

void _pwmFreAdjust(TIM_HandleTypeDef *htim, uint64_t f_out, dsp_args in)
{
    uint32_t fclk = in.fclk ? in.fclk : 84000000;
    u16 arr, psc;
    calc_tim_arr_psc(fclk, f_out, &arr, &psc);
    htim->Instance->ARR = arr;
    htim->Instance->PSC = psc;
    htim->Instance->CCR1 = arr/2 - 1;
}

void _pwmFreAdjust_psc(TIM_HandleTypeDef *htim, uint64_t f_out, dsp_args in)
{
    uint32_t fclk = in.fclk ? in.fclk : 168000000;
    float _psc = (float )fclk/ (float )f_out;
    htim->Instance->PSC = (u16)_psc + 1;
    htim->Instance->ARR = 0XFFFF;
}


//方波产生
void _pwm_out(TIM_HandleTypeDef *htim, u32 channel,uint64_t f_out, dsp_args in)
{
    _pwmFreAdjust(htim, f_out, in);
    HAL_TIM_PWM_Start(htim,channel);
}


__IO uint32_t TIM_TIMEOUT_COUNT = 0;			///< 定时器定时溢出计数
uint32_t TIM_CAPTURE_BUF[3]   = {0, 0, 0};		///< 分别存储上升沿计数、下降沿计数、下个上升沿计数
__IO uint8_t TIM_CAPTURE_STA = 0;			///< 状态标记


/// 定时器时间溢出回调函数
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == PWM_TIM.Instance)
    {
        TIM_TIMEOUT_COUNT++;										// 溢出次数计数
    }
}

///< 输入捕获回调函数
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == PWM_TIM.Instance)
    {
        switch (TIM_CAPTURE_STA)
        {
            case 1:
            {
 //               printf("准备捕获下降沿...\r\n");
                TIM_CAPTURE_BUF[0] = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1) + TIM_TIMEOUT_COUNT * 0xFFFF;
                __HAL_TIM_SET_CAPTUREPOLARITY(htim, pwm_channel, TIM_INPUTCHANNELPOLARITY_FALLING);					// 设置为下降沿触发
                TIM_CAPTURE_STA++;
                break;
            }
            case 2:
            {
//                printf("准备捕获下个上升沿...\r\n");
                TIM_CAPTURE_BUF[1] = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1) + TIM_TIMEOUT_COUNT * 0xFFFF;
                __HAL_TIM_SET_CAPTUREPOLARITY(htim, pwm_channel, TIM_INPUTCHANNELPOLARITY_RISING);					// 设置为上升沿触发
                TIM_CAPTURE_STA++;
                break;
            }
            case 3:
            {
   //             printf("捕获结束...\r\n");
  //              printf("# end ----------------------------------------------------\r\n");
                TIM_CAPTURE_BUF[2] = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1) + TIM_TIMEOUT_COUNT * 0xFFFF;
                HAL_TIM_IC_Stop_IT(htim, pwm_channel);									// 停止捕获
                HAL_TIM_Base_Stop_IT(htim);												// 停止定时器更新中断
                TIM_CAPTURE_STA++;
                break;
            }
            default:
                break;
        }
    }
}


void _pwm_input(TIM_HandleTypeDef *htim, u32 channel,uint64_t f_s, dsp_args in)
{
    switch (TIM_CAPTURE_STA)
    {
        case 0:
        {
            pwm_channel = channel;
            _pwmFreAdjust_psc(htim, f_s, in);
 //           printf("# start ----------------------------------------------------\r\n");
 //           printf("准备捕获下个上升沿...\r\n");
            TIM_TIMEOUT_COUNT = 0;
            __HAL_TIM_SET_COUNTER(htim, 0);											// 清除定时器2现有计数
            memset(TIM_CAPTURE_BUF, 0, sizeof(TIM_CAPTURE_BUF));						        // 清除捕获计数
            __HAL_TIM_SET_CAPTUREPOLARITY(htim, pwm_channel, TIM_INPUTCHANNELPOLARITY_RISING);	// 设置为上升沿触发
            HAL_TIM_Base_Start_IT(htim);												// 启动定时器更新中断
            HAL_TIM_IC_Start_IT(htim, pwm_channel);									// 启动捕获中断
            TIM_CAPTURE_STA++;
            break;
        }

        case 4:
        {
            uint32_t high  = TIM_CAPTURE_BUF[1] - TIM_CAPTURE_BUF[0];
            uint32_t cycle = TIM_CAPTURE_BUF[2] - TIM_CAPTURE_BUF[0];
            float32_t duty_ratio = (float32_t)high/(float32_t)cycle;
#if 0
            //float32_t frq = 1.0 / (((float)cycle) / 1000000.0);
#endif
            gui_pwm_show_result(high, cycle, duty_ratio);

#ifdef PWM_DEBUG
            printf("\r\n\r\n");
            printf("################################# START #########################################\r\n");
            printf("High:       %lu us\r\n", high );
            printf("Period:     %lu us\r\n", cycle );
            printf("Freq:       %f  Hz\r\n", frq);
            printf("################################## END ##########################################\r\n\r\n");
            delay_ms(1000);
#endif
            TIM_CAPTURE_STA = 0;
            break;
        }

        default:
            break;
    }

}
