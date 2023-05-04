/*
 *
 *
 *  Created on: Nov 19, 2022
 *      Author: Whisky
 */

/* 目录
 * 1. DMA IDLE中断空闲回调函数
 * 2. UART printf()函数
 *
 * */

/*DMA IDLE中断空闲回调函数使用方法
 * 注意事项：
1.使用前使能IDLE回调函数，在MX_USARTx_UART_Init();之后再调用使能函数!!!
2.用户增加的IRQ中断处理：
 在stm32f1xx_it.c中的USARTx_IRQHandler函数末调用USER_UART_IRQHandler(&UART_HANDLE);
3.注册回调函数即可使注册的函数在DMA空闲时中断
  函数调用实例
  void USER_UART_RxCpltCallback(void)
  {
	uint8_t data_length  = RX_STR_LEN - __HAL_DMA_GET_COUNTER(&DMA_RX_HANDLE);//计算收到字符长度
	UNUSED(data_length);
	HAL_UART_Transmit(&huart1, (uint8_t *)Rx_str, strlen(Rx_str), 0xffff);
	memset(Rx_str, 0, data_length);//初始化Rx_str
	data_length  = 0;
  }
  UART_IDLE_RegisterCallback(USER_UART_RxCpltCallback);

 * */



#ifndef INC_MODULE_UART_H_
#define INC_MODULE_UART_H_

#include "main.h"
#include "usart.h"
#include "dma.h"
#include "string.h"
#include "stdio.h"
#include <stdarg.h>
#include <stdlib.h>

#define UART_HANDLE huart3
#define UART_INSTANCE USART3
#define DMA_RX_HANDLE hdma_usart3_rx

extern DMA_HandleTypeDef DMA_RX_HANDLE;

#define RX_STR_LEN 255
extern u8 Rx_str[RX_STR_LEN];
extern uint8_t data_length;
/********************************************************/
/* 提前配置                                              */
/********************************************************/
//用户增加的IRQ中断处理
 /* 使用方法：在stm32f1xx_it.c中的USARTx_IRQHandler添加
 *例如下
  void USART2_IRQHandler(void)
  {
  	  HAL_UART_IRQHandler(&huart2);
  	  USER_UART_IRQHandler(&huart2);
  }
*/
void USER_UART_IRQHandler(UART_HandleTypeDef *huart);

//回调函数(中转)
void USER_UART_IDLECallback(UART_HandleTypeDef *huart);


/********************************************************/
/* 用户调用                                              */
/********************************************************/

// 使能IDLE回调函数	在MX_USARTx_UART_Init();之后再调用!!!
void ENABLE_IDLECallback(void);
//注册IDLE回调函数
void UART_IDLE_RegisterCallback(void (*fnptr)(void));

//IDLE回调函数清空
void UART_IDLE_UnRegisterCallback(void);

//UART printf函数
void UART_printf(UART_HandleTypeDef *UARTx, char *Data, ... );



#endif
