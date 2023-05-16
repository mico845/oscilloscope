//
// Created by Whisky on 2023/5/16.
//

#include "common_inc.h"

#ifdef UART_RECEIVE
uint8_t data_length;
void USER_UART_RxCpltCallback(void)
{
    data_length  = RX_STR_LEN - __HAL_DMA_GET_COUNTER(&DMA_RX_HANDLE);//计算收到字符长度
    UNUSED(data_length);
    HAL_UART_Transmit(&huart3, (uint8_t *)Rx_str, strlen((char *)Rx_str), 0xffff);
    HAL_UART_Transmit(&huart3, (uint8_t *)'\n', 1, 0xffff);
    u32 freq = atoi((char *)Rx_str);
    dac_out(freq);
    memset(Rx_str, 0, data_length);//初始化Rx_str
    data_length  = 0;
}
#endif
