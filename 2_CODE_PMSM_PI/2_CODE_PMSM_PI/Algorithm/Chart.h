#ifndef RTW_HEADER_Chart_h_
#define RTW_HEADER_Chart_h_
#ifndef Chart_COMMON_INCLUDES_
#define Chart_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif

#include "Chart_types.h"
#include <stddef.h>
#include <string.h>

#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

typedef struct {
    boolean_T Relay_Mode;
} DW_Low_judge_Chart_T;

typedef struct {
    boolean_T Relay_Mode;
} DW_Over_judge_Chart_T;

typedef struct {
    uint8_T is_active_c3_Chart;
    uint8_T is_c3_Chart;
    DW_Over_judge_Chart_T Over_judge;
    DW_Low_judge_Chart_T Low_judge;
} DW_Chart_T;

typedef struct {
    real32_T Go_Flag;
    real32_T Udc;
} ExtU_Chart_T;

typedef struct {
    real_T Go;
} ExtY_Chart_T;

typedef struct ERROR_State_tag {
    real32_T Over_V;
    real32_T STOP;
    real32_T Low_V;
} ERROR_State_type;

struct tag_RTM_Chart_T {
    const char_T * volatile errorStatus;
};

extern DW_Chart_T Chart_DW;
extern ExtU_Chart_T Chart_U;
extern ExtY_Chart_T Chart_Y;
extern void Chart_initialize(void);
extern void Chart_step(void);
extern void Chart_terminate(void);
extern ERROR_State_type ERROR_State;
extern RT_MODEL_Chart_T *const Chart_M;

#endif

