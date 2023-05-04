//
// Created by Whisky on 2023/1/8.
//

#ifndef HELLOWORLD_DELAY_H
#define HELLOWORLD_DELAY_H
#include "main.h"
void delay_init(uint16_t sysclk);    //单位为MHZ
void delay_ms(uint16_t nms);
void delay_us(uint32_t nus);
#endif //HELLOWORLD_DELAY_H
