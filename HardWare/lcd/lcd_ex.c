#include "lcd.h"
#include "delay.h"

/**
 * @brief       ILI9341寄存器初始化代码
 * @param       无
 * @retval      无
 */
void _lcd_ex_ili9341_reginit(void)
{
    _lcd_wr_regno(0xCF);
    _lcd_wr_data(0x00);
    _lcd_wr_data(0xC1);
    _lcd_wr_data(0X30);
    _lcd_wr_regno(0xED);
    _lcd_wr_data(0x64);
    _lcd_wr_data(0x03);
    _lcd_wr_data(0X12);
    _lcd_wr_data(0X81);
    _lcd_wr_regno(0xE8);
    _lcd_wr_data(0x85);
    _lcd_wr_data(0x10);
    _lcd_wr_data(0x7A);
    _lcd_wr_regno(0xCB);
    _lcd_wr_data(0x39);
    _lcd_wr_data(0x2C);
    _lcd_wr_data(0x00);
    _lcd_wr_data(0x34);
    _lcd_wr_data(0x02);
    _lcd_wr_regno(0xF7);
    _lcd_wr_data(0x20);
    _lcd_wr_regno(0xEA);
    _lcd_wr_data(0x00);
    _lcd_wr_data(0x00);
    _lcd_wr_regno(0xC0); /* Power control */
    _lcd_wr_data(0x1B);  /* VRH[5:0] */
    _lcd_wr_regno(0xC1); /* Power control */
    _lcd_wr_data(0x01);  /* SAP[2:0];BT[3:0] */
    _lcd_wr_regno(0xC5); /* VCM control */
    _lcd_wr_data(0x30);  /* 3F */
    _lcd_wr_data(0x30);  /* 3C */
    _lcd_wr_regno(0xC7); /* VCM control2 */
    _lcd_wr_data(0XB7);
    _lcd_wr_regno(0x36); /*  Memory Access Control */
    _lcd_wr_data(0x48);
    _lcd_wr_regno(0x3A);
    _lcd_wr_data(0x55);
    _lcd_wr_regno(0xB1);
    _lcd_wr_data(0x00);
    _lcd_wr_data(0x1A);
    _lcd_wr_regno(0xB6); /*  Display Function Control */
    _lcd_wr_data(0x0A);
    _lcd_wr_data(0xA2);
    _lcd_wr_regno(0xF2); /*  3Gamma Function Disable */
    _lcd_wr_data(0x00);
    _lcd_wr_regno(0x26); /* Gamma curve selected */
    _lcd_wr_data(0x01);
    _lcd_wr_regno(0xE0); /* Set Gamma */
    _lcd_wr_data(0x0F);
    _lcd_wr_data(0x2A);
    _lcd_wr_data(0x28);
    _lcd_wr_data(0x08);
    _lcd_wr_data(0x0E);
    _lcd_wr_data(0x08);
    _lcd_wr_data(0x54);
    _lcd_wr_data(0XA9);
    _lcd_wr_data(0x43);
    _lcd_wr_data(0x0A);
    _lcd_wr_data(0x0F);
    _lcd_wr_data(0x00);
    _lcd_wr_data(0x00);
    _lcd_wr_data(0x00);
    _lcd_wr_data(0x00);
    _lcd_wr_regno(0XE1);    /* Set Gamma */
    _lcd_wr_data(0x00);
    _lcd_wr_data(0x15);
    _lcd_wr_data(0x17);
    _lcd_wr_data(0x07);
    _lcd_wr_data(0x11);
    _lcd_wr_data(0x06);
    _lcd_wr_data(0x2B);
    _lcd_wr_data(0x56);
    _lcd_wr_data(0x3C);
    _lcd_wr_data(0x05);
    _lcd_wr_data(0x10);
    _lcd_wr_data(0x0F);
    _lcd_wr_data(0x3F);
    _lcd_wr_data(0x3F);
    _lcd_wr_data(0x0F);
    _lcd_wr_regno(0x2B);
    _lcd_wr_data(0x00);
    _lcd_wr_data(0x00);
    _lcd_wr_data(0x01);
    _lcd_wr_data(0x3f);
    _lcd_wr_regno(0x2A);
    _lcd_wr_data(0x00);
    _lcd_wr_data(0x00);
    _lcd_wr_data(0x00);
    _lcd_wr_data(0xef);
    _lcd_wr_regno(0x11); /* Exit Sleep */
    delay_ms(120);
    _lcd_wr_regno(0x29); /* display on */
 }
 
 
 






 