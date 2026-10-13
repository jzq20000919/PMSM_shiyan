/********************************请叫我阿特_24的QQ：335959305****************************
***********************************************V：ZL_five_eight******************************/
#include "IO_define.h"
#include "Algorithm.h" //引用此头文件用来显示变量，注意算法部分的变量需要用结构体
#include "Chart.h"
#include "gd32f4xx_it.h"
#include "PWM.h"
#include "ADC.h"
#include "lcd.h"
#include "lcd_init.h"

float count;
usb_core_driver cdc_acm;
float memTemp[5];
float Iq_REF,PLL_omega,Iq_OBE;
int32_t Iqref,Iqreal,Vin,RPM;
uint32_t iqref,iqreal,rpm;
extern StrCmd SAppCmd;
uint8_t flagDone = 0;

int main(void)
{
    systick_config();  // 滴答定时器初始化
		
		usb_gpio_config();
		usb_rcu_config();
		usb_timer_init();
		usbd_init (&cdc_acm,USB_CORE_ENUM_FS,&cdc_desc,&cdc_class);
		usb_intr_config();
		
	  mid_ADC_value_Init();//上电自动检测ADC中值
	  PWM_Init();  //PWM相关引脚初始化
	  ADC_Init();  //ADC相关引脚初始化
    Encoder_EXIT_Init();  //AB相脉冲信号计数功能初始化，正交编码器模式2
	  Encoder_Z_Init();  //Z相信号定位，外部中断初始化
		Timer6_Init();
	
//	timer_counter_value_config( TIMER1 ,  65536);
	
	  GPIO_Init();   //使能引脚初始化
		
		LCD_Init();
		LCD_Fill(0,0,LCD_W,LCD_H,BLACK);
	
	  Algorithm_initialize(); 
	  Chart_initialize();
//		Timer3Init();
		
		flagDone = 1;
		
	  while(1)
		{
				
			LcdTask();
			
		}
			
}

