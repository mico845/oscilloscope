//
// Created by Whisky on 2023/5/16.
//
#include "common_inc.h"

void Main()
{
    //------------- 初始化
    Init();
    //------------- show
    gui_frame(CYAN);
    gui_clear_frame(LGRAY);

    //------------- DAC
    pwm_out(&htim3, TIM_CHANNEL_1, 1050);
    dac_set(WAVEFORM_SINE, 0, 3300, CHANNEL_2);
    dac_set(WAVEFORM_TRIANGLE, 0, 3300, CHANNEL_1);
    dac_out(10000);

    //--------------Delay
    delay_ms(1000);

    //-------------- ADC
    adc_start_auto(&hadc1, auto_samplerate);

    //-------------- PWM input
    char text[] = "[ Running ]";
    lcd_show_string_config((LCD_WIDTH - 80) - 25+7,  2 + 10, 8 * sizeof (text), 16, LCD_FONTSIZE_1206, text, GREEN, LCD_MODE_BACKFILLED);


    u8 pause = 0;

    u8 key_value;
    KEY_MODE key_state = KEY_MODE_NO_CONTINUE;

    for (;;)
    {
        //-----------PWM
        pwm_input(&htim9, TIM_CHANNEL_1, 1000000, .fclk = 144000000);

        //-----------按钮调制
        key_value = key_scan(key_state);

        switch (key_value) {
            case KEY0_PRES : {
                delay_ms(20);
                key_state = KEY_MODE_CONTINUE;
                if (auto_samplerate <= (40000))
                {
                    auto_samplerate += 1000;
                }

                else if (auto_samplerate <= (2400000 - 10000))
                {
                    auto_samplerate += 10000;
                }

                adc_stop(&hadc1);
                adc_start_auto(&hadc1, auto_samplerate);
                delay_ms(20);
            }break;
            case KEY1_PRES :
            {
                delay_ms(20);
                key_state = KEY_MODE_CONTINUE;
                if (auto_samplerate >= 20000)
                {
                    auto_samplerate -= 10000;
                }

                else if (auto_samplerate > 1000)
                {
                    auto_samplerate -= 1000;

                }

                adc_stop(&hadc1);
                adc_start_auto(&hadc1, auto_samplerate);
                delay_ms(20);
            }break;
            case KEY_UP_PRES:
            {
                key_state = KEY_MODE_NO_CONTINUE;
                delay_ms(20);
                if (pause)
                {
                    pause = 0;

                    sprintf(text, "[ Running ]");

                    lcd_show_string_config((LCD_WIDTH - 80) - 25+7,  2 + 10, 8 * sizeof (text), 16, LCD_FONTSIZE_1206, text, GREEN, LCD_MODE_BACKFILLED);

                    adc_start_auto(&hadc1, auto_samplerate);
                }
                else
                {
                    pause = 1;

                    sprintf(text, "[ STOP ]   ");

                    lcd_show_string_config((LCD_WIDTH - 80) - 25+7,  2 + 10, 8 * sizeof (text), 16, LCD_FONTSIZE_1206, text, RED, LCD_MODE_BACKFILLED);

                }
                delay_ms(20);
            }break;
        }


        //-----------ADC 捕获处理
#if 1
        if (adc_dma_finished)
        {
            adc_dma_finished = false;
            adc_process();
            if (!pause)
                adc_start_auto(&hadc1, auto_samplerate);
        }

#endif
    }
}

