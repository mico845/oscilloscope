//
// Created by Whisky on 2023/4/5.
//

#include "matrix_key.h"
#include "delay.h"

#define R1_Pin GPIO_PIN_0
#define R1_GPIO_Port GPIOD
#define R2_Pin GPIO_PIN_1
#define R2_GPIO_Port GPIOD
#define R3_Pin GPIO_PIN_2
#define R3_GPIO_Port GPIOD
#define R4_Pin GPIO_PIN_3
#define R4_GPIO_Port GPIOD
#define C1_Pin GPIO_PIN_4
#define C1_GPIO_Port GPIOD
#define C2_Pin GPIO_PIN_5
#define C2_GPIO_Port GPIOD
#define C3_Pin GPIO_PIN_6
#define C3_GPIO_Port GPIOD
#define C4_Pin GPIO_PIN_7
#define C4_GPIO_Port GPIOD

#define ROW_NUM 4
#define COLUMN_NUM 4

GPIO_TypeDef* row_ports[ROW_NUM] = {R1_GPIO_Port, R2_GPIO_Port, R3_GPIO_Port, R4_GPIO_Port};
GPIO_PinState row_pins[ROW_NUM] = {R1_Pin, R2_Pin, R3_Pin, R4_Pin};
GPIO_TypeDef* column_ports[COLUMN_NUM] = {C1_GPIO_Port, C2_GPIO_Port, C3_GPIO_Port, C4_GPIO_Port};
uint16_t column_pins[COLUMN_NUM] = {C1_Pin, C2_Pin, C3_Pin, C4_Pin};

uint8_t keypad[ROW_NUM][COLUMN_NUM] = {
        {'1', '2', '3', 'A'},
        {'4', '5', '6', 'B'},
        {'7', '8', '9', 'C'},
        {'*', '0', '#', 'D'}
};

void keypad_init() {
    // 配置行为输出模式，列为输入模式
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    __HAL_RCC_GPIOD_CLK_ENABLE();
    //__HAL_RCC_GPIOC_CLK_ENABLE();
    for (int i = 0; i < ROW_NUM; i++) {
        GPIO_InitStruct.Pin = row_pins[i];
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(row_ports[i], &GPIO_InitStruct);
    }

    for (int i = 0; i < COLUMN_NUM; i++) {
        GPIO_InitStruct.Pin = column_pins[i];
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(column_ports[i], &GPIO_InitStruct);
    }
}

char keypad_scan() {
    char key = '\0';
    // 扫描行
    for (int i = 0; i < ROW_NUM; i++) {
        HAL_GPIO_WritePin(row_ports[i], row_pins[i], GPIO_PIN_RESET);
        // 扫描列
        for (int j = 0; j < COLUMN_NUM; j++) {
            if (HAL_GPIO_ReadPin(column_ports[j], column_pins[j]) == GPIO_PIN_RESET) {
                // 根据键盘矩阵查找键值
                delay_ms(20);
                while (HAL_GPIO_ReadPin(column_ports[j], column_pins[j]) == GPIO_PIN_RESET);
                delay_ms(20);
                key = keypad[i][j];
                break;
            }
        }
        HAL_GPIO_WritePin(row_ports[i], row_pins[i], GPIO_PIN_SET);
        if (key != '\0') {
            break;
        }
    }
    return key;
}

