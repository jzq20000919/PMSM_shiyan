#include "OLED.h"
#include "OLEDku.h" 
void GBZK_GPIO_Config(void)
{		
	 rcu_periph_clock_enable(RCU_GPIOD);//启用外设时钟功能  
	 rcu_periph_clock_enable(RCU_GPIOB);
	
   gpio_mode_set( Port_DC_Pin, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, DC_Pin);
   gpio_output_options_set( Port_DC_Pin, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, DC_Pin);
	
   gpio_mode_set( Port_CS1_Pin , GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, CS1_Pin);
   gpio_output_options_set( Port_CS1_Pin, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, CS1_Pin);
	
	 gpio_mode_set( Port_CS2_Pin, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, CS2_Pin);
   gpio_output_options_set( Port_CS2_Pin, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, CS2_Pin);
	 
   gpio_bit_reset(Port_DC_Pin,DC_Pin);
	 gpio_bit_reset(Port_CS1_Pin,CS1_Pin);
	 gpio_bit_reset(Port_CS2_Pin,CS2_Pin);
	
	 gpio_mode_set( Port_CLK_Pin, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, CLK_Pin);
   gpio_output_options_set( Port_CLK_Pin, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, CLK_Pin);
	
   gpio_mode_set( Port_MOSI_Pin , GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, MOSI_Pin);
   gpio_output_options_set( Port_MOSI_Pin, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, MOSI_Pin);
	 
	 gpio_mode_set( Port_FS0_Pin, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, FS0_Pin);
   gpio_output_options_set( Port_FS0_Pin, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, FS0_Pin);
	
   gpio_bit_reset(Port_CLK_Pin,CLK_Pin);
	 gpio_bit_reset(Port_MOSI_Pin,MOSI_Pin);
	 gpio_bit_reset(Port_FS0_Pin,FS0_Pin);
	

}

void GBZK_ROMOUTSET(void)
{		
	 gpio_mode_set( Port_FS0_Pin, GPIO_MODE_INPUT, GPIO_PUPD_PULLDOWN, FS0_Pin);
 
}

void GBZK_ROMOUTRESET(void)
{			  	 
	 gpio_mode_set( Port_FS0_Pin, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, FS0_Pin);
   gpio_output_options_set( Port_FS0_Pin, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, FS0_Pin);
}

/*写指令到LCD模块*/
void transfer_command_lcd(int data1)   
{
	char i;
	lcd_rs(0);;;
	lcd_cs1(0);
	for(i=0;i<8;i++)
   {lcd_sclk(0);;;
		if(data1&0x80) {lcd_sid(1);;;}
		else {lcd_sid(0);;;}
		lcd_sclk(1);
		__nop();;;
	//	lcd_sclk(0);;;
	 	data1<<=1;
   }
	 	lcd_rs(1);;;
	 lcd_cs1(1);
}

/*写数据到LCD模块*/
void transfer_data_lcd(int data1)
{
	char i;
	lcd_rs(1);;;
	lcd_cs1(0);
	for(i=0;i<8;i++)
   {
		lcd_sclk(0);;;
		if(data1&0x80) {lcd_sid(1);;;}
		else {lcd_sid(0);;;}
		lcd_sclk(1);;;
		__nop();;;
		//lcd_sclk(0);;;
	 	data1<<=1;
   }lcd_rs(1);;;
	 lcd_cs1(1);
}

/*LCD模块初始化*/
void initial_lcd()
{

	GBZK_GPIO_Config();
	lcd_cs1(0);
	Rom_CS(1);
	delay_1ms(50);
  transfer_command_lcd(0xAE);   //display off
	transfer_command_lcd(0x20);	//Set Memory Addressing Mode	
	transfer_command_lcd(0x10);	//00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	transfer_command_lcd(0xb0);	//Set Page Start Address for Page Addressing Mode,0-7
	transfer_command_lcd(0xc8);	//Set COM Output Scan Direction
	transfer_command_lcd(0x00);//---set low column address
	transfer_command_lcd(0x10);//---set high column address
	transfer_command_lcd(0x40);//--set start line address
	transfer_command_lcd(0x81);//--set contrast control register
	transfer_command_lcd(0xFF);
	transfer_command_lcd(0xa1);//--set segment re-map 0 to 127
	transfer_command_lcd(0xa6);//--set normal display
	transfer_command_lcd(0xa8);//--set multiplex ratio(1 to 64)
	transfer_command_lcd(0x3F);//
	transfer_command_lcd(0xa4);//0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	transfer_command_lcd(0xd3);//-set display offset
	transfer_command_lcd(0x00);//-not offset
	transfer_command_lcd(0xd5);//--set display clock divide ratio/oscillator frequency
	transfer_command_lcd(0xf0);//--set divide ratio
	transfer_command_lcd(0xd9);//--set pre-charge period
	transfer_command_lcd(0x22); //
	transfer_command_lcd(0xda);//--set com pins hardware configuration
	transfer_command_lcd(0x12);
	transfer_command_lcd(0xdb);//--set vcomh
	transfer_command_lcd(0x20);//0x20,0.77xVcc
	transfer_command_lcd(0x8d);//--set DC-DC enable
	transfer_command_lcd(0x14);//
	transfer_command_lcd(0xaf);//--turn on oled panel 
	lcd_cs1(1);
	clear_screen();                                  //clear all dots 
}

void lcd_address(uchar page,uchar column)
{
  transfer_command_lcd(0xb0 + column);   /*设置页地址*/
	transfer_command_lcd(((page & 0xf0) >> 4) | 0x10);	/*设置列地址的高4位*/
	transfer_command_lcd((page & 0x0f) | 0x00);	/*设置列地址的低4位*/	
}

/*全屏清屏*/
void clear_screen()
{
	unsigned char i,j;
	lcd_cs1(0);
	Rom_CS(1);	
	for(i=0;i<8;i++)
	{
		transfer_command_lcd(0xb0+i);
		transfer_command_lcd(0x00);
		transfer_command_lcd(0x10);
		for(j=0;j<128;j++)
		{
		  	transfer_data_lcd(0x00);
		}
	}
 	lcd_cs1(1);
}

void clear_sector(uint8_t x,uint8_t y)
{
	unsigned char i,j;
	lcd_cs1(0);
	Rom_CS(1);	
	for(i=0;i<x;i++)
	{
		transfer_command_lcd(0xb0+i);
		transfer_command_lcd(0x00);
		transfer_command_lcd(0x10);
		for(j=0;j<y;j++)
		{
		  	transfer_data_lcd(0x00);
		}
	}
 	lcd_cs1(1);
}
/*显示128x64点阵图像*/
//void display_128x64(const uchar *dp)
//{
//	uint i,j;
//	lcd_cs1(0);
//	for(j=0;j<8;j++)
//	{
//		lcd_address(0,j);
//		for (i=0;i<128;i++)
//		{	
//			transfer_data_lcd(*dp);					/*写数据到LCD,每写完一个8位的数据后列地址自动加1*/
//			dp++;
//		}
//	}
//	lcd_cs1(1);
//}

/*显示16x16点阵图像、汉字、生僻字或16x16点阵的其他图标*/
void display_graphic_16x16(unsigned int page,unsigned int column, unsigned char *dp)
{
	uint i,j;
 	lcd_cs1(0);
	Rom_CS(1); 	
	for(j=2;j>0;j--)
	{
		lcd_address(column,page);
		for (i=0;i<16;i++)
		{	
			transfer_data_lcd(*dp);					/*写数据到LCD,每写完一个8位的数据后列地址自动加1*/
			dp++;
		}
		page++;
	}
	lcd_cs1(1);
}

//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//mode:0,反白显示;1,正常显示				 
//size:选择字体 16/12 
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr)
{      	
	unsigned char c=0,i=0;	
		c=chr-' ';//得到偏移后的值			
		if(x>Max_Column-1){x=0;y=y+2;}
		  if(SIZE ==16)
			{
			lcd_address(x,y);	
			for(i=0;i<8;i++)
			transfer_data_lcd(F8X16[c*16+i]);
			lcd_address(x,y+1);
			for(i=0;i<8;i++)
			transfer_data_lcd(F8X16[c*16+i+8]);
			}
			else 
			{	
				lcd_address(x,y+1);
				for(i=0;i<6;i++)
				transfer_data_lcd(F6x8[c][i]);
			}
}

//m^n函数
uint32_t oled_pow(uint8_t m,uint8_t n)
{
	uint32_t result=1;	 
	while(n--)result*=m;    
	return result;
}				  
//显示2个数字
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295);	 		  
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size)
{         	
	uint8_t t,temp;
	uint8_t enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(y+(size/2)*t,x,' ');
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(y+(size/2)*t,x,temp+'0'); 
	}
} 



/*显示8x16点阵图像、ASCII, 或8x16点阵的自造字符、其他图标*/
void display_graphic_8x16(uint page,uchar column,uchar *dp)
{
	uint i,j;
	lcd_cs1(0);	
	for(j=2;j>0;j--)
	{
		lcd_address(column,page);
		for (i=0;i<8;i++)
		{	
			transfer_data_lcd(*dp);					/*写数据到LCD,每写完一个8位的数据后列地址自动加1*/
			dp++;
		}
		page++;
	}
	lcd_cs1(1);
}


/*显示5*7点阵图像、ASCII, 或5x7点阵的自造字符、其他图标*/
void display_graphic_5x7(uint page,uchar column,uchar *dp)
{
	uint col_cnt;
	uchar page_address;
	uchar column_address_L,column_address_H;
	page_address = 0xb0+page-1;
	lcd_cs1(0);	
	column_address_L =(column&0x0f)-1;
	column_address_H =((column>>4)&0x0f)+0x10;
	transfer_command_lcd(page_address); 		/*Set Page Address*/
	transfer_command_lcd(column_address_H);	/*Set MSB of column Address*/
	transfer_command_lcd(column_address_L);	/*Set LSB of column Address*/
	
	for (col_cnt=0;col_cnt<6;col_cnt++)
	{	
		transfer_data_lcd(*dp);
		dp++;
	}
	lcd_cs1(1);
}

/****送指令到晶联讯字库IC***/
void send_command_to_ROM( uchar datu )
{
	uchar i;
	for(i=0;i<8;i++ )
	{
		if(datu&0x80)
			{lcd_sid(1);;;__nop();}
		else
			{lcd_sid(0);;; __nop(); }
			datu = datu<<1;
			lcd_sclk(0);;;__nop();
			lcd_sclk(1);;;__nop();
	}
}

/****从晶联讯字库IC中取汉字或字符数据（1个字节）***/
static uchar get_data_from_ROM( )
{
	
	uchar i;
	uchar ret_data=0;
	lcd_sclk(1);;;  __nop();
	GBZK_ROMOUTSET();
	for(i=0;i<8;i++)
	{
		Rom_OUT(1);;; __nop();
		lcd_sclk(0);;;__nop();
		ret_data=ret_data<<1;
		if( ROM_OUT )
			{ret_data=ret_data+1;;;__nop();}
		else
			{ret_data=ret_data+0;;;__nop();	}
		lcd_sclk(1);
	}
	GBZK_ROMOUTRESET();
	return(ret_data);
}

/*从相关地址（addrHigh：地址高字节,addrMid：地址中字节,addrLow：地址低字节）中连续读出DataLen个字节的数据到 pBuff的地址*/
/*连续读取*/
void get_n_bytes_data_from_ROM(uchar addrHigh,uchar addrMid,uchar addrLow,uchar *pBuff,uchar DataLen )
{
	uchar i;
	Rom_CS(0);
	lcd_cs1(1);	
	lcd_sclk(0);
	send_command_to_ROM(0x03);
	send_command_to_ROM(addrHigh);
	send_command_to_ROM(addrMid);
	send_command_to_ROM(addrLow);
	for(i = 0; i < DataLen; i++ )
	     *(pBuff+i) =get_data_from_ROM();
	Rom_CS(1);
}

ulong  fontaddr=0;
void display_GB2312_string(uchar y,uchar x,uchar *text)
{
	uchar i= 0;
	uchar addrHigh,addrMid,addrLow ;
	uchar fontbuf[32];			
	while((text[i]>0x00))
	{
		if(((text[i]>=0xb0) &&(text[i]<=0xf7))&&(text[i+1]>=0xa1))
		{						
			/*国标简体（GB2312）汉字在晶联讯字库IC中的地址由以下公式来计算：*/
			/*Address = ((MSB - 0xB0) * 94 + (LSB - 0xA1)+ 846)*32+ BaseAdd;BaseAdd=0*/
			/*由于担心8位单片机有乘法溢出问题，所以分三部取地址*/
			fontaddr = (text[i]- 0xb0)*94; 
			fontaddr += (text[i+1]-0xa1)+846;
			fontaddr = (ulong)(fontaddr*32);
			
			addrHigh = (fontaddr&0xff0000)>>16;  /*地址的高8位,共24位*/
			addrMid = (fontaddr&0xff00)>>8;      /*地址的中8位,共24位*/
			addrLow = fontaddr&0xff;	     /*地址的低8位,共24位*/
			get_n_bytes_data_from_ROM(addrHigh,addrMid,addrLow,fontbuf,32 );/*取32个字节的数据，存到"fontbuf[32]"*/
			display_graphic_16x16(y,x,fontbuf);/*显示汉字到LCD上，y为页地址，x为列地址，fontbuf[]为数据*/
			i+=2;
			x+=16;
		}
		else if(((text[i]>=0xa1) &&(text[i]<=0xa3))&&(text[i+1]>=0xa1))
		{						
			/*国标简体（GB2312）15x16点的字符在晶联讯字库IC中的地址由以下公式来计算：*/
			/*Address = ((MSB - 0xa1) * 94 + (LSB - 0xA1))*32+ BaseAdd;BaseAdd=0*/
			/*由于担心8位单片机有乘法溢出问题，所以分三部取地址*/
			fontaddr = (text[i]- 0xa1)*94; 
			fontaddr += (text[i+1]-0xa1);
			fontaddr = (ulong)(fontaddr*32);
			
			addrHigh = (fontaddr&0xff0000)>>16;  /*地址的高8位,共24位*/
			addrMid = (fontaddr&0xff00)>>8;      /*地址的中8位,共24位*/
			addrLow = fontaddr&0xff;	     /*地址的低8位,共24位*/
			get_n_bytes_data_from_ROM(addrHigh,addrMid,addrLow,fontbuf,32 );/*取32个字节的数据，存到"fontbuf[32]"*/
			display_graphic_16x16(y,x,fontbuf);/*显示汉字到LCD上，y为页地址，x为列地址，fontbuf[]为数据*/
			i+=2;
			x+=16;
		}
		else if((text[i]>=0x20) &&(text[i]<=0x7e))	
		{						
			unsigned char fontbuf[16];			
			fontaddr = (text[i]- 0x20);
			fontaddr = (unsigned long)(fontaddr*16);
			fontaddr = (unsigned long)(fontaddr+0x3cf80);			
			addrHigh = (fontaddr&0xff0000)>>16;
			addrMid = (fontaddr&0xff00)>>8;
			addrLow = fontaddr&0xff;
      get_n_bytes_data_from_ROM(addrHigh,addrMid,addrLow,fontbuf,16 );/*取16个字节的数据，存到"fontbuf[32]"*/
			display_graphic_8x16(y,x,fontbuf);/*显示8x16的ASCII字到LCD上，y为页地址，x为列地址，fontbuf[]为数据*/
			i+=1;
			x+=8;
		}
		else
			i++;	
	}
}

void display_string_5x7(uchar y,uchar x,uchar *text)
{
	unsigned char i= 0;
	unsigned char addrHigh,addrMid,addrLow ;
	while((text[i]>0x00))
	{
		if((text[i]>=0x20) &&(text[i]<=0x7e))	
		{						
			unsigned char fontbuf[8];			
			fontaddr = (text[i]- 0x20);
			fontaddr = (unsigned long)(fontaddr*8);
			fontaddr = (unsigned long)(fontaddr+0x3bfc0);			
			addrHigh = (fontaddr&0xff0000)>>16;
			addrMid = (fontaddr&0xff00)>>8;
			addrLow = fontaddr&0xff;
      get_n_bytes_data_from_ROM(addrHigh,addrMid,addrLow,fontbuf,8);/*取8个字节的数据，存到"fontbuf[32]"*/
			display_graphic_5x7(y,x,fontbuf);/*显示5x7的ASCII字到LCD上，y为页地址，x为列地址，fontbuf[]为数据*/
			i+=1;
			x+=6;
		}
		else
		i++;	
	}
}


