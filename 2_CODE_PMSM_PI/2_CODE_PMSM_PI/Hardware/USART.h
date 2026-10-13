#ifndef __USART_H
#define __USART_H

#include "gd32f4xx.h"
#include "systick.h"
#include "stdio.h"
#include "stdlib.h"

#define USART_TX_RCU  RCU_GPIOB   // 串口TX的端口时钟
#define USART_RX_RCU  RCU_GPIOB   // 串口RX的端口时钟
#define USART_RCU     RCU_USART0  // 串口0的时钟

#define USART_TX_PORT GPIOB				// 串口TX的端口
#define USART_RX_PORT GPIOB				// 串口RX的端口

#define USART_TX_PIN  GPIO_PIN_6 // 串口TX的引脚
#define USART_RX_PIN  GPIO_PIN_7 // 串口RX的引脚

#define USART 						USART0      								// 串口0
#define USART_IRQ     		USART0_IRQn 								// 串口0中断
#define USART_IRQHandler  USART0_IRQHandler						// 串口0中断服务函数

/* 串口缓冲区的数据长度 */
#define USART_RECEIVE_LENGTH  4096

#define DMA_RCU   	       RCU_DMA1                  // DMA时钟
#define DMA  				       DMA1 										 // DMA
#define DMA_CH  		       DMA_CH7									 // DMA通道
#define DMA_CH_IRQ  			 DMA1_Channel7_IRQn				 // DMA中断
#define DMA_CH_IRQ_HANDLER DMA1_Channel7_IRQHandler  // DMA中断服务函数名
#define DMA_recive_CH      DMA_CH2
#define DMA_recive_CH_IRQ  DMA1_Channel2_IRQn		
#define DMA_recive_CH_IRQ_HANDLER DMA1_Channel2_IRQHandler

int fputc(int ch, FILE *f);
extern float Recive_data[15];
void USART_Config(uint32_t band_rate);  			 // 配置串口



#endif

