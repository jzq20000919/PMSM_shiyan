#ifndef Algorithm_h_
#define Algorithm_h_
#ifndef Algorithm_COMMON_INCLUDES_
#define Algorithm_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif

#include "Algorithm_types.h"
#include <stddef.h>
#include <string.h>

#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

typedef struct {
    real32_T Integrator_DSTATE;
    real32_T Integrator_DSTATE_j;
} DW_Current_PI_control_Algorit_T;

typedef struct {
    real32_T Integrator_DSTATE;
} DW_SpeedPIcontrol2_Algorithm_T;

typedef struct {
    real32_T DiscreteTimeIntegrator_DSTATE;
    real32_T UD_DSTATE;
} DW_SpeedSMCcontrol1_Algorithm_T;

typedef struct {
    DW_SpeedSMCcontrol1_Algorithm_T SpeedSMCcontrol1;
    DW_SpeedPIcontrol2_Algorithm_T SpeedPIcontrol2;
    DW_Current_PI_control_Algorit_T Current_PI_control;
    real32_T Gain;
    real32_T Gain1;
    real32_T Gain2;
} DW_Algorithm_T;

typedef struct {
    real32_T Real_Theta;
    real32_T Ref_Id;
    real32_T Ref_Iq;
    real32_T Ref_RPM;
    real32_T Iabc[3];
    real32_T Real_RPM;
    real32_T Udc;
    real_T Enable;
} ExtU_Algorithm_T;

typedef struct {
    real32_T Tabc[3];
} ExtY_Algorithm_T;

typedef struct PI_Para_tag {
    real32_T current_bandwidth;
    real32_T speed_bandwidth;
} PI_Para_type;

typedef struct PI_para_tag {
    real32_T CUR_I;
    real32_T CUR_P;
    real32_T SPEED_I;
    real32_T SPEED_P;
} PI_para_type;

struct tag_RTM_Algorithm_T {
    const char_T * volatile errorStatus;
};

extern DW_Algorithm_T Algorithm_DW;
extern ExtU_Algorithm_T Algorithm_U;
extern ExtY_Algorithm_T Algorithm_Y;
extern void Algorithm_initialize(void);
extern void Algorithm_step(void);
extern void Algorithm_terminate(void);
extern PI_Para_type PI_Para;
extern PI_para_type PI_para;
extern RT_MODEL_Algorithm_T *const Algorithm_M;

#endif

