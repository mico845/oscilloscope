//
// Created by Whisky on 2023/1/11.
//

#ifndef HELLOWORLD_OLED_H
#define HELLOWORLD_OLED_H
#include "main.h"

#define OLED_ADDRESS 0x78

#define OLED_CMD  	0		//写命令
#define OLED_DATA 	1		//写数据

#define OLED_FONT_12  12
#define OLED_FONT_16  16
#define OLED_FONT_24  24

#define OLED_MODE_FILLED    1
#define OLED_MODE_NO_FILLED 0

typedef struct {
    u8 size;
    u8 mode;
} oled_args;

extern oled_args oled_painter;

#define oled_show_char(x, y, chr, ...) _oled_show_char(x, y, chr, (oled_args){__VA_ARGS__})
#define oled_show_num(x, y, num, len, ...) _oled_show_num(x, y, num, len, (oled_args){__VA_ARGS__})

void oled_init(void);
void oled_draw_point(u8 x,u8 y,u8 t);
void oled_refresh_gram();
void oled_clear(void);
void oled_display_on(void);
void oled_display_off(void);

void oled_fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void _oled_show_char(u8 x, u8 y, u8 chr, oled_args in);
void _oled_show_num(u8 x,u8 y,u32 num,u8 len, oled_args in);
void oled_show_string(u8 x,u8 y,const u8 *p);
#endif //HELLOWORLD_OLED_H
