#ifndef __LCD_INIT_H
#define __LCD_INIT_H

#include "main.h"

#include "IO_define.h"


#define USE_HORIZONTAL 2  //设置横屏或者竖屏显示 0或1为竖屏 2或3为横屏


#if USE_HORIZONTAL==0||USE_HORIZONTAL==1
#define LCD_W 80
#define LCD_H 160

#else
#define LCD_W 160
#define LCD_H 80
#endif



//-----------------LCD端口定义---------------- 

#define LCD_SCLK_Clr() gpio_bit_reset(LCD_SCL_GPIO_Port,LCD_SCL_Pin)//SCL=SCLK
#define LCD_SCLK_Set() gpio_bit_set(LCD_SCL_GPIO_Port,LCD_SCL_Pin)

#define LCD_MOSI_Clr() gpio_bit_reset(LCD_SDA_GPIO_Port,LCD_SDA_Pin)//SDA=MOSI
#define LCD_MOSI_Set() gpio_bit_set(LCD_SDA_GPIO_Port,LCD_SDA_Pin)

#define LCD_RES_Clr()  gpio_bit_reset(LCD_RES_GPIO_Port,LCD_RES_Pin)//RES
#define LCD_RES_Set()  gpio_bit_set(LCD_RES_GPIO_Port,LCD_RES_Pin)

#define LCD_DC_Clr()   gpio_bit_reset(LCD_DC_GPIO_Port,LCD_DC_Pin)//DC
#define LCD_DC_Set()   gpio_bit_set(LCD_DC_GPIO_Port,LCD_DC_Pin)
 		     
#define LCD_CS_Clr()   gpio_bit_reset(LCD_CS_GPIO_Port,LCD_CS_Pin)//CS
#define LCD_CS_Set()   gpio_bit_set(LCD_CS_GPIO_Port,LCD_CS_Pin)

#define LCD_BLK_Clr()  gpio_bit_reset(LCD_BLK_GPIO_Port,LCD_BLK_Pin)//BLK
#define LCD_BLK_Set()  gpio_bit_set(LCD_BLK_GPIO_Port,LCD_BLK_Pin)





void LCD_Writ_Bus(uint8_t dat);//模拟SPI时序
void LCD_WR_DATA8(uint8_t dat);//写入一个字节
void LCD_WR_DATA(uint16_t dat);//写入两个字节
void LCD_WR_REG(uint8_t dat);//写入一个指令
void LCD_Address_Set(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2);//设置坐标函数
void LCD_Init(void);//LCD初始化
#endif




