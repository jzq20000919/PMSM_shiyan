#include "GPIO.h"
#include "IO_define.h"
void GPIO_Init()
{
	 rcu_periph_clock_enable(RCU_GPIOC);//启用外设时钟功能  
	 rcu_periph_clock_enable(RCU_GPIOA);//启用外设时钟功能  
	 rcu_periph_clock_enable(RCU_GPIOB);
	
//   gpio_mode_set(Time_PWM_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, Time_PWM_Pin);
//   gpio_output_options_set(Time_PWM_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, Time_PWM_Pin);
	
	
	 /**泄放电阻MOS管驱动引脚初始化**/
	 gpio_mode_set(DISCHARGE_MOS_HGPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, DISCHARGE_MOS_HPin);
   gpio_output_options_set(DISCHARGE_MOS_HGPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, DISCHARGE_MOS_HPin);
	 gpio_mode_set(DISCHARGE_MOS_LGPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, DISCHARGE_MOS_LPin);
   gpio_output_options_set(DISCHARGE_MOS_LGPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, DISCHARGE_MOS_LPin);
	
	 /**两个LED1,2的初始化**/
	 gpio_mode_set(LED1_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED1_Pin);
   gpio_output_options_set(LED1_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, LED1_Pin);
	 gpio_mode_set(LED2_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED2_Pin);
   gpio_output_options_set(LED2_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, LED2_Pin);
	
	
	 
	 
	 //IO默认置0
//	 gpio_bit_reset(Nouse_PWM_Port,Nouse_PWM_Pin);
	 gpio_bit_reset(DISCHARGE_MOS_HGPIO_Port,DISCHARGE_MOS_HPin);
	 gpio_bit_reset(DISCHARGE_MOS_LGPIO_Port,DISCHARGE_MOS_LPin);
	 gpio_bit_reset(LED2_GPIO_Port,LED2_Pin);
	 gpio_bit_reset(LED1_GPIO_Port,LED1_Pin);
	 
	 

	 gpio_bit_set(LED1_GPIO_Port,LED1_Pin);
	 gpio_bit_set(LED2_GPIO_Port,LED2_Pin);
	 
}

