#ifndef RTW_HEADER_Chart_private_h_
#define RTW_HEADER_Chart_private_h_
#include "rtwtypes.h"
#include "Chart.h"
#include "Chart_types.h"

extern void Chart_Low_judge(real32_T rtu_Input, real32_T *rty_Out,
    DW_Low_judge_Chart_T *localDW);
extern void Chart_Over_judge(real32_T rtu_Input, real32_T *rty_Out,
    DW_Over_judge_Chart_T *localDW);

#endif

