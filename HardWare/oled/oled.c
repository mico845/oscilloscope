//
// Created by Whisky on 2023/1/11.
//

#include "oled.h"
#include "myiic.h"

oled_args oled_painter;

//OLED的显存
//存放格式如下.
//[0]0 1 2 3 ... 127
//[1]0 1 2 3 ... 127
//[2]0 1 2 3 ... 127
//[3]0 1 2 3 ... 127
//[4]0 1 2 3 ... 127
//[5]0 1 2 3 ... 127
//[6]0 1 2 3 ... 127
//[7]0 1 2 3 ... 127
u8 OLED_GRAM[128][8];

//向SSD1306写入一个字节。
//dat:要写入的数据/命令
//cmd:数据/命令标志 0,表示命令;1,表示数据;
void oled_wr_byte(u8 dat,u8 cmd)
{
    iic_start();
    iic_send_byte(OLED_ADDRESS);
    iic_wait_ack();
    if(cmd == OLED_CMD)
        iic_send_byte(0x00);
    else if(cmd == OLED_DATA)
        iic_send_byte(0x40);
    iic_wait_ack();
    iic_send_byte(dat);
    iic_wait_ack();
    iic_stop();
}

void oled_refresh_gram(void)
{
    u8 i,n;
    for(i=0;i<8;i++)
    {
        oled_wr_byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
        oled_wr_byte (0x00,OLED_CMD);      //设置显示位置—列低地址
        oled_wr_byte (0x10,OLED_CMD);      //设置显示位置—列高地址
        for(n=0;n<128;n++)oled_wr_byte(OLED_GRAM[n][i],OLED_DATA);
    }
}

//开启OLED显示
void oled_display_on(void)
{
    oled_wr_byte(0X8D,OLED_CMD);  //SET DCDC命令
    oled_wr_byte(0X14,OLED_CMD);  //DCDC ON
    oled_wr_byte(0XAF,OLED_CMD);  //DISPLAY ON
}
//关闭OLED显示
void oled_display_off(void)
{
    oled_wr_byte(0X8D,OLED_CMD);  //SET DCDC命令
    oled_wr_byte(0X10,OLED_CMD);  //DCDC OFF
    oled_wr_byte(0XAE,OLED_CMD);  //DISPLAY OFF
}

void oled_clear(void)
{
    u8 i,n;
    for(i=0;i<8;i++)for(n=0;n<128;n++)OLED_GRAM[n][i]=0X00;
    oled_refresh_gram();//更新显示
}

//t:1 填充 0,清空
void oled_draw_point(u8 x,u8 y,u8 t)
{
    u8 pos,bx,temp=0;
    if(x>127||y>63)return;//超出范围了.
    pos=7-y/8;
    bx=y%8;
    temp=1<<(7-bx);
    if(t)OLED_GRAM[x][pos]|=temp;
    else OLED_GRAM[x][pos]&=~temp;

}

void oled_init(void)
{
    iic_init();

    oled_wr_byte(0xAE,OLED_CMD); //关闭显示
    oled_wr_byte(0xD5,OLED_CMD); //设置时钟分频因子,震荡频率
    oled_wr_byte(80,OLED_CMD);   //[3:0],分频因子;[7:4],震荡频率
    oled_wr_byte(0xA8,OLED_CMD); //设置驱动路数
    oled_wr_byte(0X3F,OLED_CMD); //默认0X3F(1/64)
    oled_wr_byte(0xD3,OLED_CMD); //设置显示偏移
    oled_wr_byte(0X00,OLED_CMD); //默认为0
    oled_wr_byte(0x40,OLED_CMD); //设置显示开始行 [5:0],行数.
    oled_wr_byte(0x8D,OLED_CMD); //电荷泵设置
    oled_wr_byte(0x14,OLED_CMD); //bit2，开启/关闭
    oled_wr_byte(0x20,OLED_CMD); //设置内存地址模式
    oled_wr_byte(0x02,OLED_CMD); //[1:0],00，列地址模式;01，行地址模式;10,页地址模式;默认10;
    oled_wr_byte(0xA1,OLED_CMD); //段重定义设置,bit0:0,0->0;1,0->127;
    oled_wr_byte(0xC0,OLED_CMD); //设置COM扫描方向;bit3:0,普通模式;1,重定义模式 COM[N-1]->COM0;N:驱动路数
    oled_wr_byte(0xDA,OLED_CMD); //设置COM硬件引脚配置
    oled_wr_byte(0x12,OLED_CMD); //[5:4]配置
    oled_wr_byte(0x81,OLED_CMD); //对比度设置
    oled_wr_byte(0xEF,OLED_CMD); //1~255;默认0X7F (亮度设置,越大越亮)
    oled_wr_byte(0xD9,OLED_CMD); //设置预充电周期
    oled_wr_byte(0xf1,OLED_CMD); //[3:0],PHASE 1;[7:4],PHASE 2;
    oled_wr_byte(0xDB,OLED_CMD); //设置VCOMH 电压倍率
    oled_wr_byte(0x30,OLED_CMD); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;
    oled_wr_byte(0xA4,OLED_CMD); //全局显示开启;bit0:1,开启;0,关闭;(白屏/黑屏)
    oled_wr_byte(0xA6,OLED_CMD); //设置显示方式;bit0:1,反相显示;0,正常显示
    oled_wr_byte(0xAF,OLED_CMD); //开启显示

    oled_clear();

    oled_painter.size = OLED_FONT_12;
    oled_painter.mode = OLED_MODE_FILLED;

}
