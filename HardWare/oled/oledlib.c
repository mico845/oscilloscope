//
// Created by Whisky on 2023/1/11.
//
#include "oled.h"
#include "oledfont.h"

//x1,y1,x2,y2 填充区域的对角坐标
//确保x1<=x2;y1<=y2 0<=x1<=127 0<=y1<=63
//dot:0,清空;1,填充
void oled_fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot)
{
    u8 x,y;
    for(x=x1;x<=x2;x++)
    {
        for(y=y1;y<=y2;y++)oled_draw_point(x,y,dot);
    }
    oled_refresh_gram();//更新显示
}

//x:0~127
//y:0~63
void _oled_show_char(u8 x, u8 y, u8 chr,oled_args in)
{
    //默认参数
    u8 size = in.size ? in.size : oled_painter.size;
    u8 mode = in.mode ? in.mode : oled_painter.mode;


    u8 temp,t,t1;
    u8 y0=y;
    u8 csize=(size/8+((size%8)?1:0))*(size/2);		//得到字体一个字符对应点阵集所占的字节数
    chr=chr-' ';//得到偏移后的值
    for(t=0;t<csize;t++)
    {
        if(size==OLED_FONT_12)temp=oled_asc2_1206[chr][t]; 	 	//调用1206字体
        else if(size==OLED_FONT_16)temp=oled_asc2_1608[chr][t];	//调用1608字体
        else if(size==OLED_FONT_24)temp=oled_asc2_2412[chr][t];	//调用2412字体
        else return;								//没有的字库
        for(t1=0;t1<8;t1++)
        {
            if(temp&0x80)oled_draw_point(x,y,mode);
            else oled_draw_point(x,y,!mode);
            temp<<=1;
            y++;
            if((y-y0)==size)
            {
                y=y0;
                x++;
                break;
            }
        }
    }
}

//m^n函数
u32 my_pow(u8 m,u8 n)
{
    u32 result=1;
    while(n--)result*=m;
    return result;
}


//len :数字的位数
//num:数值(0~4294967295);
void _oled_show_num(u8 x,u8 y,u32 num,u8 len, oled_args in)
{
    //默认参数
    u8 size = in.size ? in.size : oled_painter.size;
    u8 mode = in.mode ? in.mode : oled_painter.mode;

    u8 t,temp;
    u8 enshow=0;
    for(t=0;t<len;t++)
    {
        temp=(num/my_pow(10,len-t-1))%10;
        if(enshow==0&&t<(len-1))
        {
            if(temp==0)
            {
                oled_show_char(x+(size/2)*t,y,' ',.size = size, .mode = mode);
                continue;
            }else enshow=1;

        }
        oled_show_char(x+(size/2)*t,y,temp+'0',.size = size, .mode = mode);
    }
}

void oled_show_string(u8 x,u8 y,const u8 *p)
{
    //默认参数
    u8 size =  oled_painter.size;
    u8 mode =  oled_painter.mode;

    while((*p<='~')&&(*p>=' '))//判断是不是非法字符!
    {
        if(x>(128-(size/2))){x=0;y+=size;}
        if(y>(64-size)){y=x=0;oled_clear();}
        oled_show_char(x,y,*p,.size = size, .mode = mode);
        x+=size/2;
        p++;
    }

}