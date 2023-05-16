/*
 *
 *
 *  Created on: Nov 19, 2022
 *      Author: Whisky
 */
#include <Module_UART.h>

u8 Rx_str[RX_STR_LEN];

void (*fnter_IDLECallback)(void);

/**
 * @brief	注册IDLE回调函数
 * @param	(*fnptr) 函数指针->IDLE回调函数
 * @retval 无
 */
void UART_IDLE_RegisterCallback(void (*fnptr)(void))
{
        fnter_IDLECallback = fnptr;
}

/**
 * @brief	IDLE回调函数清空
 * @param	无
 * @retval 无
 */
void UART_IDLE_UnRegisterCallback(void)
{
        fnter_IDLECallback = NULL;
}

/**
 * @brief	IDLE回调函数使能
 * @param	无
 * @retval 无
 */
void ENABLE_IDLECallback(void)
{
        //__HAL_UART_ENABLE_IT(&UART_HANDLE , UART_IT_RXNE);// 使能串口接收中断
        __HAL_UART_ENABLE_IT(&UART_HANDLE, UART_IT_IDLE); // 使能串口空闲中断
                                                          //__HAL_UART_CLEAR_IDLEFLAG(&UART_HANDLE);
}

void USER_UART_IDLECallback(UART_HandleTypeDef *huart)
{
        HAL_UART_DMAStop(huart); // 停止本次DMA传输

        fnter_IDLECallback();

        // HAL_UART_IRQHandler(huart);
        HAL_UART_Receive_DMA(huart, (uint8_t *)Rx_str, RX_STR_LEN); // 重新打开DMA接收
}

void USER_UART_IRQHandler(UART_HandleTypeDef *huart)
{
        if (huart->Instance == UART_INSTANCE)
        {
                // 判断是否是空闲中断
                if (__HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE) != 0)
                {
                        // 清除空闲中断标志（否则会一直不断进入中断）
                        __HAL_UART_CLEAR_IDLEFLAG(huart);
                        // 调用中断处理函数
                        USER_UART_IDLECallback(huart);
                }
        }
}
