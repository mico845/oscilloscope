/*
 * ad8934.c
 *
 *  Created on: Nov 6, 2022
 *      Author: Whisky
 */

#ifndef SRC_AD9834_C_
#define SRC_AD9834_C_

#include "ad9834.h"

#define AD9834_Write_Bytes 16

void AD9834_WriteBytes(uint16_t data)
{
	uint8_t i = 0;

	AD9834_FSYNC_SET;
	AD9834_SCLK_SET;
	AD9834_FSYNC_RESET;

	for( i = 0 ; i < AD9834_Write_Bytes ; i ++ )
	{
		if(data & 0x8000)
		{
			// Write On 1
			AD9834_SDATA_SET;
		}
		else
		{
			// Write On 0
			AD9834_SDATA_RESET;
		}
		AD9834_SCLK_RESET;
		data <<= 1 ;
		AD9834_SCLK_SET;
	}

}

void AD9834_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	/* GPIO Ports Clock Enable */
    Port_CLK_ON();
	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(AD9834_Control_Port, AD9834_FSYNC_Pin|AD9834_SCLK_Pin|AD9834_SDATA_Pin|AD9834_RESET_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(AD9834_Fuction_Port, AD9834_FS_Pin|AD9834_PS_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pins : PAPin PAPin PAPin PAPin */
	GPIO_InitStruct.Pin = AD9834_FSYNC_Pin|AD9834_SCLK_Pin|AD9834_SDATA_Pin|AD9834_RESET_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;

	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	Port_CLK_ON();
	HAL_GPIO_Init(AD9834_Control_Port , &GPIO_InitStruct);

	/*Configure GPIO pins : PBPin PBPin */
	GPIO_InitStruct.Pin = AD9834_FS_Pin|AD9834_PS_Pin;

	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;

	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	Port_CLK_ON();
	HAL_GPIO_Init(AD9834_Fuction_Port, &GPIO_InitStruct);

	AD9834_WriteBytes(AD9834_RESET);
	AD9834_WriteBytes(0xC000);//相位寄存器写0
	//AD9834_WriteBytes(0x2038);
	//AD9834_WriteBytes(AD9834_RESET);

}

void AD9834_SetFreq(uint8_t Freq_Num, uint32_t Freq_Value)
{
	uint32_t FREQREG = (uint32_t)(Freq_Value*(268435456.0)/AD9834_MCLK);
	uint16_t FREQREG_14LSB = (uint16_t)FREQREG;
	uint16_t FREQREG_14MSB = (uint16_t)(FREQREG>>14);
	if(Freq_Num == FREQ_0)
	{
		FREQREG_14LSB &= ~(1U<<15);
		FREQREG_14LSB |= 1<<14;
		FREQREG_14MSB &= ~(1U<<15);
		FREQREG_14MSB |= 1<<14;
		//AD9834_WriteBytes(0x2000);
	}
	else
	{
		FREQREG_14LSB &= ~(1<<14);
		FREQREG_14LSB |= 1U<<15;
		FREQREG_14MSB &= ~(1<<14);
		FREQREG_14MSB |= 1U<<15;
		//AD9834_WriteBytes(0x0000);
	 }

	AD9834_WriteBytes(FREQREG_14LSB);
	AD9834_WriteBytes(FREQREG_14MSB);
}

void AD9834_SetWave(uint16_t initdata)
{

	AD9834_RESET_SET;
	AD9834_RESET_RESET;
	AD9834_WriteBytes(initdata);

}

#endif /* SRC_AD9834_C_ */
