//
// Created by Whisky on 2023/5/3.
//

#include "gui.h"
#include "lcd.h"


#define NUM_COLOR BRRED
#define BACK_COLOR LGRAYBLUE

void gui_frame(u16 color)
{
    lcd_draw_rectangle(9,9,(LCD_WIDTH - 80) - 25+1,LCD_HEIGHT * 0.4f + 10+1, color);
}

void gui_clear_frame(u16 color)
{
    //清除这块矩形区域的图像，好让下次重新绘制
    lcd_fill(10,10 ,(LCD_WIDTH - 80) - 25,LCD_HEIGHT * 0.4f + 10, color);
}

void gui_ref_axis(u16 color)
{
    u16 y_interval = (LCD_HEIGHT * 0.4f)/8;
    for (u16 y = 10; y < LCD_HEIGHT * 0.4f + 10; y += y_interval) {
        lcd_draw_hline(10 , y, (LCD_WIDTH - 80) - 25 - 10 , color);
    }
    u16 x_interval = ((LCD_WIDTH - 80) - 25 - 10) / 10;
    for (u16 x = 10; x < (LCD_WIDTH - 80) - 25; x += x_interval) {
        lcd_draw_line(x , 10, x, LCD_HEIGHT * 0.4f +10, color);
    }
}

void gui_pwm_show_result(u32 high, u32 cycle, float64_t duty_ratio)
{
    lcd_fill(10 + 14 * 8 -1, LCD_HEIGHT * 0.4f + 10+1 + 15 + 16 * 3 - 1,  10 + 14 * 8 + (7 + 8) * 6 + 1,   LCD_HEIGHT * 0.4f + 10+1 + 27 + 16 * 3 + 12 * 3, BACK_COLOR);
    lcd_show_string_config(10 + 14 * 8,  LCD_HEIGHT * 0.4f + 10+1 + 15 + 16 * 3, 6 *  (10), 12, LCD_FONTSIZE_1206, "PWM (PE5):",BROWN, LCD_MODE_NO_BACKFILLED);
    lcd_show_string_config(10 + 14 * 8,  LCD_HEIGHT * 0.4f + 10+1 + 27 + 16 * 3, 6 *  (7), 12, LCD_FONTSIZE_1206, "High  :",BROWN, LCD_MODE_NO_BACKFILLED);
    lcd_show_num(10 + 14 * 8 + 7 * 6, LCD_HEIGHT * 0.4f + 10+1 + 27 + 16 * 3, high, 8 , .size = LCD_FONTSIZE_1206, .color= NUM_COLOR);
    lcd_show_string_config(10 + 14 * 8,  LCD_HEIGHT * 0.4f + 10+1 + 27 + 16 * 3 + 12 * 1, 6 *  (7), 12, LCD_FONTSIZE_1206, "Period:",BROWN, LCD_MODE_NO_BACKFILLED);
    lcd_show_num(10 + 14 * 8 + 7 * 6, LCD_HEIGHT * 0.4f + 10+1 + 27 + 16 * 3 + 12 * 1, cycle, 8 , .size = LCD_FONTSIZE_1206, .color= NUM_COLOR);
    lcd_show_string_config(10 + 14 * 8,  LCD_HEIGHT * 0.4f + 10+1 + 27 + 16 * 3 + 12 * 2, 6 *  (7), 12, LCD_FONTSIZE_1206, "Duty  :",BROWN, LCD_MODE_NO_BACKFILLED);
    lcd_show_xnum(10 + 14 * 8 + 9 * 6, LCD_HEIGHT * 0.4f + 10+1 + 27 + 16 * 3 + 12 * 2, (u32)(duty_ratio * 100), 2 , .size = LCD_FONTSIZE_1206, .color= NUM_COLOR);
    lcd_show_char(10 + 14 * 8 + 11 * 6, LCD_HEIGHT * 0.4f + 10+1 + 27 + 16 * 3 + 12 * 2,'.' , .size = LCD_FONTSIZE_1206, .color= NUM_COLOR);
    lcd_show_xnum(10 + 14 * 8 + 12 * 6, LCD_HEIGHT * 0.4f + 10+1 + 27 + 16 * 3 + 12 * 2, ((u32)(duty_ratio * 1000 )% 100), 2 , .size = LCD_FONTSIZE_1206, .color= NUM_COLOR);
    lcd_show_char(10 + 14 * 8 + 14 * 6, LCD_HEIGHT * 0.4f + 10+1 + 27 + 16 * 3 + 12 * 2,'%' , .size = LCD_FONTSIZE_1206, .color= NUM_COLOR);

}

void show_result(float32_t freq, float32_t THD, float32_t max, float32_t min, float32_t pp_value, float32_t rms, WAVEFORM_TYPE wave)
{
    //printf("freq:=%lu, THD=%.2f%%\n", (u32)freq, THD);
    //painter.mode = LCD_MODE_BACKFILLED;

    //---------freq
    lcd_fill((LCD_WIDTH - 80) - 25+15 - 1  , 32 + 10 - 1 ,  (LCD_WIDTH - 80) - 25+15 + 8 * 8 + 1,  32 + 10 + 16 * 2 + 1 , BACK_COLOR);
    lcd_show_string((LCD_WIDTH - 80) - 25+15, 32 + 10, "Freq:");
    lcd_show_num((LCD_WIDTH - 80) - 25+15, 32 + 10 + 16, (u32)freq,8, .color = NUM_COLOR);

    //---------THD
    lcd_fill((LCD_WIDTH - 80) - 25+15 - 1 , 32 + 10+4+16*2 - 1,  (LCD_WIDTH - 80) - 25+15 + 8 * 8 + 1,   32 + 10+4 + 16 * 4 + 1, BACK_COLOR);
    lcd_show_string((LCD_WIDTH - 80) - 25+15, 32 + 10+4+16*2, "THD:");
    lcd_show_num((LCD_WIDTH - 80) - 25+15, 32 + 10+4 + 16 * 3, (u32)(THD),3 , .color = NUM_COLOR);
    lcd_show_char((LCD_WIDTH - 80) - 25+15 + 3*8, 32 + 10+4 + 16 * 3, '.', .color = NUM_COLOR);
    lcd_show_xnum((LCD_WIDTH - 80) - 25+15 + 4*8, 32 + 10+4 + 16 * 3, ((u32)(THD * 100) % 100), 2, .mode = LCD_MODE_NO_BACKFILLED, .color = NUM_COLOR);
    lcd_show_char((LCD_WIDTH - 80) - 25+15 + 6*8, 32 + 10+4 + 16 * 3, '%', .color = NUM_COLOR);

    lcd_fill(10 - 1 , LCD_HEIGHT * 0.4f + 10+1 + 15 - 1,  10 + 10 * 8 + 1,   LCD_HEIGHT * 0.4f + 10+1 + 15+ 16 * 4 + 1, BACK_COLOR);
    //---------max
    lcd_show_string(10, LCD_HEIGHT * 0.4f + 10+1 + 15, "max:");
    lcd_show_num(10 + 4 * 8, LCD_HEIGHT * 0.4f + 10+1 + 15, digital_to_analog_mv(max),4, .color = NUM_COLOR);
    lcd_show_string(10 + 8 * 8, LCD_HEIGHT * 0.4f + 10+1 + 15, "mv");

    //---------min
    lcd_show_string(10, LCD_HEIGHT * 0.4f + 10+1 + 15 + 16 * 1, "min:");
    lcd_show_num(10 + 4 * 8, LCD_HEIGHT * 0.4f + 10+1 + 15 + 16 * 1, digital_to_analog_mv(min),4 , .color = NUM_COLOR);
    lcd_show_string(10 + 8 * 8, LCD_HEIGHT * 0.4f + 10+1 + 15+ 16 * 1, "mv");

    //---------peak to peak
    lcd_show_string(10, LCD_HEIGHT * 0.4f + 10+1 + 15 + 16 * 2, "pp :");
    lcd_show_num(10 + 4 * 8, LCD_HEIGHT * 0.4f + 10+1 + 15 + 16 * 2, digital_to_analog_mv(pp_value),4 , .color = NUM_COLOR);
    lcd_show_string(10 + 8 * 8, LCD_HEIGHT * 0.4f + 10+1 + 15+ 16 * 2, "mv");

    //---------rms
    lcd_show_string(10, LCD_HEIGHT * 0.4f + 10+1 + 15 + 16 * 3, "rms:");
    lcd_show_num(10 + 4 * 8, LCD_HEIGHT * 0.4f + 10+1 + 15 + 16 * 3, digital_to_analog_mv(rms),4 , .color = NUM_COLOR);
    lcd_show_string(10 + 8 * 8, LCD_HEIGHT * 0.4f + 10+1 + 15+ 16 * 3, "mv");

    //---------recognize wave
    lcd_fill(10 + 14 * 8 -1, LCD_HEIGHT * 0.4f + 10+1 + 15 - 1,  10 + (14 + 8) * 8 + 1,   LCD_HEIGHT * 0.4f + 10+1 + 15 + 16 + 1, BACK_COLOR);
    if (wave == WAVEFORM_TRIANGLE)
        lcd_show_string(10 + 14 * 8, LCD_HEIGHT * 0.4f + 10+1 + 15, "TRIANGLE");
    else if (wave == WAVEFORM_SQUARE)
        lcd_show_string(10 + 14 * 8, LCD_HEIGHT * 0.4f + 10+1 + 15, "SQUARE  ");
    else if (wave == WAVEFORM_SINE)
        lcd_show_string(10 + 14 * 8, LCD_HEIGHT * 0.4f + 10+1 + 15, "SINE    ");
    else if (wave == WAVEFORM_UNKNOWN)
        lcd_show_string(10 + 14 * 8, LCD_HEIGHT * 0.4f + 10+1 + 15, "UNKNOWN ");

    //---------samplerate
    lcd_fill((LCD_WIDTH - 80) - 25 - 1 , LCD_HEIGHT * 0.4f + 10+1 + 15 - 1,  (LCD_WIDTH - 80) - 25 + 8 * 11 + 1,   LCD_HEIGHT * 0.4f + 10+1 + 15 + 16 * 2 + 1, BACK_COLOR);
    lcd_show_string((LCD_WIDTH - 80) - 25, LCD_HEIGHT * 0.4f + 10+1 + 15 , "samplerate:");
    lcd_show_num((LCD_WIDTH - 80) - 25 + 8 * 1 , LCD_HEIGHT * 0.4f + 10+1 + 15 + 16 * 1, (u32)auto_samplerate,8, .color = NUM_COLOR);

}

void graph(u32 *buffer, u32 max,uint64_t size, float scale,u16 color)
{

    // 计算y轴方向上的像素比例
    float y_scale = 0.4f * LCD_HEIGHT / max;
    // 计算x轴方向上的像素比例
    float x_scale = (LCD_WIDTH - 80) * scale / size;

    u32 x1, x2, y1, y2;

    for (int i = 0; i < size - 1; i ++) {
        // 计算坐标
        // 计算当前点和下一个点的坐标
        x1 = i * x_scale + 10;
        y1 = LCD_HEIGHT * 0.4f - (uint32_t)(buffer[i] * y_scale) + 10;
        x2 = (i + 1) * x_scale + 10;
        y2 = LCD_HEIGHT * 0.4f - (uint32_t)(buffer[i + 1] * y_scale) + 10;

        // 超出矩形范围则设为矩形右边缘
        if (x1 > (LCD_WIDTH - 80) - 25) x1 = (LCD_WIDTH - 80) - 25;
        if (x2 > (LCD_WIDTH - 80) - 25) x2 = (LCD_WIDTH - 80) - 25;

        // 绘制直线
        lcd_draw_line(x1, y1, x2, y2, color);
#if 0
        // 绘制点
        lcd_draw_point(x1, y1, color);
#endif
    }
}


