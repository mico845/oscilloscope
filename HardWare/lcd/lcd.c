#include <argz.h>
//
// Created by Whisky on 2023/1/7.
//
#include "lcd.h"
//#include "retarget.h"
#include "delay.h"

/* 管理LCD重要参数 */
_lcd_dev _lcddev;

/* 写命令 */
void _lcd_wr_regno(uint16_t regval)
{
    regval = regval;
    LCD->LCD_REG = regval;
}

/* 写数据 */
void _lcd_wr_data(uint16_t data)
{
   data = data;            /* 使用-O2优化的时候,必须插入的延时 */
    LCD->LCD_RAM = data;
}

/* 写寄存器 */
void lcd_write_reg(uint16_t regno, uint16_t data)
{
    LCD->LCD_REG = regno;   /* 写入要写的寄存器序号 */
    LCD->LCD_RAM = data;    /* 写入数据 */
}

/* 读数据 */
uint16_t lcd_rd_data(void)
{
    volatile uint16_t ram;  /* 防止被优化 */
    ram = LCD->LCD_RAM;
    return ram;
}

uint16_t _lcd_r_regno(uint16_t lcd_reg)
{
    _lcd_wr_regno(lcd_reg);		//写入要读的寄存器序号
    delay_us(5);
    return lcd_rd_data();		//返回读到的值
}

/* 准备写GRAM */
void lcd_write_ram_prepare(void)
{
    _lcd_wr_regno(_lcddev.wramcmd);
}

/* 准备读GRAM */
void lcd_read_ram_prepare(void)
{
    _lcd_wr_regno(_lcddev.rramcmd);
}

/* 设置坐标 */
void lcd_set_cursor(uint16_t x, uint16_t y)
{
    _lcd_wr_regno(_lcddev.setxcmd);
    _lcd_wr_data(x >> 8);
    _lcd_wr_data(x & 0xFF);
    _lcd_wr_regno(_lcddev.setycmd);
    _lcd_wr_data(y >> 8);
    _lcd_wr_data(y & 0xFF);
}

void lcd_draw_point(uint16_t x, uint16_t y, uint16_t color)
{
    lcd_set_cursor(x, y);
    lcd_write_ram_prepare();
    _lcd_wr_data(color);
}

void lcd_draw_point_continue(uint16_t color)
{
    _lcd_wr_data(color);
}

__unused uint16_t lcd_read_point(uint16_t x, uint16_t y)
{
    uint16_t R , G , B;
    lcd_set_cursor(x, y);
    lcd_read_ram_prepare();
    R = lcd_rd_data();  //dummy
    R = lcd_rd_data();
    B = lcd_rd_data();
    G = R & 0xFF;
    return (((R >> 11) << 11) | ((G >> 2) << 5) | (B >> 11));
}

void lcd_display_dir(u8 dir);

void lcd_init(void)
{
    _lcddev.id = _lcd_r_regno(0x0000);
    if(_lcddev.id<0XFF||_lcddev.id==0XFFFF||_lcddev.id==0X9300)
    {
        _lcd_wr_regno(0xD3);
        _lcddev.id = lcd_rd_data();  /* dummy */
        _lcddev.id = lcd_rd_data();  /* 00 */
        _lcddev.id = lcd_rd_data();  /* 93 */
        _lcddev.id <<= 8;
        _lcddev.id |= lcd_rd_data();  /* 41 */
        if(_lcddev.id != 0X9341)		//非9341,
        {
            return;//退出初始化
        }
    }
    //printf("LCD id:%#x \r\n", _lcddev.id);
    /* 完成初始化数组序列 */

    _lcd_ex_ili9341_reginit();


#if 0
    /* 初始化LCD结构体 */
    _lcddev.width = 240;
    _lcddev.height = 320;
    _lcddev.setxcmd = 0x2A;
    _lcddev.setycmd = 0x2B;
    _lcddev.wramcmd = 0x2C;
    _lcddev.rramcmd = 0x2E;

    /* 设置终末位置 */
    _lcd_wr_regno(_lcddev.setxcmd);
    _lcd_wr_data(0);
    _lcd_wr_data(0);
    _lcd_wr_data((_lcddev.width - 1) >> 8);
    _lcd_wr_data((_lcddev.width - 1) & 0XFF);
    _lcd_wr_regno(_lcddev.setycmd);
    _lcd_wr_data(0);
    _lcd_wr_data(0);
    _lcd_wr_data((_lcddev.height - 1) >> 8);
    _lcd_wr_data((_lcddev.height - 1) & 0XFF);

    /* 设置扫描方向 */
    lcd_write_reg(0x36, 1 << 3);
#endif
    painter.color = GREEN;
    painter.back_color = BLACK;
    painter.size = LCD_FONTSIZE_1608;
    painter.mode = LCD_MODE_BACKFILLED;
    painter.chinese_size = LCD_CHINESE_FONT_16;
    painter.dir = dir_landscape_left;

    lcd_display_dir(painter.dir);	//选择--屏幕显示方式

    /* 点亮背光 */
    LCD_BL(1);

    lcd_clear();
}

void _lcd_clear(f_args in)
{
    uint16_t color = in.color? in.color: painter.back_color;
    uint32_t totalpoint = _lcddev.width;
    totalpoint *= _lcddev.height;                /* 得到总点数 */
    lcd_set_cursor(0, 0);                   /* 设置光标位置 */
    lcd_write_ram_prepare();                    /* 开始写入GRAM */
    for (uint32_t index = 0; index < totalpoint; index++)
    {
        _lcd_wr_data(color);
    }
}

//在指定区域内填充单个颜色
//(sx,sy),(ex,ey):填充矩形对角坐标
//区域大小为:(ex-sx+1)*(ey-sy+1)
//color:要填充的颜色
void lcd_fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t color)
{
    uint16_t i, j;
    uint16_t xlen;

    xlen = ex - sx + 1;

    for (i = sy; i <= ey; i++)
    {
        lcd_set_cursor(sx, i);       //设置光标位置
        lcd_write_ram_prepare();     //开始写入GRAM

        for (j = 0; j < xlen; j++)
        {
            *(uint16_t *)FSMC_ADDR_DATA = color;   //设置光标位置
        }
    }
}


/**
 * @brief       在指定区域内填充指定颜色块
 * @param       (sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex - sx + 1) * (ey - sy + 1)
 * @param       color: 要填充的颜色数组首地址
 * @retval      无
 */
void lcd_color_fill(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t *color)
{
    uint16_t height, width;
    uint16_t i, j;

    width = ex - sx + 1;            /* 得到填充的宽度 */
    height = ey - sy + 1;           /* 高度 */

    for (i = 0; i < height; i++)
    {
        lcd_set_cursor(sx, sy + i); /* 设置光标位置 */
        lcd_write_ram_prepare();    /* 开始写入GRAM */

        for (j = 0; j < width; j++)
        {
            LCD->LCD_RAM = color[i * width + j]; /* 写入数据 */
        }
    }
}


//扫描方向定义
#define L2R_U2D  0 //从左到右,从上到下
#define L2R_D2U  1 //从左到右,从下到上
#define R2L_U2D  2 //从右到左,从上到下
#define R2L_D2U  3 //从右到左,从下到上

#define U2D_L2R  4 //从上到下,从左到右
#define U2D_R2L  5 //从上到下,从右到左
#define D2U_L2R  6 //从下到上,从左到右
#define D2U_R2L  7 //从下到上,从右到左

void lcd_scan_dir(u8 dir)
{
    u16 regval=0;

//	u16 temp;

    if(_lcddev.id==0x9341)
    {
        switch(dir)
        {
            case L2R_U2D://从左到右,从上到下
                regval|=(0<<7)|(0<<6)|(0<<5);
                break;
            case L2R_D2U://从左到右,从下到上
                regval|=(1<<7)|(0<<6)|(0<<5);
                break;
            case R2L_U2D://从右到左,从上到下
                regval|=(0<<7)|(1<<6)|(0<<5);
                break;
            case R2L_D2U://从右到左,从下到上
                regval|=(1<<7)|(1<<6)|(0<<5);
                break;
            case U2D_L2R://从上到下,从左到右
                regval|=(0<<7)|(0<<6)|(1<<5);
                break;
            case U2D_R2L://从上到下,从右到左
                regval|=(0<<7)|(1<<6)|(1<<5);
                break;
            case D2U_L2R://从下到上,从左到右
                regval|=(1<<7)|(0<<6)|(1<<5);
                break;
            case D2U_R2L://从下到上,从右到左
                regval|=(1<<7)|(1<<6)|(1<<5);
                break;
        }

        lcd_write_reg(0x36,regval|0x08);//改变扫描方向命令  ---此处需要查看数据手册，确定RGB颜色交换位的配置

        //以下设置，为窗口参数设置，设置了全屏的显示范围

        _lcd_wr_regno(_lcddev.setxcmd);
        _lcd_wr_data(0);
        _lcd_wr_data(0);
        _lcd_wr_data((_lcddev.width-1)>>8);
        _lcd_wr_data((_lcddev.width-1)&0XFF);

        _lcd_wr_regno(_lcddev.setycmd);
        _lcd_wr_data(0);
        _lcd_wr_data(0);
        _lcd_wr_data((_lcddev.height-1)>>8);
        _lcd_wr_data((_lcddev.height-1)&0XFF);

    }
}

//设置LCD显示方向
//  dir:   0,竖屏  正
//         1,竖屏  反
//         2,横屏  左
//         3,横屏  右
void lcd_display_dir(u8 dir)
{
    u8 SCAN_DIR;

    if(dir==0)			     //竖屏  正
    {
        _lcddev.dir=0;	     //竖屏
        _lcddev.width=240;
        _lcddev.height=320;

        _lcddev.wramcmd=0X2C;
        _lcddev.setxcmd=0X2A;
        _lcddev.setycmd=0X2B;

        SCAN_DIR=L2R_U2D; //选择扫描方向

    }

    else if (dir==1)			 //横屏
    {
        _lcddev.dir=0;	     //竖屏
        _lcddev.width=240;
        _lcddev.height=320;

        _lcddev.wramcmd=0X2C;
        _lcddev.setxcmd=0X2A;
        _lcddev.setycmd=0X2B;

        SCAN_DIR=R2L_D2U; //选择扫描方向
    }


    else if (dir==2)			//横屏
    {
        _lcddev.dir=1;	     //横屏
        _lcddev.width=320;
        _lcddev.height=240;

        _lcddev.wramcmd=0X2C;
        _lcddev.setxcmd=0X2A;
        _lcddev.setycmd=0X2B;

        SCAN_DIR=U2D_R2L; //选择扫描方向

    }
    else if (dir==3)				  //横屏
    {
        _lcddev.dir=1;	        //横屏
        _lcddev.width=320;
        _lcddev.height=240;

        _lcddev.wramcmd=0X2C;
        _lcddev.setxcmd=0X2A;
        _lcddev.setycmd=0X2B;

        SCAN_DIR=D2U_L2R; //选择扫描方向

    }
    else //设置默认为竖屏--正
    {
        _lcddev.dir=0;	     //竖屏
        _lcddev.width=240;
        _lcddev.height=320;

        _lcddev.wramcmd=0X2C;
        _lcddev.setxcmd=0X2A;
        _lcddev.setycmd=0X2B;

        SCAN_DIR=L2R_U2D; //选择扫描方向

    }


    /////设置屏幕显示--扫描方向

    lcd_scan_dir(SCAN_DIR);	//设置屏幕显示--扫描方向
}


/**
 * @brief       设置窗口(对RGB屏无效), 并自动设置画点坐标到窗口左上角(sx,sy).
 * @param       sx,sy:窗口起始坐标(左上角)
 * @param       width,height:窗口宽度和高度,必须大于0!!
 *   @note      窗体大小:width*height.
 *
 * @retval      无
 */
void lcd_set_window(uint16_t sx, uint16_t sy, uint16_t width, uint16_t height)
{
    uint16_t twidth, theight;
    twidth = sx + width - 1;
    theight = sy + height - 1;
   /* 9341/5310/7789/1963横屏 等 设置窗口 */
    _lcd_wr_regno(_lcddev.setxcmd);
    _lcd_wr_data(sx >> 8);
    _lcd_wr_data(sx & 0xFF);
    _lcd_wr_data(twidth >> 8);
    _lcd_wr_data(twidth & 0xFF);
    _lcd_wr_regno( _lcddev.setycmd);
    _lcd_wr_data(sy >> 8);
    _lcd_wr_data(sy & 0xFF);
    _lcd_wr_data(theight >> 8);
    _lcd_wr_data(theight & 0xFF);
    lcd_write_ram_prepare();
}






