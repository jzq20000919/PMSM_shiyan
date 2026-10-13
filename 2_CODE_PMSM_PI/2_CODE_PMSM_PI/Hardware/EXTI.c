#include "EXTI.h"
#include "IO_define.h"

void Encoder_EXIT_Init()//AB相计数
{
	 timer_parameter_struct timer_initpara;       //TIMER初始化结构体
	 timer_ic_parameter_struct  timer_icinitpara;
	 rcu_periph_clock_enable(RCU_TIMER1);

	/*Encoder_EXIT 引脚初始化*/
	rcu_periph_clock_enable(RCU_GPIOA);//启用外设时钟功能  
	rcu_periph_clock_enable(RCU_GPIOB);//启用外设时钟功能  
	
	gpio_mode_set(Encoder_A_Port, GPIO_MODE_AF, GPIO_PUPD_NONE, Encoder_A_Pin);
	gpio_af_set(Encoder_A_Port,GPIO_AF_1,Encoder_A_Pin);//端口复用为TIMER1_CH0模式
	gpio_output_options_set(Encoder_A_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, Encoder_A_Pin);
	//AF1是TIMER0和TIMER1的复用
  gpio_mode_set(Encoder_B_Port, GPIO_MODE_AF, GPIO_PUPD_NONE, Encoder_B_Pin);
	gpio_af_set(Encoder_B_Port,GPIO_AF_1,Encoder_B_Pin);//端口复用为TIMER1_CH1模式
	gpio_output_options_set(Encoder_B_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, Encoder_B_Pin);
	/*Encoder_EXIT 引脚初始化*/
	
	
	/*Encoder_EXIT 外设初始化*/
	 timer_initpara.prescaler=0;
	 timer_initpara.alignedmode=TIMER_COUNTER_EDGE;
	 timer_initpara.counterdirection=TIMER_COUNTER_UP;
	 timer_initpara.clockdivision=TIMER_CKDIV_DIV1;
	 timer_initpara.period=100000;
	 timer_initpara.repetitioncounter=0;
	 timer_init(TIMER1,&timer_initpara);

	  timer_icinitpara.icpolarity  = TIMER_IC_POLARITY_RISING;  //通道控制寄存器2(TIMERx_CHCTL2)的CH0NP和CH0P位全部置1
	  //由于timer_quadrature_decoder_mode_config会重新设置CH0NP和CH0P位,所以这个地方随便写
    timer_icinitpara.icselection = TIMER_IC_SELECTION_DIRECTTI;  //选择ISx映射在 CIxFE0/CIxFE0/ITS 三选一 。选CIxFE0
    timer_icinitpara.icprescaler = TIMER_IC_PSC_DIV1;//编码器应用不需要对输入的AB信号进行分频
    timer_icinitpara.icfilter    = 15;   //ABZ编码器信号质量高，不需要滤波
	  timer_input_capture_config(TIMER1, TIMER_CH_0, &timer_icinitpara); 
    timer_input_capture_config(TIMER1, TIMER_CH_1, &timer_icinitpara);
	 
	  timer_quadrature_decoder_mode_config(TIMER1, TIMER_ENCODER_MODE2, 
										TIMER_IC_POLARITY_RISING, TIMER_IC_POLARITY_RISING);
		//timer_slave_mode_select(TIMER1,TIMER_ENCODER_MODE2);
		timer_enable(TIMER1);
}

void Encoder_Z_Init(void) //Z编码器校正定位
{
	 rcu_periph_clock_enable(RCU_GPIOA); //开启时钟
	 rcu_periph_clock_enable(RCU_SYSCFG);
	 gpio_mode_set(Encoder_Z_Port, GPIO_MODE_INPUT, GPIO_PUPD_PULLDOWN , Encoder_Z_Pin);//配置GPIO模式
	 
	 nvic_irq_enable(Encoder_Z_EXTI_IRQn, 1, 1); //设置中断优先级
	 syscfg_exti_line_config(Encoder_Z_EXTI_SOURCE, Encoder_Z_EXTI_SOURCE_PIN);//GPIO和中断线进行连接
	
	 exti_init(Encoder_Z_EXTI_LINE, EXTI_INTERRUPT, EXTI_TRIG_RISING);  //设置上升沿触发，中断模式
	 exti_interrupt_enable(Encoder_Z_EXTI_LINE); //使能中断
	 exti_interrupt_flag_clear(Encoder_Z_EXTI_LINE);//清除中断标志位
	 
}

void Timer6_Init()
{
	timer_parameter_struct timer_initpara;       //TIMER初始化结构体
	rcu_periph_clock_enable(RCU_TIMER6);
	
	timer_initpara.prescaler=839;
	timer_initpara.alignedmode=TIMER_COUNTER_EDGE;
	timer_initpara.counterdirection=TIMER_COUNTER_UP;
	timer_initpara.clockdivision=TIMER_CKDIV_DIV1;
	timer_initpara.period=1000;
	timer_initpara.repetitioncounter=0;
	timer_init(TIMER6,&timer_initpara);
	
	timer_interrupt_flag_clear(TIMER6,TIMER_INT_UP);    //清除中断标志位
	timer_interrupt_enable(TIMER6,TIMER_INT_UP);     //中断使能
	
	nvic_irq_enable(TIMER6_IRQn,1,0);    //设置中断优先级
	
	timer_enable(TIMER6);    //使能定时器6

}
