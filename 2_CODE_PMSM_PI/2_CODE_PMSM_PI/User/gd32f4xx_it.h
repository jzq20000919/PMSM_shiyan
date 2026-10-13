/*!
    \file    gd32f4xx_it.h
    \brief   the header file of the ISR

    \version 2016-08-15, V1.0.0, firmware for GD32F4xx
    \version 2018-12-12, V2.0.0, firmware for GD32F4xx
    \version 2020-09-30, V2.1.0, firmware for GD32F4xx
    \version 2022-03-09, V3.0.0, firmware for GD32F4xx
*/

/*
    Copyright (c) 2022, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors
       may be used to endorse or promote products derived from this software without
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.
*/

#ifndef GD32F4XX_IT_H
#define GD32F4XX_IT_H
#include "gd32f4xx.h"
#include "IO_define.h"


#define MOS_ON	  gpio_bit_set(DISCHARGE_MOS_HGPIO_Port,DISCHARGE_MOS_HPin); gpio_bit_reset(DISCHARGE_MOS_LGPIO_Port,DISCHARGE_MOS_LPin);											  
#define MOS_OFF	  gpio_bit_reset(DISCHARGE_MOS_HGPIO_Port,DISCHARGE_MOS_HPin);

#define LED1_ON   gpio_bit_set(LED1_GPIO_Port,LED1_Pin);
#define LED1_OFF  gpio_bit_reset(LED1_GPIO_Port,LED1_Pin);

#define LED2_ON   gpio_bit_set(LED2_GPIO_Port,LED2_Pin);
#define LED2_OFF  gpio_bit_reset(LED2_GPIO_Port,LED2_Pin);

#define Time_test_on   gpio_bit_set(Time_PWM_Port,Time_PWM_Pin);
#define Time_test_off  gpio_bit_reset(Time_PWM_Port,Time_PWM_Pin);

#define Nouse_pin_on   gpio_bit_set(Nouse_PWM_Port,Nouse_PWM_Pin);
#define Nouse_pin_off  gpio_bit_reset(Nouse_PWM_Port,Nouse_PWM_Pin);

#define ENCODER_Pulse_ROUND   10000    //转一圈编码器产生10000个脉冲
#define TIMER6_Time   0.01f       //定时器6中断时间，为10ms

typedef struct
{
	float Ref_Iq;
	float Ref_RPM;
	float I_A;
	float I_B;
	float I_C;
	float V_Bus;
	float encoder_theta;
	float Real_RPM;
	int16_t encoder_count;
} Control_input;


/* function declarations */
/* this function handles NMI exception */
void NMI_Handler(void);
/* this function handles HardFault exception */
void HardFault_Handler(void);
/* this function handles MemManage exception */
void MemManage_Handler(void);
/* this function handles BusFault exception */
void BusFault_Handler(void);
/* this function handles UsageFault exception */
void UsageFault_Handler(void);
/* this function handles SVC exception */
void SVC_Handler(void);
/* this function handles DebugMon exception */
void DebugMon_Handler(void);
/* this function handles PendSV exception */
void PendSV_Handler(void);
/* this function handles SysTick exception */
void SysTick_Handler(void);

void ADC_IRQHandler(void);
#endif /* GD32F4XX_IT_H */
