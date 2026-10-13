#ifndef IO_define_H
#define IO_define_H
#include "gd32f4xx.h"
#include "systick.h"
#include "arm_math.h"    
#include "PWM.h"
#include "ADC.h"
#include "USART.h"
#include "EXTI.h"
#include "GPIO.h"

#include "drv_usbd_int.h"
#include "drv_usb_hw.h"
#include "cdc_acm_core.h"
#include "Scope.h"


//ABC三相上下桥引脚及GPIO口
#define PWM_A_UP_PIN       GPIO_PIN_8
#define PWM_A_UP_Port      GPIOA
#define PWM_A_DOWN_PIN     GPIO_PIN_13
#define PWM_A_DOWN_Port 	 GPIOB

#define PWM_B_UP_PIN       GPIO_PIN_9
#define PWM_B_UP_Port	     GPIOA
#define PWM_B_DOWN_PIN     GPIO_PIN_14
#define PWM_B_DOWN_Port	   GPIOB

#define PWM_C_UP_PIN       GPIO_PIN_10
#define PWM_C_UP_Port			 GPIOA
#define PWM_C_DOWN_PIN     GPIO_PIN_15
#define PWM_C_DOWN_Port    GPIOB

#define PWM_BRKIN_Pin      GPIO_PIN_6
#define PWM_BRKIN_Port     GPIOA
//ABZ脉冲引脚及GPIO口 外部中断引脚输入
#define Encoder_Z_Pin              GPIO_PIN_2  
#define Encoder_Z_Port             GPIOB
#define Encoder_Z_EXTI_IRQn        EXTI2_IRQn
#define Encoder_Z_EXTI_IRQnHandler EXTI2_IRQHandler
#define Encoder_Z_EXTI_SOURCE      EXTI_SOURCE_GPIOB
#define Encoder_Z_EXTI_SOURCE_PIN  EXTI_SOURCE_PIN2
#define Encoder_Z_EXTI_LINE        EXTI_2
#define Encoder_A_Pin   GPIO_PIN_0
#define Encoder_A_Port  GPIOA
#define Encoder_B_Pin   GPIO_PIN_1
#define Encoder_B_Port  GPIOA

//Lcd
#define LCD_SCL_GPIO_Port 	GPIOA
#define	LCD_SCL_Pin					GPIO_PIN_5
#define LCD_SDA_GPIO_Port		GPIOB
#define	LCD_SDA_Pin					GPIO_PIN_5
#define	LCD_RES_GPIO_Port		GPIOB
#define	LCD_RES_Pin					GPIO_PIN_8
#define LCD_DC_GPIO_Port		GPIOB
#define LCD_DC_Pin					GPIO_PIN_9
#define LCD_CS_GPIO_Port		GPIOB
#define LCD_CS_Pin					GPIO_PIN_4
#define LCD_BLK_GPIO_Port		GPIOC
#define LCD_BLK_Pin					GPIO_PIN_15


//ABC两相电流输入引脚及GPIO
#define ADC_A_Pin    GPIO_PIN_6
#define ADC_A_Port   GPIOA


#define ADC_B_Pin    GPIO_PIN_7
#define ADC_B_Port   GPIOA
#define ADC_C_Pin 	 GPIO_PIN_4
#define ADC_C_Port   GPIOC

#define ADC_Vbus_Pin  GPIO_PIN_0
#define ADC_Vbus_Port GPIOC
//DRV芯片PWM使能及报警引脚
//#define Time_PWM_Pin       			GPIO_PIN_5
//#define Time_PWM_Port      			GPIOA
//#define Nouse_PWM_Pin       			GPIO_PIN_4
//#define Nouse_PWM_Port      			GPIOA


#define DISCHARGE_MOS_HPin       GPIO_PIN_9
#define DISCHARGE_MOS_HGPIO_Port GPIOC
#define DISCHARGE_MOS_LPin       GPIO_PIN_8
#define DISCHARGE_MOS_LGPIO_Port GPIOC

#define LED1_Pin            		  GPIO_PIN_13
#define LED1_GPIO_Port       		GPIOC

#define LED2_Pin            		  GPIO_PIN_14
#define LED2_GPIO_Port       		GPIOC

extern float memTemp[5];
extern void SendDataFix(void);
extern void Timer3Init(void);
extern void LcdTask(void);

#endif /* IO_define_H*/
