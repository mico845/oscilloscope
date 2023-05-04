/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dac.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "fsmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#ifdef UART_RECEIVE
#include "Module_UART.h"
#endif

#include "retarget.h"
#include "delay.h"
#include "my_dsp.h"
#include "lcd.h"
#include "demos.h"
#include "key.h"
#include "gui.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
#ifdef UART_RECEIVE
uint8_t data_length;
void USER_UART_RxCpltCallback(void)
{
    data_length  = RX_STR_LEN - __HAL_DMA_GET_COUNTER(&DMA_RX_HANDLE);//计算收到字符长度
    UNUSED(data_length);
    HAL_UART_Transmit(&huart3, (uint8_t *)Rx_str, strlen((char *)Rx_str), 0xffff);
    HAL_UART_Transmit(&huart3, (uint8_t *)'\n', 1, 0xffff);
    u32 freq = atoi((char *)Rx_str);
    dac_out(freq);
    memset(Rx_str, 0, data_length);//初始化Rx_str
    data_length  = 0;
}
#endif
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

//#define SAMPLERATE (204800)

u8 pause = 0;

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_DMA_Init();
  MX_DAC_Init();
  MX_TIM8_Init();
  MX_ADC1_Init();
  MX_TIM2_Init();
  MX_USART3_UART_Init();
  MX_FSMC_Init();
  MX_TIM3_Init();
  MX_TIM9_Init();
  /* USER CODE BEGIN 2 */
    delay_init(168);
    RetargetInit(&huart1);
    lcd_init();
    painter.size = LCD_FONTSIZE_1608;
    lcd_clear();
    start_demos();
    painter.back_color= GRAY;
    painter.color = BROWN;
    painter.mode = LCD_MODE_NO_BACKFILLED;
    lcd_clear();
#ifdef UART_RECEIVE
    //ENABLE_IDLECallback();
    //UART_IDLE_RegisterCallback(USER_UART_RxCpltCallback);
    //HAL_UART_Receive_DMA(&huart3, (uint8_t*)Rx_str, RX_STR_LEN);
#endif
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    //------------- show
    gui_frame(CYAN);
    gui_clear_frame(LGRAY);

    //------------- DAC
    pwm_out(&htim3, TIM_CHANNEL_1, 1050);
    dac_set(WAVEFORM_SINE, 0, 3300, CHANNEL_2);
    dac_set(WAVEFORM_TRIANGLE, 0, 3300, CHANNEL_1);
    dac_out(1050);

    //--------------Delay
    delay_ms(1000);

    //-------------- ADC
    adc_start_auto(&hadc1, auto_samplerate);

    //-------------- PWM input

    char text[] = "[ Running ]";
    lcd_show_string_config((LCD_WIDTH - 80) - 25+7,  2 + 10, 8 * sizeof (text), 16, LCD_FONTSIZE_1206, text, GREEN, LCD_MODE_BACKFILLED);

    u8 key_value;

    while (1) {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

        //-----------PWM 检测
        pwm_input(&htim9, TIM_CHANNEL_1, 1000000, .fclk = 168000000);

        //-----------按钮调制
        key_value = key_scan(KEY_MODE_NO_CONTINUE);

        switch (key_value) {
            case KEY0_PRES : {
                delay_ms(20);
                if (auto_samplerate <= (409600) )
                    auto_samplerate *= 2;
                adc_stop(&hadc1);
                adc_start_auto(&hadc1, auto_samplerate);
                delay_ms(20);
            }break;
            case KEY1_PRES :
            {
                delay_ms(20);
                if (auto_samplerate >= (3200 / 2))
                    auto_samplerate /= 2;
                adc_stop(&hadc1);
                adc_start_auto(&hadc1, auto_samplerate);
                delay_ms(20);
            }break;
            case KEY_UP_PRES:
            {
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
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
