#include "Chart.h"
#include "rtwtypes.h"
#include "Chart_private.h"
#include <string.h>

#define Chart_IN_NO_ACTIVE_CHILD       ((uint8_T)0U)
#define Chart_IN_None                  ((uint8_T)1U)
#define Chart_IN_Out                   ((uint8_T)2U)

ERROR_State_type ERROR_State;
DW_Chart_T Chart_DW;
ExtU_Chart_T Chart_U;
ExtY_Chart_T Chart_Y;
static RT_MODEL_Chart_T Chart_M_;
RT_MODEL_Chart_T *const Chart_M = &Chart_M_;
void Chart_Low_judge(real32_T rtu_Input, real32_T *rty_Out, DW_Low_judge_Chart_T
                     *localDW)
{
    localDW->Relay_Mode = ((rtu_Input >= 20.0F) || ((rtu_Input > 18.0F) &&
                            localDW->Relay_Mode));
    *rty_Out = (real32_T)!localDW->Relay_Mode;
}

void Chart_Over_judge(real32_T rtu_Input, real32_T *rty_Out,
                      DW_Over_judge_Chart_T *localDW)
{
    localDW->Relay_Mode = ((rtu_Input >= 28.0F) || ((rtu_Input > 25.0F) &&
                            localDW->Relay_Mode));
    *rty_Out = localDW->Relay_Mode;
}

void Chart_step(void)
{
    if (Chart_DW.is_active_c3_Chart == 0U) {
        Chart_DW.is_active_c3_Chart = 1U;
        Chart_DW.is_c3_Chart = Chart_IN_None;
    } else if (Chart_DW.is_c3_Chart == Chart_IN_None) {
        if ((Chart_U.Go_Flag == 1.0F) && (ERROR_State.STOP == 0.0F)) {
            Chart_Over_judge(Chart_U.Udc, &(ERROR_State.Over_V),
                             &Chart_DW.Over_judge);
            Chart_Low_judge(Chart_U.Udc, &(ERROR_State.Low_V),
                            &Chart_DW.Low_judge);
            ERROR_State.STOP = ERROR_State.Low_V + ERROR_State.Over_V;
            Chart_DW.is_c3_Chart = Chart_IN_Out;
        } else {
            Chart_Over_judge(Chart_U.Udc, &(ERROR_State.Over_V),
                             &Chart_DW.Over_judge);
            Chart_Low_judge(Chart_U.Udc, &(ERROR_State.Low_V),
                            &Chart_DW.Low_judge);
            ERROR_State.STOP = ERROR_State.Low_V + ERROR_State.Over_V;
        }
    } else if ((ERROR_State.Over_V == 1.0F) || (ERROR_State.Low_V == 1.0F)) {
        Chart_Y.Go = 0.0;
        Chart_DW.is_c3_Chart = Chart_IN_None;
    } else if (Chart_U.Go_Flag == 0.0F) {
        Chart_DW.is_c3_Chart = Chart_IN_None;
    } else {
        Chart_Over_judge(Chart_U.Udc, &(ERROR_State.Over_V),
                         &Chart_DW.Over_judge);
        Chart_Low_judge(Chart_U.Udc, &(ERROR_State.Low_V), &Chart_DW.Low_judge);
        ERROR_State.STOP = ERROR_State.Low_V + ERROR_State.Over_V;
        Chart_Y.Go = 1.0;
    }
}

void Chart_initialize(void)
{
    rtmSetErrorStatus(Chart_M, (NULL));
    (void) memset((void *)&Chart_DW, 0,
                  sizeof(DW_Chart_T));
    Chart_DW.is_active_c3_Chart = 0U;
    Chart_DW.is_c3_Chart = Chart_IN_NO_ACTIVE_CHILD;
    ERROR_State.Over_V = 0.0F;
    ERROR_State.STOP = 0.0F;
    ERROR_State.Low_V = 0.0F;
}

void Chart_terminate(void)
{
}
