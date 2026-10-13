#include "PWM.h"
#include "IO_define.h"
int DEADTIME=50;

void PWM_Init()//PWM输出 ADC触发设置
{

   //定时器相关结构体
 	 timer_parameter_struct timer_initpara;        //TIMER初始化结构体
	 timer_break_parameter_struct timer_bkdtpara;  //中止功能配置结构体,配置中止功能
	 timer_oc_parameter_struct timer_ocintpara;    //输出通道结构体,将TIMER通道输出参数结构体中所有参数初始化
	
	/***PWM GPIO引脚初始化***/
	 rcu_periph_clock_enable(RCU_GPIOA);//启用外设时钟功能  
	 rcu_periph_clock_enable(RCU_GPIOB);
	        //这开时钟要加个 RCU_  前缀，人麻了
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
	 //BRKIN引脚输入初始化
 /***PWM GPIO引脚初始化***/
 
	/***PWM定时器0 外设初始化***/
	 rcu_periph_clock_enable(RCU_TIMER0);
	 timer_deinit(TIMER0);
	 rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL2); //确保进入到定时器的时钟频率为168MHz,
	 //需要查看一下时钟树
	 
	 //使能PWM_TIMER，预分频系数为2  倒三角 所以在仿真中需要乘以-1
	 timer_initpara.prescaler=PWM_PRES-1;//2-1
	 timer_initpara.alignedmode=TIMER_COUNTER_CENTER_DOWN;
	 timer_initpara.counterdirection=TIMER_COUNTER_UP;
	 timer_initpara.clockdivision=TIMER_CKDIV_DIV1;
	 timer_initpara.period=PWM_PERI;//4199
	 timer_initpara.repetitioncounter=0;
	 timer_init(TIMER0,&timer_initpara);
	 
		 //输出通道配置
    timer_ocintpara.outputstate  = TIMER_CCX_ENABLE;  
    timer_ocintpara.outputnstate = TIMER_CCXN_ENABLE;
    timer_ocintpara.ocpolarity   = TIMER_OC_POLARITY_HIGH;     
    timer_ocintpara.ocnpolarity  = TIMER_OCN_POLARITY_HIGH;
    timer_ocintpara.ocidlestate  = TIMER_OC_IDLE_STATE_LOW;
    timer_ocintpara.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;
    
    timer_channel_output_config(TIMER0, TIMER_CH_0,&timer_ocintpara);
    timer_channel_output_config(TIMER0, TIMER_CH_1,&timer_ocintpara);
    timer_channel_output_config(TIMER0, TIMER_CH_2,&timer_ocintpara);
	
	//死区刹车相关配置
		 timer_bkdtpara.runoffstate = TIMER_ROS_STATE_ENABLE;
		 timer_bkdtpara.ideloffstate = TIMER_IOS_STATE_ENABLE;
		 timer_bkdtpara.protectmode = TIMER_CCHP_PROT_0;
		 timer_bkdtpara.deadtime = DEADTIME;
		 timer_bkdtpara.breakstate = TIMER_BREAK_DISABLE;   
		 timer_bkdtpara.breakpolarity = TIMER_BREAK_POLARITY_HIGH;
		 timer_bkdtpara.outputautostate = TIMER_OUTAUTO_DISABLE;
		 timer_break_config(TIMER0, &timer_bkdtpara);
	//死区刹车相关配置	
	
	//通道3不输出
    timer_ocintpara.outputstate = TIMER_CCX_ENABLE;
    timer_ocintpara.ocpolarity = TIMER_OC_POLARITY_LOW;
    timer_ocintpara.ocidlestate = TIMER_OC_IDLE_STATE_LOW;
    timer_channel_output_config(TIMER0, TIMER_CH_3,&timer_ocintpara);
    
		//channel 0~2 通道输出配置
		timer_channel_output_pulse_value_config(TIMER0, TIMER_CH_0, 100); //配置外设TIMERx的通道输出比较值
		timer_channel_output_mode_config(TIMER0, TIMER_CH_0, TIMER_OC_MODE_PWM0);//配置外设TIMERx通道输出比较模式
		timer_channel_output_shadow_config(TIMER0, TIMER_CH_0, TIMER_OC_SHADOW_ENABLE); //预装载使能
		
		timer_channel_output_pulse_value_config(TIMER0, TIMER_CH_1, 100); 
		timer_channel_output_mode_config(TIMER0, TIMER_CH_1, TIMER_OC_MODE_PWM0);
	  timer_channel_output_shadow_config(TIMER0, TIMER_CH_1, TIMER_OC_SHADOW_ENABLE); 
		
		timer_channel_output_pulse_value_config(TIMER0, TIMER_CH_2, 100); 
		timer_channel_output_mode_config(TIMER0, TIMER_CH_2, TIMER_OC_MODE_PWM0);
		timer_channel_output_shadow_config(TIMER0, TIMER_CH_2, TIMER_OC_SHADOW_ENABLE); 
		
		//channel 3设置，用于触发ADC
		timer_channel_output_pulse_value_config(TIMER0, TIMER_CH_3, PWM_PERI-1); //4198
    timer_channel_output_mode_config(TIMER0, TIMER_CH_3, TIMER_OC_MODE_PWM1);
    timer_channel_output_shadow_config(TIMER0, TIMER_CH_3, TIMER_OC_SHADOW_ENABLE);  //定时器重载影子使能
    timer_master_output_trigger_source_select(TIMER0, TIMER_TRI_OUT_SRC_O3CPRE); //比较。在这种模式下主模式控制器选择O3CPRE信号被用于作为触发输出TRGO
    //timer_master_output_trigger_source_select是多出012配置的
		timer_channel_output_state_config(TIMER0, TIMER_CH_0, TIMER_CCX_ENABLE)	;  //配置通道状态
    timer_channel_output_state_config(TIMER0, TIMER_CH_1, TIMER_CCX_ENABLE);
    timer_channel_output_state_config(TIMER0, TIMER_CH_2, TIMER_CCX_ENABLE);
    timer_channel_output_state_config(TIMER0, TIMER_CH_3, TIMER_CCX_ENABLE);
    timer_channel_complementary_output_state_config(TIMER0, TIMER_CH_0, TIMER_CCXN_ENABLE); //配置互补通道输出状态
    timer_channel_complementary_output_state_config(TIMER0, TIMER_CH_1, TIMER_CCXN_ENABLE);
    timer_channel_complementary_output_state_config(TIMER0, TIMER_CH_2, TIMER_CCXN_ENABLE);
		
  //  timer_update_source_config(TIMER0, TIMER_UPDATE_SRC_REGULAR);   //配置外设TIMERx的更新源
  //  timer_single_pulse_mode_config(TIMER0, TIMER_SP_MODE_REPETITIVE);//配置外设TIMERx的脉冲模式（单、重复）
    timer_primary_output_config(TIMER0,ENABLE);  //高级定时器才需要使能， 所有的通道输出使能
  //  timer_auto_reload_shadow_enable(TIMER0);   //TIMERx自动重载影子使能
    timer_enable(TIMER0);  //定时器使能
		
		timer_channel_output_state_config(TIMER0, TIMER_CH_0, TIMER_CCX_DISABLE );  //配置通道状态
    timer_channel_output_state_config(TIMER0, TIMER_CH_1, TIMER_CCX_DISABLE );
    timer_channel_output_state_config(TIMER0, TIMER_CH_2, TIMER_CCX_DISABLE );
    timer_channel_complementary_output_state_config(TIMER0, TIMER_CH_0, TIMER_CCXN_DISABLE ); //配置互补通道输出状态
    timer_channel_complementary_output_state_config(TIMER0, TIMER_CH_1, TIMER_CCXN_DISABLE );
    timer_channel_complementary_output_state_config(TIMER0, TIMER_CH_2, TIMER_CCXN_DISABLE );
}



