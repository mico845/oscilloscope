# oscilloscope

## 一个简易示波器
1. 波形显示
2. 采样率可调、可暂停/运行
3. 波形基本参数测量
4. 使用FFT频率测量、THD计算
5. 波形识别
6. PWM脉宽、周期、占空比测量

ADC引脚为 PA1
PWM测量引脚为 PE5

自检 DAC和定时器出三波，频率均为1050：
PA4 DAC 三角波
PA5 DAC 正弦波
PA6 TIM 方波
