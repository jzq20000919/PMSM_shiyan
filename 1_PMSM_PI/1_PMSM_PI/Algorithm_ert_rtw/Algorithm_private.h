#ifndef Algorithm_private_h_
#define Algorithm_private_h_
#include "rtwtypes.h"
#include "Algorithm.h"
#include "Algorithm_types.h"

extern void Algorithm_Clark_Trans(real32_T rtu_Ia, real32_T rtu_Ib, real32_T
    rtu_Ic, real32_T *rty_I_alpha, real32_T *rty_I_beta);
extern void Algorit_Current_PI_control_Init(DW_Current_PI_control_Algorit_T
    *localDW);
extern void Algorithm_Current_PI_control(real32_T rtu_refD, real32_T rtu_realD,
    real32_T rtu_refQ, real32_T rtu_realQ, real32_T *rty_Ud, real32_T *rty_Uq,
    DW_Current_PI_control_Algorit_T *localDW);
extern void Algorithm_Park_Trans(real32_T rtu_alpha, real32_T rtu_beta, const
    real32_T rtu_SinCos[2], real32_T *rty_i_d, real32_T *rty_i_q);
extern void SVPWM_By_Average_Zero_Injection(real32_T rtu_d, real32_T rtu_q,
    const real32_T rtu_SinCos[2], real32_T rtu_Udc, real32_T *rty_Ta, real32_T
    *rty_Tb, real32_T *rty_Tc, real32_T *rty_U_alpha, real32_T *rty_U_beta);
extern void Algorithm_SpeedPIcontrol2_Init(DW_SpeedPIcontrol2_Algorithm_T
    *localDW);
extern real32_T Algorithm_SpeedPIcontrol2(real32_T rtu_Ref_RPM, real32_T
    rtu_Real_RPM, DW_SpeedPIcontrol2_Algorithm_T *localDW);
extern void Algorithm_SpeedSMCcontrol1_Init(DW_SpeedSMCcontrol1_Algorithm_T
    *localDW);
extern real32_T Algorithm_SpeedSMCcontrol1(real32_T rtu_Ref_RPM, real32_T
    rtu_Real_RPM, DW_SpeedSMCcontrol1_Algorithm_T *localDW);
extern void Algorithm_Theta_to_SinCos(real32_T rtu_Theta_in, real32_T
    *rty_SinCos, real32_T *rty_SinCos_e);

#endif

