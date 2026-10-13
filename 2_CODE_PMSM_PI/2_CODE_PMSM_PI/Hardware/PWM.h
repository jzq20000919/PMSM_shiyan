#ifndef _PWM_H
#define _PWM_H


#include "gd32f4xx.h"
#include "systick.h"

#define PWM_TIMER			RCU_TIMER0		// 定时器时钟
#define PWM_PRES      2
#define PWM_PERI      4199
//#define DEADTIME      10


void PWM_Init(void); //PWM输出 ADC触发设置
#endif  /* BSP_PWM_H */

