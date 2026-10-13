#ifndef _ADC_H
#define _ADC_H

#include "gd32f4xx.h"
#include "systick.h"

#define RCU_GPIO_ADCin_01 RCU_GPIOA
#define RCU_GPIO_ADCin_02 RCU_GPIOC
#define RCU_Used_ADC1st   RCU_ADC0
#define Used_ADC1st       ADC0
#define ADC_A_channel     ADC_CHANNEL_6
#define ADC_B_channel     ADC_CHANNEL_7

#define ADC_Vbus_channel  ADC_CHANNEL_10
void ADC_Init(void);
void mid_ADC_value_Init(void);
extern uint16_t B_mid_valuel,A_mid_valuel;
#endif
