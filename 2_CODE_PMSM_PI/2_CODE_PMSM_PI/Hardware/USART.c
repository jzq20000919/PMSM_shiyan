#include "USART.h"
#include "gd32f4xx_it.h"



float Recive_data[15]={0};
/************************************************
函数名称 ： usart_gpio_config
功    能 ： 串口配置GPIO
参    数 ： band_rate:波特率
返 回 值 ： 无
*************************************************/
void USART_Config(uint32_t band_rate)
{
	dma_single_data_parameter_struct dma_init_struct;                // DMA单数据结构体
	rcu_periph_clock_enable(DMA_RCU);														// 开启DMA时钟
	rcu_periph_clock_enable(USART_TX_RCU);   // 开启串口时钟
	rcu_periph_clock_enable(USART_RX_RCU);   // 开启端口时钟
	rcu_periph_clock_enable(USART_RCU);      // 开启端口时钟
	
	/* 配置GPIO复用功能 */
  gpio_af_set(USART_TX_PORT,GPIO_AF_7,USART_TX_PIN);	
	gpio_af_set(USART_RX_PORT,GPIO_AF_7,USART_RX_PIN);	
	/* 配置TX为复用模式 上拉模式 配置RX为复用模式 上拉模式*/
	gpio_mode_set(USART_TX_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, USART_TX_PIN);
	gpio_mode_set(USART_RX_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, USART_RX_PIN);
	/* 配置TX,RX为推挽输出 50MHZ */
	gpio_output_options_set(USART_TX_PORT,GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, USART_TX_PIN);
	gpio_output_options_set(USART_RX_PORT,GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, USART_RX_PIN);

	
	/* 配置串口的参数 */
	usart_deinit(USART);                                 // 复位串口
	usart_baudrate_set(USART,band_rate);                 // 设置波特率
	usart_parity_config(USART,USART_PM_NONE);            // 没有校验位
	usart_word_length_set(USART,USART_WL_8BIT);          // 8位数据位
	usart_stop_bit_set(USART,USART_STB_1BIT);     			 // 1位停止位
	
	usart_transmit_config(USART,USART_TRANSMIT_ENABLE);  // 使能串口发送
	usart_receive_config(USART,USART_RECEIVE_ENABLE);    // 使能串口接收
	usart_enable(USART);                                 // 使能串口

//	usart_interrupt_enable(USART,USART_INT_IDLE);
//  usart_interrupt_enable(USART,USART_INT_RBNE);
//	nvic_irq_enable(USART_IRQ, 2, 2); 
	 
	 /* 配置DMA发送通道初始化参数 */
	   dma_deinit(DMA,DMA_CH);																	// 初始化DMA发送通道
		 dma_init_struct.direction 					 = DMA_MEMORY_TO_PERIPH;          		 // 内存到外设
		 dma_init_struct.memory_inc 				 = DMA_MEMORY_INCREASE_ENABLE;      	 // 增量模式    
		 dma_init_struct.periph_memory_width = DMA_PERIPH_WIDTH_8BIT;         		 // 一次传输长度8bit     	
		// dma_init_struct.memory0_addr 		 = 
		// dma_init_struct.number 					 =          
		 dma_init_struct.periph_addr 				 = (uint32_t)&USART_DATA(USART);   // 外设地址        
		 dma_init_struct.periph_inc          = DMA_PERIPH_INCREASE_DISABLE;         
		 dma_init_struct.circular_mode 			 = DMA_CIRCULAR_MODE_DISABLE;      		 // 关闭循环模式               
		 dma_init_struct.priority						 = DMA_PRIORITY_HIGH;  					 // 超高优先级
		 dma_single_data_mode_init(DMA,DMA_CH,&dma_init_struct);
		 
		 dma_channel_subperipheral_select(DMA,DMA_CH,DMA_SUBPERI4);//横向纵向选择
	   usart_dma_transmit_config(USART,USART_DENT_ENABLE); //使能DMA发送串口功能
		 dma_channel_enable(DMA,DMA_CH);//使能DMA通道
		 dma_interrupt_enable(DMA,DMA_CH,DMA_CHXCTL_FTFIE);
		 nvic_irq_enable(DMA_CH_IRQ, 2, 1); 
				 
			/***接收通道相关初始化**/
			 dma_deinit(DMA,DMA_recive_CH);		
			 dma_init_struct.direction 					 = DMA_PERIPH_TO_MEMORY;          		 // 内存到外设
			 dma_init_struct.memory_inc 				 = DMA_MEMORY_INCREASE_ENABLE;      	 // 增量模式    
			 dma_init_struct.periph_memory_width = DMA_PERIPH_WIDTH_8BIT;         		 // 一次传输长度8bit     	
			 dma_init_struct.memory0_addr 		   = (uint32_t)Recive_data;
			 dma_init_struct.number 					   = 20;         
			 dma_init_struct.periph_addr 				 = (uint32_t)&USART_DATA(USART);   // 外设地址        
			 dma_init_struct.periph_inc          = DMA_PERIPH_INCREASE_DISABLE;         
			 dma_init_struct.circular_mode 			 = DMA_CIRCULAR_MODE_DISABLE;      		 // 关闭循环模式               
			 dma_init_struct.priority						 = DMA_PRIORITY_ULTRA_HIGH;  					 // 超高优先级
			 dma_single_data_mode_init(DMA,DMA_recive_CH,&dma_init_struct);
			 
			 dma_channel_subperipheral_select(DMA,DMA_recive_CH,DMA_SUBPERI4);//横向纵向选择
			 usart_dma_receive_config(USART,USART_DENR_ENABLE); //使能DMA接收串口功能
			 dma_channel_enable(DMA,DMA_recive_CH);//使能DMA通道
			 dma_interrupt_enable(DMA,DMA_recive_CH,DMA_CHXCTL_FTFIE);
			 nvic_irq_enable(DMA_recive_CH_IRQ, 2, 0); 
}






