//
// Created by Whisky on 2023/4/30.
//

#ifndef HELLOWORLD_KEY_H
#define HELLOWORLD_KEY_H
#include "main.h"
typedef enum
{
    KEY_MODE_NO_CONTINUE = 0,
    KEY_MODE_CONTINUE = 1,
}KEY_MODE;


#define KEY0        HAL_GPIO_ReadPin(KEY0_GPIO_Port, KEY0_Pin)          /* 读取KEY0引脚 */
#define KEY1        HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin)          /* 读取KEY1引脚 */
#define WK_UP      HAL_GPIO_ReadPin(KEY_UP_GPIO_Port, KEY_UP_Pin)      /* 读取KEY_UP引脚 */


#define KEY0_PRES       1              /* KEY0按下 */
#define KEY1_PRES       2              /* KEY1按下 */
#define KEY_UP_PRES     4              /* KEY_UP按下 */

uint8_t key_scan(KEY_MODE mode);     /* 按键扫描函数 */

#endif //HELLOWORLD_KEY_H
