/*
 * demos.c
 *
 *  Created on: 03.10.2019
 *      Author: bolo
 */

//-------------------------
#include "main.h"
#include "demos.h"
#include "delay.h"
#include "string.h"

#define  LCD_ROW_NUM    LCD_WIDTH                    // Number of rows
#define  LCD_COL_NUM    LCD_HEIGHT                   // Number of columns

//-------------------------
extern u16  getscanline(void);
//-------------------------
#define sq_size 100
#define rect_size 100
#define t_wait 2

static void half_sec_delay(void);
static void wait_t(void);

static void demo_08(void);
static void demo_01(void);

//-------------------------
void start_demos(void)
{
    demo_08();
    lcd_clear();
    half_sec_delay();
    demo_01();
    half_sec_delay();
}

//
void demo_01(void)
{

    lcd_clear();
#define color_num 5
    u16 color[1 + color_num] = {0, YELLOW, BLUE, GREEN, RED, WHITE};
    for (int i = 1; i <= color_num; ++i) {
        lcd_draw_rectangle(10 * i,10 * i,LCD_WIDTH-10 * i,LCD_HEIGHT-10 * i,color[i]);
        half_sec_delay();
    }
    u8 text[]={"WE ARE TOTO TEAM"};
    painter.color = MAGENTA;
    lcd_show_string(LCD_WIDTH/2 - strlen((char *)text)*4,LCD_HEIGHT/2 - 8,text);
}



//------------------------------
//包围的线
void demo_08(void)
{

    lcd_clear(.color=COLOR_565_BLACK);
    int16_t x0,y0,x1,y1;
    x0 = 0;
    y0 = 0;
    x1 = 4;
    y1 = 239;

    while(y0 < 239)
    {
        lcd_draw_line(x0,y0,x1,y1,COLOR_565_BLUE);
        y0 = y0+6;
        x1 = x1+8;
         delay_ms(10);

    }

    x0 = 319;
    y0 = 239;
    x1 = 315;
    y1 = 0;

    while(y0 > 0)
    {
        lcd_draw_line(x0,y0,x1,y1,COLOR_565_BLUE);
        y0 = y0-6;
        x1 = x1-8;
         delay_ms(10);
    }

    x0 = 0;
    y0 = 239;
    x1 = 4;
    y1 = 0;

    while(y0 > 0)
    {
        lcd_draw_line(x0,y0,x1,y1,COLOR_565_RED);
        y0 = y0-6;
        x1 = x1+8;
         delay_ms(10);
    }

    x0 = 319;
    y0 = 0;
    x1 = 315;
    y1 = 239;

    while(y0 < 239)
    {
        lcd_draw_line(x0,y0,x1,y1,COLOR_565_RED);
        y0 = y0+6;
        x1 = x1-8;
         delay_ms(10);
    }

     delay_ms(1000);
}
//-----------------------------------
void half_sec_delay(void)
{
    delay_ms(500);
}
//----------
void wait_t(void)
{
     delay_ms(t_wait);
}
//-----------------------------------

//-----------------------------------




