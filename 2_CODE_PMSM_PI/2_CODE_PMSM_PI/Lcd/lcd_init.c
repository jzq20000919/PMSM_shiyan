#include "lcd_init.h"


extern void delay_1ms(uint32_t count);
extern void usb_udelay (const uint32_t usec);
static void delay_ms(uint32_t ms)
{
		delay_1ms(ms);
}


void LcdGpioInit(void)
{

	rcu_periph_clock_enable(RCU_GPIOC);//启用外设时钟功能  
	rcu_periph_clock_enable(RCU_GPIOA);//启用外设时钟功能  
	rcu_periph_clock_enable(RCU_GPIOB);
	
	
	/**LCD GPIO INIT**/
	gpio_mode_set(LCD_SCL_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LCD_SCL_Pin);//SCL
	gpio_output_options_set(LCD_SCL_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, LCD_SCL_Pin);
	gpio_bit_set(LCD_SCL_GPIO_Port,LCD_SCL_Pin);

	gpio_mode_set(LCD_SDA_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LCD_SDA_Pin);//SDA
	gpio_output_options_set(LCD_SDA_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, LCD_SDA_Pin);
	gpio_bit_set(LCD_SDA_GPIO_Port,LCD_SDA_Pin);

	gpio_mode_set(LCD_RES_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LCD_RES_Pin);//RES
	gpio_output_options_set(LCD_RES_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, LCD_RES_Pin);

	gpio_mode_set(LCD_DC_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LCD_DC_Pin);//DC
	gpio_output_options_set(LCD_DC_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, LCD_DC_Pin);

	gpio_mode_set(LCD_CS_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LCD_CS_Pin);//CS
	gpio_output_options_set(LCD_CS_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, LCD_CS_Pin);

	gpio_mode_set(LCD_BLK_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LCD_BLK_Pin);//BLK
	gpio_output_options_set(LCD_BLK_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, LCD_BLK_Pin);

	gpio_bit_set(LCD_RES_GPIO_Port,LCD_RES_Pin);
	gpio_bit_set(LCD_DC_GPIO_Port,LCD_DC_Pin);
	gpio_bit_set(LCD_CS_GPIO_Port,LCD_CS_Pin);
	
}

/******************************************************************************
      函数说明：LCD串行数据写入函数
      入口数据：dat  要写入的串行数据
      返回值：  无
******************************************************************************/
void LCD_Writ_Bus(uint8_t dat) 
{	
	uint8_t i;
	LCD_CS_Clr();
	for(i=0;i<8;i++)
	{			  
		LCD_SCLK_Clr();
		if(dat&0x80)
		{
		   LCD_MOSI_Set();
		}
		else
		{
		   LCD_MOSI_Clr();
		}
		
		LCD_SCLK_Set();
		dat<<=1;
		
	}	
//	while(RESET == spi_i2s_flag_get(SPI0,SPI_FLAG_TBE));//等待发送缓存为空
//  spi_i2s_data_transmit(SPI0, dat); 
//  while(SET == spi_i2s_flag_get(SPI0, SPI_FLAG_TRANS));//等待数据发送完毕。SPI通信进行中标志位（TRANS）
  LCD_CS_Set();	

}


/******************************************************************************
      函数说明：LCD写入数据
      入口数据：dat 写入的数据
      返回值：  无
******************************************************************************/
void LCD_WR_DATA8(uint8_t dat)
{
	LCD_Writ_Bus(dat);
}


/******************************************************************************
      函数说明：LCD写入数据
      入口数据：dat 写入的数据
      返回值：  无
******************************************************************************/
void LCD_WR_DATA(uint16_t dat)
{
	LCD_Writ_Bus(dat>>8);
	LCD_Writ_Bus(dat);
}


/******************************************************************************
      函数说明：LCD写入命令
      入口数据：dat 写入的命令
      返回值：  无
******************************************************************************/
void LCD_WR_REG(uint8_t dat)
{
	LCD_DC_Clr();//写命令
	LCD_Writ_Bus(dat);
	LCD_DC_Set();//写数据
}


/******************************************************************************
      函数说明：设置起始和结束地址
      入口数据：x1,x2 设置列的起始和结束地址
                y1,y2 设置行的起始和结束地址
      返回值：  无
******************************************************************************/
void LCD_Address_Set(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2)
{
	if(USE_HORIZONTAL==0)
	{
		LCD_WR_REG(0x2a);//列地址设置
		LCD_WR_DATA(x1+24);
		LCD_WR_DATA(x2+24);
		LCD_WR_REG(0x2b);//行地址设置
		LCD_WR_DATA(y1);
		LCD_WR_DATA(y2);
		LCD_WR_REG(0x2c);//储存器写
	}
	else if(USE_HORIZONTAL==1)
	{
		LCD_WR_REG(0x2a);//列地址设置
		LCD_WR_DATA(x1+24);
		LCD_WR_DATA(x2+24);
		LCD_WR_REG(0x2b);//行地址设置
		LCD_WR_DATA(y1);
		LCD_WR_DATA(y2);
		LCD_WR_REG(0x2c);//储存器写
	}
	else if(USE_HORIZONTAL==2)
	{
		LCD_WR_REG(0x2a);//列地址设置
		LCD_WR_DATA(x1);
		LCD_WR_DATA(x2);
		LCD_WR_REG(0x2b);//行地址设置
		LCD_WR_DATA(y1+24);
		LCD_WR_DATA(y2+24);
		LCD_WR_REG(0x2c);//储存器写
	}
	else
	{
		LCD_WR_REG(0x2a);//列地址设置
		LCD_WR_DATA(x1);
		LCD_WR_DATA(x2);
		LCD_WR_REG(0x2b);//行地址设置
		LCD_WR_DATA(y1+24);
		LCD_WR_DATA(y2+24);
		LCD_WR_REG(0x2c);//储存器写
	}
}

void LCD_Init(void)
{

	LcdGpioInit(); 
	
	LCD_RES_Clr();//复位
	delay_ms(100);
	LCD_RES_Set();
	delay_ms(100);
	
	LCD_BLK_Set();//打开背光
  delay_ms(100);
	
	LCD_WR_REG(0x11);     //Sleep out
	delay_ms(120);                //Delay 120ms
	
	LCD_WR_REG(0xB1);     //Normal mode
	LCD_WR_DATA8(0x05);   
	LCD_WR_DATA8(0x3C);   
	LCD_WR_DATA8(0x3C);   
	LCD_WR_REG(0xB2);     //Idle mode
	LCD_WR_DATA8(0x05);   
	LCD_WR_DATA8(0x3C);   
	LCD_WR_DATA8(0x3C);   
	LCD_WR_REG(0xB3);     //Partial mode
	LCD_WR_DATA8(0x05);   
	LCD_WR_DATA8(0x3C);   
	LCD_WR_DATA8(0x3C);   
	LCD_WR_DATA8(0x05);   
	LCD_WR_DATA8(0x3C);   
	LCD_WR_DATA8(0x3C);   
	LCD_WR_REG(0xB4);     //Dot inversion
	LCD_WR_DATA8(0x03);   
	
	LCD_WR_REG(0xC0);     
	LCD_WR_DATA8(0x0E);   
	LCD_WR_DATA8(0x0E);   
	LCD_WR_DATA8(0x04);  
	
	LCD_WR_REG(0xC1);     //VGH VGL
	LCD_WR_DATA8(0xC5);   //C0
	
	LCD_WR_REG(0xC2);     //Normal Mode
	LCD_WR_DATA8(0x0D);   
	LCD_WR_DATA8(0x00);
	
	LCD_WR_REG(0xC3);     //Idle
	LCD_WR_DATA8(0x8D);   
	LCD_WR_DATA8(0x2A);  
	
	LCD_WR_REG(0xC4);     //Partial+Full
	LCD_WR_DATA8(0x8D);   
	LCD_WR_DATA8(0xEE);   
	
	LCD_WR_REG(0xC5);     //VCOM
	LCD_WR_DATA8(0x06);  
	
	LCD_WR_REG(0x36);     //MX, MY, RGB mode
	if(USE_HORIZONTAL==0)LCD_WR_DATA8(0x08);
	else if(USE_HORIZONTAL==1)LCD_WR_DATA8(0xC8);
	else if(USE_HORIZONTAL==2)LCD_WR_DATA8(0x78);
	else LCD_WR_DATA8(0xA8);  

	LCD_WR_REG(0x3A); 
	LCD_WR_DATA8(0x55);
		
	LCD_WR_REG(0xE0);     
	LCD_WR_DATA8(0x0b);   
	LCD_WR_DATA8(0x17);   
	LCD_WR_DATA8(0x0a);   
	LCD_WR_DATA8(0x0d);   
	LCD_WR_DATA8(0x1a);   
	LCD_WR_DATA8(0x19);   
	LCD_WR_DATA8(0x16);   
	LCD_WR_DATA8(0x1d);   
	LCD_WR_DATA8(0x21);   
	LCD_WR_DATA8(0x26);   
	LCD_WR_DATA8(0x37);   
	LCD_WR_DATA8(0x3c);   
	LCD_WR_DATA8(0x00);   
	LCD_WR_DATA8(0x09);   
	LCD_WR_DATA8(0x05);   
	LCD_WR_DATA8(0x10);   

	LCD_WR_REG(0xE1);     
	LCD_WR_DATA8(0x0c);   
	LCD_WR_DATA8(0x19);   
	LCD_WR_DATA8(0x09);   
	LCD_WR_DATA8(0x0d);   
	LCD_WR_DATA8(0x1b);   
	LCD_WR_DATA8(0x19);   
	LCD_WR_DATA8(0x15);   
	LCD_WR_DATA8(0x1d);   
	LCD_WR_DATA8(0x21);   
	LCD_WR_DATA8(0x26);   
	LCD_WR_DATA8(0x39);   
	LCD_WR_DATA8(0x3E);   
	LCD_WR_DATA8(0x00);   
	LCD_WR_DATA8(0x09);   
	LCD_WR_DATA8(0x05);   
	LCD_WR_DATA8(0x10);   
	 
	delay_ms (120);
	LCD_WR_REG(0x29);     //Display on
}








