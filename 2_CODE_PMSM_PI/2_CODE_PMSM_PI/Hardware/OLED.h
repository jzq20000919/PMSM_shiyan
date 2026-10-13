
#define _LCD_ZK_H_
#ifdef _LCD_ZK_H_
#include "gd32f4xx.h"
#include "systick.h"
/* the macro definition to trigger the led on or off 
 * 1 - off
 - 0 - on
 */
#define ON  0
#define OFF 1

//带参宏，可以像内联函数一样使用
#define Port_CLK_Pin  	GPIOB
#define Port_MOSI_Pin 	GPIOB
#define Port_DC_Pin   	GPIOB
#define Port_CS1_Pin 	  GPIOB
#define Port_FS0_Pin 	  GPIOB
#define Port_CS2_Pin 	  GPIOD

#define CLK_Pin  				GPIO_PIN_3
#define MOSI_Pin 				GPIO_PIN_5
#define DC_Pin 				  GPIO_PIN_9
#define CS1_Pin 				GPIO_PIN_8
#define FS0_Pin 				GPIO_PIN_4
#define CS2_Pin  				GPIO_PIN_2

#define lcd_sclk(a)	if (a)	\
					gpio_bit_set(Port_CLK_Pin,CLK_Pin);\
					else		\
					gpio_bit_reset(Port_CLK_Pin,CLK_Pin)
					
#define lcd_sid(a)	if (a)	\
					gpio_bit_set(Port_MOSI_Pin,MOSI_Pin);\
					else		\
					gpio_bit_reset(Port_MOSI_Pin,MOSI_Pin)
					
#define lcd_rs(a)	if (a)	\
					gpio_bit_set(Port_DC_Pin,DC_Pin);\
					else		\
					gpio_bit_reset(Port_DC_Pin,DC_Pin)
					
#define lcd_cs1(a)	if (a)	\
					gpio_bit_set(Port_CS1_Pin,CS1_Pin);\
					else		\
					gpio_bit_reset(Port_CS1_Pin,CS1_Pin)
					
#define Rom_OUT(a)	if (a)	\
					gpio_bit_set(Port_FS0_Pin,FS0_Pin);\
					else		\
					gpio_bit_reset(Port_FS0_Pin,FS0_Pin)
			
#define Rom_CS(a)	if (a)	\
					gpio_bit_set(Port_CS2_Pin,CS2_Pin);\
					else		\
					gpio_bit_reset(Port_CS2_Pin,CS2_Pin)

#define ROM_OUT    gpio_input_bit_get(Port_FS0_Pin,FS0_Pin)
					
					


extern unsigned char const bmp1[];
extern unsigned char const jiong1[];
extern unsigned char const lei1[];

#define uchar unsigned char 
#define uint unsigned int
#define ulong unsigned long

#define Max_Column	128
#define SIZE 16
void OLED_GPIO_Config(void);
void GBZK_GPIO_Config(void);
/*写指令到LCD模块*/
void transfer_command_lcd(int data1);   
/*写数据到LCD模块*/
void transfer_data_lcd(int data1);
/*延时*/
void delay(int n_ms);               
/*LCD模块初始化*/
void initial_lcd(void);
void lcd_address(unsigned char page,unsigned char column);
/*全屏清屏*/
void clear_screen(void);
void clear_sector(uint8_t x,uint8_t y);
/*显示128x64点阵图像*/
void display_128x64(const unsigned char *dp);
/*显示132x64点阵图像*/
//void display_132x64(unsigned char *dp);

/*显示16x16点阵图像、汉字、生僻字或16x16点阵的其他图标*/
void display_graphic_16x16(unsigned int page,unsigned int column, unsigned char *dp);

void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size);
uint32_t oled_pow(uint8_t m,uint8_t n);
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr);


/*显示8x16点阵图像、ASCII, 或8x16点阵的自造字符、其他图标*/
void display_graphic_8x16(unsigned int page,unsigned char column,unsigned char *dp);

/*显示5*7点阵图像、ASCII, 或5x7点阵的自造字符、其他图标*/
void display_graphic_5x7(unsigned int page,unsigned char column,unsigned char *dp);

/****送指令到晶联讯字库IC***/
void send_command_to_ROM( unsigned char datu );
/****从晶联讯字库IC中取汉字或字符数据（1个字节）***/
static unsigned char get_data_from_ROM(void);
/*从相关地址（addrHigh：地址高字节,addrMid：地址中字节,addrLow：地址低字节）中连续读出DataLen个字节的数据到 pBuff的地址*/
/*连续读取*/
void get_n_bytes_data_from_ROM(unsigned char addrHigh,unsigned char addrMid,unsigned char addrLow,unsigned char *pBuff,unsigned char DataLen );
/******************************************************************/
void display_GB2312_string(unsigned char y,unsigned char x,unsigned char *text);
void display_string_5x7(unsigned char y,unsigned char x,unsigned char *text);


#endif
