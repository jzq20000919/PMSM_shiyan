#include "gd32f4xx_it.h"
#include "arm_math.h"    
#include "IO_define.h"
#include "Algorithm.h"
#include "Chart.h"
#include "USART.h"
#include "ADC.h"
#include "systick.h"
extern uint16_t UsbRx(usb_dev *udev,uint8_t *buf);
extern uint16_t UsbTx(usb_dev *udev,uint8_t *buf,uint16_t len);
static void PWM_Stop(void);
static void PWM_Go(void);
static void Data_get(void);
void Send_data();

static void direction_jundge(void);
 extern uint8_t flagDone ;
extern usb_core_driver cdc_acm;
void usb_timer_irq (void);
static void resume_mcu_clk(void);

//转速计算用
uint8_t flag_cal = 0;
int16_t CNT_now,CNT_last,CNT_error;
int32_t encoder_turn = 0;

//测试用变量
float theta=0,amp=0.5;
uint16_t c=0,z=0,signle_data[15]={0},lenth=0;
uint16_t time_count=0;
//算法用变量
Control_input Control_Input;	
uint8_t Req1msCnt;
float Upload_data[15];

uint8_t Go_flag=0,Udc_error_flag=0;
float Set_Iq=5,Iq;
float Set_RPM=400,last_omega;
float ABZ_count,asad=1500;
uint16_t ADC_IN_B, ADC_IN_C , ADC_IN_A, ADC_IN_Vbus,reverse_flag,restart_count,PWM_FLAG;
float a,b,mid=1500;
void ADC_IRQHandler()//采集完成之后进ADC中断执行FOC算法
{
	adc_interrupt_flag_clear(ADC0, ADC_INT_FLAG_EOIC);

	if(flagDone)
	{
		SendDataFix();   //发送函数，在Scope.c中更改
		AppCmd(&SAppCmd);//接受函数
		Data_get();
		
		if(Go_flag && ERROR_State.STOP ==0) //每一次启停也需要初始化
		{
			PWM_Go();  
			direction_jundge();
		}
		else
		{
			PWM_Stop();
			Algorithm_initialize(); 
		}	
					
					Chart_U.Go_Flag        = Go_flag;  //Go_flag为上位机传来的信号
					Chart_U.Udc            = Control_Input.V_Bus;
					Chart_step();
					Algorithm_U.Enable     = Chart_Y.Go;   
					Algorithm_U.Ref_Iq		 = Control_Input.Ref_Iq;
					Algorithm_U.Ref_RPM    = Control_Input.Ref_RPM;  //上位机传来的设定转速
					Algorithm_U.Real_RPM   = Control_Input.Real_RPM;
					Algorithm_U.Ref_Id		 = 0.0f;
					Algorithm_U.Iabc[0]    = Control_Input.I_A;
					Algorithm_U.Iabc[1]		 = Control_Input.I_B;
					Algorithm_U.Iabc[2]		 = Control_Input.I_C;
					Algorithm_U.Udc   		 = Control_Input.V_Bus;
					Algorithm_U.Real_Theta = Control_Input.encoder_theta;
//					gpio_bit_set(Time_PWM_Port,Time_PWM_Pin);//Enable PA4,algorithm executed time is high level keeping time  
					Algorithm_step();              
//					gpio_bit_reset(Time_PWM_Port,Time_PWM_Pin);//Disable PA4,which can be obtained by oscilloscope
					timer_channel_output_pulse_value_config(TIMER0, TIMER_CH_0,Algorithm_Y.Tabc[0]); //按这个顺序给Tabc
					timer_channel_output_pulse_value_config(TIMER0, TIMER_CH_1,Algorithm_Y.Tabc[1]); 
					timer_channel_output_pulse_value_config(TIMER0, TIMER_CH_2,Algorithm_Y.Tabc[2]); 	

						
								if(ERROR_State.STOP !=0 ) //非0表示电压故障,如果在上一次电压正常了的话，那这一次就不会再进来了，也就无法把MOS关掉
								{
									if(ERROR_State.Over_V==1)//不是过压的情况,那肯定就是电源带载能力太差掉电压欠压了
									MOS_ON						//过压才开泄放电阻
									
									Udc_error_flag=1; //电压出错状态锁存标志位
									PWM_Stop();
//									LED1_ON
									LED2_ON           //测试用才开两个灯，正常开一个灯就行
								}
								else 
								{
									if(Udc_error_flag)
									Algorithm_initialize(); 
									MOS_OFF
//									LED1_OFF
									LED2_OFF
									Udc_error_flag=0;
								}
								
					
			}

}
		

void Data_get()
{
		/* ABZ ENCODER data processing start*/
		ABZ_count = (float)((int32_t)(65536 - timer_counter_read(TIMER1)));
		Control_Input.encoder_count += 	ABZ_count;
		timer_counter_value_config( TIMER1 ,  65536);
		if( Control_Input.encoder_count > 2000)
		{
			Control_Input.encoder_count -= 2000;
			encoder_turn++;
		}
		else if( Control_Input.encoder_count < 0)
		{
			Control_Input.encoder_count += 2000;
			encoder_turn--;
		}
		Control_Input.encoder_theta = 0.003141592653590f * Control_Input.encoder_count;
	/* ABZ ENCODER data processing end*/
	
	  ADC_IN_A		=	adc_inserted_data_read(ADC0, ADC_INSERTED_CHANNEL_0);
	  ADC_IN_B		=	adc_inserted_data_read(ADC0, ADC_INSERTED_CHANNEL_1);
	  ADC_IN_Vbus = adc_inserted_data_read(ADC0, ADC_INSERTED_CHANNEL_2);
  
	  Control_Input.I_A= ( ADC_IN_A - A_mid_valuel ) * 0.0402832f;//0.0107422F; 
	  Control_Input.I_B= ( ADC_IN_B - B_mid_valuel ) * 0.0402832f;
	  Control_Input.I_C= -(Control_Input.I_B + Control_Input.I_A);
    Control_Input.V_Bus   = ADC_IN_Vbus * 0.02659341F; //母线电压，改系数
			
		//在有感电流环测试代码里这个没用，因为需要观测器才能知道电角频率
		Control_Input.Ref_RPM= Set_RPM;
		/*强拖电流符号,IF启动使用*/
//		if(Set_RPM<0.0F)
//			Set_Iq= -Iq;
//		else
//			Set_Iq= Iq;
		Control_Input.Ref_Iq = Iq;
		/*强拖电流符号,IF启动使用*/
		
}


void direction_jundge()
{
	if(Control_Input.Ref_RPM>0)//反转切正转
	{
		if(last_omega<0)
		{
				reverse_flag=1;
				PWM_Stop();      
		}
	}
	else if(Control_Input.Ref_RPM<0)//正转切反转
	{
		if(last_omega>0)
		{
			reverse_flag=1;
			PWM_Stop();
		}
	}
	if( (restart_count++ <=6000) && reverse_flag )
	{
		PWM_Go();
		Algorithm_initialize();	
	}
	else
	{
		restart_count=0;
		reverse_flag=0;
	}
}

float PIANZHI_Count = 0;
//Z信号外部中断角度校准
void Encoder_Z_EXTI_IRQnHandler(void)
{
	if(exti_interrupt_flag_get(Encoder_Z_EXTI_LINE)==SET)
	{
		exti_interrupt_flag_clear(Encoder_Z_EXTI_LINE);
			Control_Input.encoder_count = 845; //实测正反转校准值不同
		//使用CSOIFO先验法实测校准值，范围0-2000内在线调试找到的
		encoder_turn = 0;
		flag_cal = 1;
		
		PIANZHI_Count = (float)((int32_t)(timer_counter_read(TIMER1) - 65536));
	}
}

//转速计算
void TIMER6_IRQHandler(void)
{
		timer_interrupt_flag_clear(TIMER6,TIMER_INT_UP);
		
		if(flag_cal == 0)
		{
				CNT_now = encoder_turn*2000 + Control_Input.encoder_count;
				CNT_error = CNT_now - CNT_last;
			
				Control_Input.Real_RPM = (float)(60.0f*CNT_error/ENCODER_Pulse_ROUND/TIMER6_Time);   //RPM圈/每分钟=一定时间下的脉冲数/一圈的脉冲数/一定时间
			
				CNT_last = CNT_now;
		}
		else
		{
				CNT_now = encoder_turn*2000 + Control_Input.encoder_count;
				CNT_last = CNT_now;
				flag_cal = 0;
		}					
}




///*Simulink上位机发送函数*/




void PWM_Go(void)
{
    timer_channel_output_state_config(TIMER0, TIMER_CH_0, TIMER_CCX_ENABLE);  //配置通道状态
    timer_channel_output_state_config(TIMER0, TIMER_CH_1, TIMER_CCX_ENABLE);
    timer_channel_output_state_config(TIMER0, TIMER_CH_2, TIMER_CCX_ENABLE);
    timer_channel_complementary_output_state_config(TIMER0, TIMER_CH_0, TIMER_CCXN_ENABLE); //配置互补通道输出状态
    timer_channel_complementary_output_state_config(TIMER0, TIMER_CH_1, TIMER_CCXN_ENABLE);
    timer_channel_complementary_output_state_config(TIMER0, TIMER_CH_2, TIMER_CCXN_ENABLE);
}

void PWM_Stop(void)
{
    timer_channel_output_state_config(TIMER0, TIMER_CH_0, TIMER_CCX_DISABLE );  //配置通道状态
    timer_channel_output_state_config(TIMER0, TIMER_CH_1, TIMER_CCX_DISABLE );
    timer_channel_output_state_config(TIMER0, TIMER_CH_2, TIMER_CCX_DISABLE );
    timer_channel_complementary_output_state_config(TIMER0, TIMER_CH_0, TIMER_CCXN_DISABLE ); //配置互补通道输出状态
    timer_channel_complementary_output_state_config(TIMER0, TIMER_CH_1, TIMER_CCXN_DISABLE );
    timer_channel_complementary_output_state_config(TIMER0, TIMER_CH_2, TIMER_CCXN_DISABLE );
}

//往下都是没用到的中断函数
















/*!
    \brief    this function handles NMI exception
    \param[in]  none
    \param[out] none
    \retval     none
*/

void NMI_Handler(void)
{
}

/*!
    \brief    this function handles HardFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void HardFault_Handler(void)
{
    /* if Hard Fault exception occurs, go to infinite loop */
    while(1) {
    }
}

/*!
    \brief    this function handles MemManage exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void MemManage_Handler(void)
{
    /* if Memory Manage exception occurs, go to infinite loop */
    while(1) {
    }
}

/*!
    \brief    this function handles BusFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void BusFault_Handler(void)
{
    /* if Bus Fault exception occurs, go to infinite loop */
    while(1) {
    }
}

/*!
    \brief    this function handles UsageFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void UsageFault_Handler(void)
{
    /* if Usage Fault exception occurs, go to infinite loop */
    while(1) {
    }
}

/*!
    \brief    this function handles SVC exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void SVC_Handler(void)
{
}

/*!
    \brief    this function handles DebugMon exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void DebugMon_Handler(void)
{
}

/*!
    \brief    this function handles PendSV exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void PendSV_Handler(void)
{
}

/*!
    \brief    this function handles SysTick exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void SysTick_Handler(void)
{
    delay_decrement();
}
uint16_t time;
void TIMER3_IRQHandler(void)
{
  if ( SET == timer_interrupt_flag_get(TIMER3 , TIMER_INT_UP) ) 
  {
     timer_interrupt_flag_clear(TIMER3 , TIMER_INT_UP);
     time++;
			
		if(time == 1000)
		{
			time = 0;
			gpio_bit_toggle(LED1_GPIO_Port,LED1_Pin);
		}
  }
}

	
	
/*!
    \brief      this function handles timer2 interrupt Handler
    \param[in]  none
    \param[out] none
    \retval     none
*/
void TIMER2_IRQHandler(void)
{
    usb_timer_irq();
	
}

#ifdef USE_USB_FS

/*!
    \brief      this function handles USBFS wakeup interrupt handler
    \param[in]  none
    \param[out] none
    \retval     none
*/
void USBFS_WKUP_IRQHandler(void)
{
    if (cdc_acm.bp.low_power) {
        resume_mcu_clk();

        rcu_pll48m_clock_config(RCU_PLL48MSRC_PLLQ);
        rcu_ck48m_clock_config(RCU_CK48MSRC_PLL48M);

        rcu_periph_clock_enable(RCU_USBFS);

        usb_clock_active(&cdc_acm);
    }

    exti_interrupt_flag_clear(EXTI_18);
}

#elif defined(USE_USB_HS)

/*!
    \brief      this function handles USBHS wakeup interrupt handler
    \param[in]  none
    \param[out] none
    \retval     none
*/
void USBHS_WKUP_IRQHandler(void)
{
    if (cdc_acm.bp.low_power) {
        resume_mcu_clk();

        #ifdef USE_EMBEDDED_PHY
            rcu_pll48m_clock_config(RCU_PLL48MSRC_PLLQ);
            rcu_ck48m_clock_config(RCU_CK48MSRC_PLL48M);
        #elif defined(USE_ULPI_PHY)
            rcu_periph_clock_enable(RCU_USBHSULPI);
        #endif

        rcu_periph_clock_enable(RCU_USBHS);

        usb_clock_active(&cdc_acm);
    }

    exti_interrupt_flag_clear(EXTI_20);
}

#endif /* USE_USBFS */

#ifdef USE_USB_FS

/*!
    \brief      this function handles USBFS IRQ Handler
    \param[in]  none
    \param[out] none
    \retval     none
*/
void USBFS_IRQHandler(void)
{
    usbd_isr(&cdc_acm);
}

#elif defined(USE_USB_HS)

/*!
    \brief      this function handles USBHS IRQ Handler
    \param[in]  none
    \param[out] none
    \retval     none
*/
void USBHS_IRQHandler(void)
{
    usbd_isr(&cdc_acm);
}

#endif /* USE_USBFS */

#ifdef USB_HS_DEDICATED_EP1_ENABLED

/*!
    \brief      this function handles EP1_IN Handler
    \param[in]  none
    \param[out] none
    \retval     none
*/
void USBHS_EP1_In_IRQHandler(void)
{
    usbd_int_dedicated_ep1in (&cdc_acm);
}

/*!
    \brief      this function handles EP1_OUT Handler
    \param[in]  none
    \param[out] none
    \retval     none
*/
void USBHS_EP1_Out_IRQHandler(void)
{
    usbd_int_dedicated_ep1out (&cdc_acm);
}

#endif /* USB_HS_DEDICATED_EP1_ENABLED */

/*!
    \brief      resume mcu clock
    \param[in]  none
    \param[out] none
    \retval     none
*/
static void resume_mcu_clk(void)
{
    /* enable HXTAL */
    rcu_osci_on(RCU_HXTAL);

    /* wait till HXTAL is ready */
    while(RESET == rcu_flag_get(RCU_FLAG_HXTALSTB)){
    }

    /* enable PLL */
    rcu_osci_on(RCU_PLL_CK);

    /* wait till PLL is ready */
    while(RESET == rcu_flag_get(RCU_FLAG_PLLSTB)){
    }

    /* select PLL as system clock source */
    rcu_system_clock_source_config(RCU_CKSYSSRC_PLLP);

    /* wait till PLL is used as system clock source */
    while(RCU_SCSS_PLLP != rcu_system_clock_source_get()){
    }
}
