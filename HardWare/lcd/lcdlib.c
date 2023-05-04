//
// Created by Whisky on 2023/1/7.
//

#include "lcd.h"
#include "lcdfont.h"

f_args painter;

void _lcd_show_char(uint16_t x, uint16_t y, char chr, f_args in)
{
    /* 设置默认参数 */
    uint8_t size = in.size ? in.size : painter.size;
    uint16_t color = in.color ? in.color : painter.color;
    uint8_t mode = in.mode ? in.mode : painter.mode;

    uint8_t *pfont = NULL;
    uint8_t chr_cnt = (size/8+((size%8)?1:0))*(size/2) ;  /* 总字节数 */
    uint16_t y0 = y;
    chr = chr - ' ';    //字库是从' '开始存的

    switch(size)
    {
        case LCD_FONTSIZE_1206:
            pfont = (uint8_t *)asc2_1206[chr];  /* 调用1206字体 */
            break;

        case LCD_FONTSIZE_1608:
            pfont = (uint8_t *)asc2_1608[chr];  /* 调用1608字体 */
            break;

        case LCD_FONTSIZE_2412:
            pfont = (uint8_t *)asc2_2412[chr];  /* 调用2412字体 */
            break;

        case LCD_FONTSIZE_3216:
            pfont = (uint8_t *)asc2_3216[chr];  /* 调用3216字体 */
            break;

        default:
            return ;
    }
    for(uint8_t t = 0; t < chr_cnt; t++)
    {
        uint8_t temp = pfont[t];
        for(uint8_t t1 = 0; t1 < 8; t1++)   //处理8位数据
        {
            if(temp & 0x80){                //首位有效
                lcd_draw_point(x, y , color);
            }
            else if(mode == LCD_MODE_BACKFILLED)
            {
                lcd_draw_point(x, y , painter.back_color);
            }

            temp <<= 1;
            y++;
            if((y - y0) == size)
            {
                y = y0;
                x++;
                break;
            }
        }
    }
}
void _lcd_show_string(uint16_t x, uint16_t y, uint8_t *p)
{
    //默认参数
    uint16_t width = painter.width ? painter.width : (_lcddev.width - x);
    uint16_t height = painter.height ? painter.height : (_lcddev.height - y);

    uint8_t x0 = x;
    width += x;
    height += y;

    while ((*p <= '~') && (*p >= ' '))   //判断是不是非法字符!
    {
        if (x >= width)
        {
            x = x0;
            y += painter.size;
        }

        if (y >= height)break; //退出

        lcd_show_char(x, y, *p, .size = painter.size, .color = painter.color, .mode = painter.mode);
        x += painter.size / 2;
        p++;
    }
}

uint32_t _lcd_pow(uint8_t m, uint8_t n)
{
    uint32_t result = 1;

    while (n--)result *= m;

    return result;
}

void _lcd_show_num(uint16_t x, uint16_t y, uint32_t num, uint8_t len,  f_args in)
{
    //默认参数
    uint8_t size = in.size ? in.size : painter.size;
    uint16_t color = in.color ? in.color : painter.color;
    uint8_t mode = in.mode ? in.mode : painter.mode;

    uint8_t t, temp;
    uint8_t enshow = 0;

    for (t = 0; t < len; t++)
    {
        temp = (num / _lcd_pow(10, len - t - 1)) % 10;

        if (enshow == 0 && t < (len - 1))
        {
            if (temp == 0)
            {
                lcd_show_char(x + (size / 2)*t, y, ' ', .size=size, .mode = mode, .color = color);
                continue;
            }
            else enshow = 1;

        }
        lcd_show_char(x + (size / 2)*t, y, temp + '0', .size=size, .mode = mode, .color = color);
    }
}

void _lcd_show_xnum(uint16_t x, uint16_t y, uint32_t num, uint8_t len,  f_args in)
{
    uint8_t t, temp;
    uint8_t enshow = 0;

    //默认参数
    uint8_t size = in.size ? in.size : painter.size;
    uint16_t color = in.color ? in.color : painter.color;
    uint8_t mode = in.mode ? in.mode : painter.mode;

    for (t = 0; t < len; t++)       /* 按总显示位数循环 */
    {
        temp = (num / _lcd_pow(10, len - t - 1)) % 10;    /* 获取对应位的数字 */

        if (enshow == 0 && t < (len - 1))   /* 没有使能显示,且还有位要显示 */
        {
            if (temp == 0)
            {
                if (mode == LCD_MODE_NO_BACKFILLED)    /* 高位需要填充0 */
                {
                    lcd_show_char(x + (size / 2)*t, y, '0', .size = size, .mode = mode & 0x01,.color = color);  /* 用0占位 */
                }
                else
                {
                    lcd_show_char(x + (size / 2)*t, y, ' ', .size = size, .mode = mode & 0x01,.color =  color);  /* 用空格占位 */
                }

                continue;
            }
            else
            {
                enshow = 1;     /* 使能显示 */
            }

        }

        lcd_show_char(x + (size / 2)*t, y, temp + '0', .size=size, .mode = mode & 0x01, .color = color);
    }
}

/**
 * @brief       画水平线
 * @param       x0,y0: 起点坐标
 * @param       len  : 线长度
 * @param       color: 颜色
 * @retval      无
 */
void lcd_draw_hline(uint16_t x, uint16_t y, uint16_t len, uint16_t color)
{
    if ((len == 0) || (x > _lcddev.width) || (y > _lcddev.height))return;

    lcd_fill(x, y, x + len - 1, y, color);
}

/**
 * @brief       画圆
 * @param       x0,y0 : 圆中心坐标
 * @param       r     : 半径
 * @param       color : 圆的颜色
 * @retval      无
 */
void lcd_draw_circle(uint16_t x0, uint16_t y0, uint8_t r, uint16_t color)
{
    int a, b;
    int di;

    a = 0;
    b = r;
    di = 3 - (r << 1);       /* 判断下个点位置的标志 */

    while (a <= b)
    {
        lcd_draw_point(x0 + a, y0 - b, color);  /* 5 */
        lcd_draw_point(x0 + b, y0 - a, color);  /* 0 */
        lcd_draw_point(x0 + b, y0 + a, color);  /* 4 */
        lcd_draw_point(x0 + a, y0 + b, color);  /* 6 */
        lcd_draw_point(x0 - a, y0 + b, color);  /* 1 */
        lcd_draw_point(x0 - b, y0 + a, color);
        lcd_draw_point(x0 - a, y0 - b, color);  /* 2 */
        lcd_draw_point(x0 - b, y0 - a, color);  /* 7 */
        a++;

        /* 使用Bresenham算法画圆 */
        if (di < 0)
        {
            di += 4 * a + 6;
        }
        else
        {
            di += 10 + 4 * (a - b);
            b--;
        }
    }
}

/**
 * @brief       填充实心圆
 * @param       x,y  : 圆中心坐标
 * @param       r    : 半径
 * @param       color: 圆的颜色
 * @retval      无
 */
void lcd_draw_fill_circle(uint16_t x, uint16_t y, uint16_t r, uint16_t color)
{
    uint32_t i;
    uint32_t imax = ((uint32_t)r * 707) / 1000 + 1;
    uint32_t sqmax = (uint32_t)r * (uint32_t)r + (uint32_t)r / 2;
    uint32_t xr = r;

    lcd_draw_hline(x - r, y, 2 * r, color);

    for (i = 1; i <= imax; i++)
    {
        if ((i * i + xr * xr) > sqmax)
        {
            /* draw lines from outside */
            if (xr > imax)
            {
                lcd_draw_hline (x - i + 1, y + xr, 2 * (i - 1), color);
                lcd_draw_hline (x - i + 1, y - xr, 2 * (i - 1), color);
            }

            xr--;
        }

        /* draw lines from inside (center) */
        lcd_draw_hline(x - xr, y + i, 2 * xr, color);
        lcd_draw_hline(x - xr, y - i, 2 * xr, color);
    }
}


/**
 * @brief       画线
 * @param       x1,y1: 起点坐标
 * @param       x2,y2: 终点坐标
 * @param       color: 线的颜色
 * @retval      无
 */
void lcd_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
    uint16_t t;
    int xerr = 0, yerr = 0, delta_x, delta_y, distance;
    int incx, incy, row, col;
    delta_x = x2 - x1;      /* 计算坐标增量 */
    delta_y = y2 - y1;
    row = x1;
    col = y1;

    if (delta_x > 0)
    {
        incx = 1;       /* 设置单步方向 */
    }
    else if (delta_x == 0)
    {
        incx = 0;       /* 垂直线 */
    }
    else
    {
        incx = -1;
        delta_x = -delta_x;
    }

    if (delta_y > 0)
    {
        incy = 1;
    }
    else if (delta_y == 0)
    {
        incy = 0;       /* 水平线 */
    }
    else
    {
        incy = -1;
        delta_y = -delta_y;
    }

    if ( delta_x > delta_y)
    {
        distance = delta_x;  /* 选取基本增量坐标轴 */
    }
    else
    {
        distance = delta_y;
    }

    for (t = 0; t <= distance + 1; t++ )    /* 画线输出 */
    {
        lcd_draw_point(row, col, color);    /* 画点 */
        xerr += delta_x;
        yerr += delta_y;

        if (xerr > distance)
        {
            xerr -= distance;
            row += incx;
        }

        if (yerr > distance)
        {
            yerr -= distance;
            col += incy;
        }
    }
}

/**
 * @brief       画矩形
 * @param       x1,y1: 起点坐标
 * @param       x2,y2: 终点坐标
 * @param       color: 矩形的颜色
 * @retval      无
 */
void lcd_draw_rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
    lcd_draw_line(x1, y1, x2, y1, color);
    lcd_draw_line(x1, y1, x1, y2, color);
    lcd_draw_line(x1, y2, x2, y2, color);
    lcd_draw_line(x2, y1, x2, y2, color);
}

void lcd_show_string_config(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t size, char *p, uint16_t color, u8 mode)
        {
         uint8_t x0 = x;

        width += x;
        height += y;

        while ((*p <= '~') && (*p >= ' '))   /* 判断是不是非法字符! */
        {
            if (x >= width)
            {
                x = x0;
                y += size;
            }

            if (y >= height)
            {
                break;      /* 退出 */
            }

            lcd_show_char(x, y, *p, .size = size, .color = color, .mode = mode);
            x += size / 2;
            p++;
        }
        }


