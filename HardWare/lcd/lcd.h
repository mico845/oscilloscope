//
// Created by Whisky on 2023/1/7.
//

#ifndef HELLOWORLD_LCD_H
#define HELLOWORLD_LCD_H
#include "main.h"
#include "lcd_color.h"

/* LCD_BL背光引脚 */
#define LCD_BL_GPIO_PORT                    LCD_BL_GPIO_Port
#define LCD_BL_GPIO_PIN                     LCD_BL_Pin
#define LCD_BL_GPIO_CLK_ENABLE()            do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)

/* LCD重要参数集 */
typedef struct
{
    uint16_t width;
    uint16_t height;
    uint16_t id;
    uint8_t dir;        /* 方向   0-竖屏 1-横屏*/
    uint16_t wramcmd;   /* gram指令 w */
    uint16_t rramcmd;   /* gram指令 r */
    uint16_t setxcmd;   /* 设置x坐标 */
    uint16_t setycmd;   /* 设置y坐标 */
} _lcd_dev;

extern _lcd_dev _lcddev;

#define LCD_HEIGHT (_lcddev.height)
#define LCD_WIDTH (_lcddev.width)

/* LCD背光控制 */
#define LCD_BL(x)       LCD_BL_GPIO_PORT->BSRR = LCD_BL_GPIO_PIN << (16 * (!x))

/* FSMC 硬件选择 */
#define LCD_FSMC_NEX         1              /* 使用FSMC_NE1接LCD_CS,取值范围只能是: 1~4 */
#define LCD_FSMC_AX          18              /* 使用FSMC_A18接LCD_RS,取值范围是: 0 ~ 25 */

typedef struct
{
    volatile u16 LCD_REG;
    volatile u16 LCD_RAM;
} LCD_TypeDef;

#define  LCD_BASE        ((uint32_t)(0x60000000 | 0x0007FFFE))// FSMC_NE1, A18
#define  LCD             ((LCD_TypeDef *) LCD_BASE)

/* LCD_BASE = (0X6000 0000 + (0X400 0000 * (x - 1))) | (2^y * 2 - 2) */
#define FSMC_ADDR_DATA          ((uint32_t) 0x60080000) //A18置为1    CS = 1
#define FSMC_ADDR_CMD           ((uint32_t) 0x60000000) //A18置为0    CS = 0

#define LCD_CHINESE_FONT_12 12
#define LCD_CHINESE_FONT_16 16
#define LCD_CHINESE_FONT_24 24

#define LCD_FONTSIZE_1206 12
#define LCD_FONTSIZE_1608 16
#define LCD_FONTSIZE_2412 24
#define LCD_FONTSIZE_3216 32

#define LCD_MODE_BACKFILLED 1
#define LCD_MODE_NO_BACKFILLED 0

typedef  enum{
    dir_vertical_forward = 0,
    dir_vertical_reverse = 1,
    dir_landscape_left = 2,
    dir_landscape_right = 3
} lcd_dir;

typedef struct {
    uint16_t color;
    uint8_t size;
    uint8_t mode;
    uint16_t width;     //string
    uint16_t height;    //string
    uint16_t back_color;
    uint16_t chinese_size;
    lcd_dir dir;
} f_args;
extern f_args painter;

//To User:
void lcd_init(void);
void lcd_draw_point(uint16_t x, uint16_t y, uint16_t color);
void lcd_draw_point_continue(uint16_t color);//搭配lcd_set_window

uint16_t lcd_read_point(uint16_t x, uint16_t y);

#define lcd_clear(...)                  _lcd_clear((f_args){__VA_ARGS__})
#define lcd_show_char(x, y, chr,...)    _lcd_show_char(x, y, chr, (f_args){__VA_ARGS__})
#define lcd_show_num(x, y, num, len,...)    _lcd_show_num(x, y, num, len, (f_args){__VA_ARGS__})
#define lcd_show_xnum(x, y, num, len,...)    _lcd_show_xnum(x, y, num, len, (f_args){__VA_ARGS__})
#define lcd_show_string(x, y, p) _lcd_show_string(x, y, ((uint8_t *)p))
void lcd_show_string_config(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t size, char *p, uint16_t color, u8 mode);

//draw lib:
void lcd_fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t color);
void lcd_color_fill(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t *color);  /* 彩色填充 */
void lcd_draw_hline(uint16_t x, uint16_t y, uint16_t len, uint16_t color);  /* 画水平线 */
void lcd_draw_circle(uint16_t x0, uint16_t y0, uint8_t r, uint16_t color);
void lcd_draw_fill_circle(uint16_t x, uint16_t y, uint16_t r, uint16_t color);
void lcd_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);     /* 画直线 */
void lcd_draw_rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);/* 画矩形 */
void lcd_set_window(uint16_t sx, uint16_t sy, uint16_t width, uint16_t height);

//To Me:
void _lcd_clear(f_args in);
void _lcd_show_char(uint16_t x, uint16_t y, char chr, f_args in);
void _lcd_show_num(uint16_t x, uint16_t y, uint32_t num, uint8_t len,  f_args in);
void _lcd_show_xnum(uint16_t x, uint16_t y, uint32_t num, uint8_t len,  f_args in);
void _lcd_show_string(uint16_t x, uint16_t y, uint8_t *p);

void _lcd_wr_data (uint16_t data);
void _lcd_wr_regno(uint16_t reg);
void _lcd_ex_ili9341_reginit(void);

#endif //HELLOWORLD_LCD_H
