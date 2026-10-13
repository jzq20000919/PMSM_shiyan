#include "ADC.h"
#include "IO_define.h"
uint16_t B_mid_valuel,A_mid_valuel;
void ADC_Init()
{
	/*ADC引脚初始化*/
	 rcu_periph_clock_enable(RCU_GPIO_ADCin_01);//启用外设时钟功能  
	 rcu_periph_clock_enable(RCU_GPIO_ADCin_02);
	 gpio_mode_set(ADC_B_Port, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, ADC_B_Pin);
   gpio_mode_set(ADC_C_Port, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, ADC_C_Pin);
	 gpio_mode_set(ADC_A_Port, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, ADC_A_Pin);
	 gpio_mode_set(ADC_Vbus_Port, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, ADC_Vbus_Pin);
	 /*ADC引脚初始化*/
	
	/*ADC外设初始化*/
	rcu_periph_clock_enable(RCU_Used_ADC1st);  //使能ADC时钟
	adc_clock_config(ADC_ADCCK_PCLK2_DIV6);// APB2总线最大84MHz 6分配速度拉满14MHz
	
	adc_deinit();  //将寄存器恢复至默认值
	adc_special_function_config(Used_ADC1st, ADC_SCAN_MODE, ENABLE );  //多通道采集，需要开启扫描模式，依次扫描1、2通道
	// 触发源是TIMER0的CH3的TRGO，所以每来一次就开启一次转换，转换完成后就进中断
	
	adc_channel_length_config(Used_ADC1st, ADC_INSERTED_CHANNEL, 3);     //配置规则通道组或注入通道组的长度
	adc_inserted_channel_config(Used_ADC1st,0,ADC_A_channel, ADC_SAMPLETIME_15);//配置ADC注入通道组
	adc_inserted_channel_config(Used_ADC1st,1,ADC_B_channel, ADC_SAMPLETIME_15);//C相为注入通道0，B相为注入通道1
	adc_inserted_channel_config(Used_ADC1st,2,ADC_Vbus_channel, ADC_SAMPLETIME_15);
	
	adc_resolution_config(Used_ADC1st,ADC_RESOLUTION_12B);
	adc_data_alignment_config(Used_ADC1st, ADC_DATAALIGN_RIGHT); //数据12位右对齐
	
	//选取外部硬件触发源，TIMER0的CH3TRGO作为触发源，上升沿触发
	adc_external_trigger_source_config(Used_ADC1st ,ADC_INSERTED_CHANNEL,ADC_EXTTRIG_INSERTED_T0_CH3);  
	adc_external_trigger_config(Used_ADC1st, ADC_INSERTED_CHANNEL , EXTERNAL_TRIGGER_RISING);
	
	//adc_interrupt_flag_clear(Used_ADC1st, ADC_INT_FLAG_EOC);  //复位规则组中断标志位
  adc_interrupt_flag_clear(Used_ADC1st, ADC_INT_FLAG_EOIC); //复位注入组中断标志位
	adc_interrupt_enable(Used_ADC1st,ADC_INT_EOIC);           //注入通道组转换结束中断标志位置位
	
	adc_enable(Used_ADC1st);
	adc_calibration_enable(Used_ADC1st);
	
	//ADC的NVIC中断等级设置
  nvic_irq_enable(ADC_IRQn, 0, 0);
}

void mid_ADC_value_Init()
{
	 uint16_t i;
	 //A相上管初始化
	 gpio_mode_set(PWM_A_UP_Port, GPIO_MODE_AF, GPIO_PUPD_NONE, PWM_A_UP_PIN);//GD系列需要操作复用
   gpio_output_options_set(PWM_A_UP_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, PWM_A_UP_PIN);
   gpio_af_set(PWM_A_UP_Port,GPIO_AF_1,PWM_A_UP_PIN);//端口复用为TIMER0_CH0模式
	 //A相下管初始化
	 gpio_mode_set(PWM_A_DOWN_Port, GPIO_MODE_AF, GPIO_PUPD_NONE, PWM_A_DOWN_PIN);//GD系列需要操作复用
   gpio_output_options_set(PWM_A_DOWN_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, PWM_A_DOWN_PIN);
   gpio_af_set(PWM_A_DOWN_Port,GPIO_AF_1,PWM_A_DOWN_PIN);//端口复用为TIMER0_CH0模式
	 //B相上管初始化
	 gpio_mode_set(PWM_B_UP_Port, GPIO_MODE_AF, GPIO_PUPD_NONE, PWM_B_UP_PIN);//GD系列需要操作复用
   gpio_output_options_set(PWM_B_UP_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, PWM_B_UP_PIN);
   gpio_af_set(PWM_B_UP_Port,GPIO_AF_1,PWM_B_UP_PIN);//端口复用为TIMER0_CH0模式
	 //B相下管初始化
	 gpio_mode_set(PWM_B_DOWN_Port, GPIO_MODE_AF, GPIO_PUPD_NONE, PWM_B_DOWN_PIN);//GD系列需要操作复用
   gpio_output_options_set(PWM_B_DOWN_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, PWM_B_DOWN_PIN);
   gpio_af_set(PWM_B_DOWN_Port,GPIO_AF_1,PWM_B_DOWN_PIN);//端口复用为TIMER0_CH0模式
	 //C相上管初始化
	 gpio_mode_set(PWM_C_UP_Port, GPIO_MODE_AF, GPIO_PUPD_NONE, PWM_C_UP_PIN);//GD系列需要操作复用
   gpio_output_options_set(PWM_C_UP_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, PWM_C_UP_PIN);
   gpio_af_set(PWM_C_UP_Port,GPIO_AF_1,PWM_C_UP_PIN);//端口复用为TIMER0_CH0模式
	 //C相下管初始化
	 gpio_mode_set(PWM_C_DOWN_Port, GPIO_MODE_AF, GPIO_PUPD_NONE, PWM_C_DOWN_PIN);//GD系列需要操作复用
   gpio_output_options_set(PWM_C_DOWN_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, PWM_C_DOWN_PIN);
   gpio_af_set(PWM_C_DOWN_Port,GPIO_AF_1,PWM_C_DOWN_PIN);//端口复用为TIMER0_CH0模式
	 
	 //关闭全部上桥，开通全部下桥，下桥短路，此时采样电阻两端等电位。
	 gpio_bit_reset(PWM_A_UP_Port,PWM_A_UP_PIN);
	 gpio_bit_reset(PWM_B_UP_Port,PWM_B_UP_PIN);
	 gpio_bit_reset(PWM_C_UP_Port,PWM_C_UP_PIN);
	 gpio_bit_set(PWM_A_DOWN_Port,PWM_A_DOWN_PIN);
	 gpio_bit_set(PWM_B_DOWN_Port,PWM_B_DOWN_PIN);
	 gpio_bit_set(PWM_C_DOWN_Port,PWM_C_DOWN_PIN);
   
	 rcu_periph_clock_enable(RCU_GPIO_ADCin_01);//启用外设时钟功能  
	 rcu_periph_clock_enable(RCU_GPIO_ADCin_02);
	 gpio_mode_set(ADC_B_Port, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, ADC_B_Pin);
   gpio_mode_set(ADC_C_Port, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, ADC_C_Pin);
	 gpio_mode_set(ADC_A_Port, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, ADC_A_Pin);

	 rcu_periph_clock_enable(RCU_Used_ADC1st);  //使能ADC时钟
	 adc_clock_config(ADC_ADCCK_PCLK2_DIV6);// APB2总线最大84MHz 6分配速度拉满14MHz
	 adc_deinit();  //将寄存器恢复至默认值
	 adc_special_function_config(Used_ADC1st, ADC_SCAN_MODE, ENABLE );  //多通道采集，需要开启扫描模式，依次扫描1、2通道
	 
	 adc_channel_length_config(Used_ADC1st, ADC_INSERTED_CHANNEL, 2);     //配置规则通道组或注入通道组的长度
	 adc_inserted_channel_config(Used_ADC1st,0,ADC_A_channel, ADC_SAMPLETIME_15);//配置ADC注入通道组
	 adc_inserted_channel_config(Used_ADC1st,1,ADC_B_channel, ADC_SAMPLETIME_15);//C相为注入通道0，B相为注入通道1
	 
	 adc_resolution_config(Used_ADC1st,ADC_RESOLUTION_12B);
	 adc_data_alignment_config(Used_ADC1st, ADC_DATAALIGN_RIGHT); //数据12位右对齐
	 
	 

   adc_end_of_conversion_config(Used_ADC1st,ADC_EOC_SET_CONVERSION);
	 adc_enable(Used_ADC1st);
	 adc_calibration_enable(Used_ADC1st);
	 
	 
	 adc_software_trigger_enable(Used_ADC1st,ADC_INSERTED_CHANNEL);	
	 
	 while(!adc_flag_get(Used_ADC1st,ADC_FLAG_STIC));
	 while(!adc_flag_get(Used_ADC1st,ADC_FLAG_EOIC));

   for(i=0;i<30;i++)
	 {
		adc_software_trigger_enable(Used_ADC1st,ADC_INSERTED_CHANNEL);	
		while(!adc_flag_get(Used_ADC1st,ADC_FLAG_STIC));
	  while(!adc_flag_get(Used_ADC1st,ADC_FLAG_EOIC));
		A_mid_valuel+=adc_inserted_data_read(Used_ADC1st,ADC_INSERTED_CHANNEL_0);
		B_mid_valuel+=adc_inserted_data_read(Used_ADC1st,ADC_INSERTED_CHANNEL_1);
		delay_1ms(10);
	 }
	 B_mid_valuel=B_mid_valuel/i;
	 A_mid_valuel=A_mid_valuel/i;
}	


