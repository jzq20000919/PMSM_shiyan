#ifndef SCOPE_H
#define SCOPE_H
#include "gd32f4xx_it.h"


#define MATLAB_SCOPE 0
#define VOFA_SCOPE 1
#define SCOPE_SELECT	MATLAB_SCOPE

typedef struct
{
	uint8_t MotroEn;
	float SpeedCmdValue;
	float PosiCmdValue;
	float TorCmdValue;
}StrCmd;

extern StrCmd SAppCmd;
extern float Set_RPM;
extern float Iq;
extern uint8_t Go_flag;
void AppCmd(StrCmd *c);

#endif


