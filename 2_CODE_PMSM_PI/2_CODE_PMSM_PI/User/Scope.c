#include "drv_usb_hw.h"
#include "cdc_acm_core.h"
#include <string.h>
#include "Scope.h"
#include "Algorithm.h"
#include "Chart.h"
extern uint16_t UsbRx(usb_dev *udev,uint8_t *buf);
extern uint16_t UsbTx(usb_dev *udev,uint8_t *buf,uint16_t len);
extern usb_core_driver cdc_acm;
extern Control_input Control_Input;	
extern uint16_t time_count;

//测试变量
//extern float PIANZHI_Count;
//extern float Real_id,Real_iq;

#define BYTE0(dwTemp)       (*(char *)(&dwTemp))
#define BYTE1(dwTemp)       (*((char *)(&dwTemp) + 1))
#define BYTE2(dwTemp)       (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp)       (*((char *)(&dwTemp) + 3))
#define SCOPE_NUM 15
float channel[SCOPE_NUM];
float MRecive_data[16]={0}; 	//MATLAB缓存
uint8_t RxDataBuff[16]={0};	//VOFA缓存
uint8_t RxFlag=1;
uint8_t RxLenth;

StrCmd SAppCmd;

//41 54 4A 43 ATJC
//50 4D 53 4D PMSM
uint8_t dataBuff[SCOPE_NUM*4+4];      //缓存大小：通道数量  * 4 + 4

void SendDataFix(void)
{

	  uint32_t cnt = 0;
		
		channel[0]= Algorithm_U.Iabc[0];
    channel[1]= Algorithm_U.Iabc[1];
		channel[2]= Algorithm_U.Iabc[2];
		channel[3]= Algorithm_U.Ref_RPM;
		channel[4]= Algorithm_U.Real_RPM; 
	channel[5]= 0.0f;
channel[6]= 0.0f;
		channel[7]= ERROR_State.STOP;
	 UsbTx(&cdc_acm,(uint8_t*)channel, (uint32_t)8*4);

	
}

void AppCmd(StrCmd *c)
{
	
	if(SCOPE_SELECT == VOFA_SCOPE)
	{
		UsbRx(&cdc_acm,(uint8_t*)RxDataBuff);
		if(RxDataBuff[0] == 0x80 && RxDataBuff[3] == 0x9E)
		{
			if(RxDataBuff[2])
				c->MotroEn = 1;
			else
				c->MotroEn = 0;
		}
		
		if(RxDataBuff[0] == 0x70 && RxDataBuff[5] == 0x8e)
			{
				uint8_t RxHexData1[] = {RxDataBuff[1],RxDataBuff[2],RxDataBuff[3],RxDataBuff[4]};
				float s1 = *(float *)RxHexData1;
				c->SpeedCmdValue = s1;//0-2000		
			}
			if(RxDataBuff[0] == 0x60 && RxDataBuff[5] == 0x7e)
			{
				uint8_t RxHexData1[] = {RxDataBuff[1],RxDataBuff[2],RxDataBuff[3],RxDataBuff[4]};
				float s1 = *(float *)RxHexData1;
				c->PosiCmdValue = s1 * 0.01f;//0-2		
			}
			if(RxDataBuff[0] == 0x50 && RxDataBuff[5] == 0x6e)
			{
				uint8_t RxHexData1[] = {RxDataBuff[1],RxDataBuff[2],RxDataBuff[3],RxDataBuff[4]};
				float s1 = *(float *)RxHexData1;
				c->TorCmdValue = s1 * 0.1f;//0-20		
			}
			
			if(SAppCmd.MotroEn) Go_flag = 1;
			else	Go_flag = 0;
			Iq = SAppCmd.TorCmdValue;
			Set_RPM = SAppCmd.SpeedCmdValue;
			
	}
	else if(SCOPE_SELECT == MATLAB_SCOPE)
	{
		UsbRx(&cdc_acm,(uint8_t*)MRecive_data);
		Set_RPM = MRecive_data[1]; 
		Iq  = MRecive_data[2];
		Go_flag = MRecive_data[3];
	}
		
	
}


