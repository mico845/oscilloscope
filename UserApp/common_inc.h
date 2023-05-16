//
// Created by Whisky on 2023/5/16.
//

#ifndef HELLOWORLD_COMMON_INC_H
#define HELLOWORLD_COMMON_INC_H

#ifdef __cplusplus
extern "C" {
#endif
/*---------------------------- C Scope ---------------------------*/
#include "stdint-gcc.h"

#ifdef UART_RECEIVE
#include "Module_UART.h"
#endif

#include "usart.h"
#include "tim.h"
#include "adc.h"
#include "retarget.h"
#include "delay.h"
#include "my_dsp.h"
#include "lcd.h"
#include "demos.h"
#include "key.h"
#include "gui.h"

void Main();


#ifdef __cplusplus
}
/*---------------------------- C++ Scope ---------------------------*/
void Init();


#endif

#endif //HELLOWORLD_COMMON_INC_H
