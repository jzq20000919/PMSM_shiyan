#include "Algorithm.h"
#include "rtwtypes.h"
#include "Algorithm_private.h"
#include <math.h>
#include <string.h>

PI_Para_type PI_Para = {

    1500.0F,

    200.0F
};

PI_para_type PI_para = {

    105.0F,

    0.255F,

    18.457F,

    0.18457F
};

DW_Algorithm_T Algorithm_DW;
ExtU_Algorithm_T Algorithm_U;
ExtY_Algorithm_T Algorithm_Y;
static RT_MODEL_Algorithm_T Algorithm_M_;
RT_MODEL_Algorithm_T *const Algorithm_M = &Algorithm_M_;
void Algorithm_Clark_Trans(real32_T rtu_Ia, real32_T rtu_Ib, real32_T rtu_Ic,
    real32_T *rty_I_alpha, real32_T *rty_I_beta)
{
    *rty_I_alpha = rtu_Ib + rtu_Ic;
    *rty_I_alpha *= 0.5F;
    *rty_I_alpha = rtu_Ia - *rty_I_alpha;
    *rty_I_alpha *= 0.666666687F;
    *rty_I_beta = rtu_Ib - rtu_Ic;
    *rty_I_beta = 0.866025388F * *rty_I_beta * 0.666666687F;
}

void Algorit_Current_PI_control_Init(DW_Current_PI_control_Algorit_T *localDW)
{
    localDW->Integrator_DSTATE = 1.0F;
    localDW->Integrator_DSTATE_j = 1.0F;
}

void Algorithm_Current_PI_control(real32_T rtu_refD, real32_T rtu_realD,
    real32_T rtu_refQ, real32_T rtu_realQ, real32_T *rty_Ud, real32_T *rty_Uq,
    DW_Current_PI_control_Algorit_T *localDW)
{
    real32_T rtb_IProdOut;
    real32_T rtb_Sum;
    rtb_Sum = rtu_refD - rtu_realD;
    *rty_Ud = rtb_Sum * PI_para.CUR_P + localDW->Integrator_DSTATE;
    if (*rty_Ud > 20.0F) {
        *rty_Ud = 20.0F;
    } else if (*rty_Ud < -20.0F) {
        *rty_Ud = -20.0F;
    }

    rtb_IProdOut = rtb_Sum * PI_para.CUR_I;
    rtb_Sum = rtu_refQ - rtu_realQ;
    *rty_Uq = PI_Para.current_bandwidth * 0.00017F * rtb_Sum +
        localDW->Integrator_DSTATE_j;
    if (*rty_Uq > 20.0F) {
        *rty_Uq = 20.0F;
    } else if (*rty_Uq < -20.0F) {
        *rty_Uq = -20.0F;
    }

    localDW->Integrator_DSTATE += 0.0001F * rtb_IProdOut;
    localDW->Integrator_DSTATE_j += PI_Para.current_bandwidth * 0.07F * rtb_Sum *
        0.0001F;
}

void Algorithm_Park_Trans(real32_T rtu_alpha, real32_T rtu_beta, const real32_T
    rtu_SinCos[2], real32_T *rty_i_d, real32_T *rty_i_q)
{
    *rty_i_d = rtu_alpha * rtu_SinCos[1];
    *rty_i_d += rtu_beta * rtu_SinCos[0];
    *rty_i_q = rtu_beta * rtu_SinCos[1];
    *rty_i_q -= rtu_alpha * rtu_SinCos[0];
}

void SVPWM_By_Average_Zero_Injection(real32_T rtu_d, real32_T rtu_q, const
    real32_T rtu_SinCos[2], real32_T rtu_Udc, real32_T *rty_Ta, real32_T *rty_Tb,
    real32_T *rty_Tc, real32_T *rty_U_alpha, real32_T *rty_U_beta)
{
    real32_T rtb_Add;
    real32_T rtb_Add2;
    *rty_U_alpha = rtu_d * rtu_SinCos[1];
    *rty_U_alpha -= rtu_q * rtu_SinCos[0];
    rtb_Add2 = 0.5F * *rty_U_alpha;
    *rty_U_beta = rtu_d * rtu_SinCos[0];
    *rty_U_beta += rtu_q * rtu_SinCos[1];
    rtb_Add = 0.866025388F * *rty_U_beta;
    *rty_U_beta = rtb_Add - rtb_Add2;
    rtb_Add2 = (0.0F - rtb_Add2) - rtb_Add;
    rtb_Add = (fmaxf(fmaxf(*rty_U_alpha, *rty_U_beta), rtb_Add2) + fminf(fminf
                (*rty_U_alpha, *rty_U_beta), rtb_Add2)) * -0.5F;
    *rty_U_beta += rtb_Add;
    *rty_Ta = (-(*rty_U_alpha + rtb_Add) / rtu_Udc + 0.5F) * 4199.0F;
    *rty_Tb = (-*rty_U_beta / rtu_Udc + 0.5F) * 4199.0F;
    *rty_Tc = (-(rtb_Add2 + rtb_Add) / rtu_Udc + 0.5F) * 4199.0F;
}

void Algorithm_SpeedPIcontrol2_Init(DW_SpeedPIcontrol2_Algorithm_T *localDW)
{
    localDW->Integrator_DSTATE = 1.0F;
}

real32_T Algorithm_SpeedPIcontrol2(real32_T rtu_Ref_RPM, real32_T rtu_Real_RPM,
    DW_SpeedPIcontrol2_Algorithm_T *localDW)
{
    real32_T rty_Iq_out_0;
    real32_T rtb_Sum1;
    rtb_Sum1 = rtu_Ref_RPM - rtu_Real_RPM;
    rty_Iq_out_0 = (rtb_Sum1 * PI_para.SPEED_P + localDW->Integrator_DSTATE) -
        5.8E-5F * PI_Para.speed_bandwidth / 0.031425F * rtu_Real_RPM;
    if (rty_Iq_out_0 > 20.0F) {
        rty_Iq_out_0 = 20.0F;
    } else if (rty_Iq_out_0 < -20.0F) {
        rty_Iq_out_0 = -20.0F;
    }

    localDW->Integrator_DSTATE += rtb_Sum1 * PI_para.SPEED_I * 0.0001F;
    return rty_Iq_out_0;
}

void Algorithm_SpeedSMCcontrol1_Init(DW_SpeedSMCcontrol1_Algorithm_T *localDW)
{
    localDW->DiscreteTimeIntegrator_DSTATE = 0.001F;
}

real32_T Algorithm_SpeedSMCcontrol1(real32_T rtu_Ref_RPM, real32_T rtu_Real_RPM,
    DW_SpeedSMCcontrol1_Algorithm_T *localDW)
{
    real32_T rty_Iq_out_0;
    int32_T tmp;
    real32_T rtb_Kp3;
    real32_T rtb_Sum3;
    real32_T rtb_TSamp;
    real32_T rtb_x1;
    rty_Iq_out_0 = 0.00369132846F * localDW->DiscreteTimeIntegrator_DSTATE;
    if (rty_Iq_out_0 > 20.0F) {
        rty_Iq_out_0 = 20.0F;
    } else if (rty_Iq_out_0 < -20.0F) {
        rty_Iq_out_0 = -20.0F;
    }

    rtb_x1 = (rtu_Ref_RPM - rtu_Real_RPM) * 0.104719758F;
    rtb_TSamp = rtb_x1 * 10000.0F;
    rtb_Sum3 = rtb_TSamp - localDW->UD_DSTATE;
    rtb_Kp3 = 60.0F * rtb_Sum3;
    rtb_Sum3 += 60.0F * rtb_x1;
    if (rtb_Sum3 < 0.0F) {
        tmp = -1;
    } else {
        tmp = (rtb_Sum3 > 0.0F);
    }

    localDW->DiscreteTimeIntegrator_DSTATE += ((300.0F * rtb_Sum3 + 200.0F *
        (real32_T)tmp) - rtb_Kp3) * 0.0001F;
    localDW->UD_DSTATE = rtb_TSamp;
    return rty_Iq_out_0;
}

void Algorithm_Theta_to_SinCos(real32_T rtu_Theta_in, real32_T *rty_SinCos,
    real32_T *rty_SinCos_e)
{
    *rty_SinCos = sinf(rtu_Theta_in);
    *rty_SinCos_e = cosf(rtu_Theta_in);
}

void Algorithm_step(void)
{
    real32_T rtb_TmpSignalConversionAtPark_0[2];
    real32_T rtb_Add1;
    real32_T rtb_Saturation;
    real32_T rtb_Saturation2;
    real32_T rtb_i_d;
    if (Algorithm_U.Enable > 0.0) {
        Algorithm_Theta_to_SinCos(Algorithm_U.Real_Theta, &rtb_Add1, &rtb_i_d);
        rtb_TmpSignalConversionAtPark_0[0] = rtb_Add1;
        rtb_TmpSignalConversionAtPark_0[1] = rtb_i_d;
        Algorithm_Clark_Trans(Algorithm_U.Iabc[0], Algorithm_U.Iabc[1],
                              Algorithm_U.Iabc[2], &rtb_Saturation2, &rtb_i_d);
        Algorithm_Park_Trans(rtb_Saturation2, rtb_i_d,
                             rtb_TmpSignalConversionAtPark_0, &rtb_Saturation,
                             &rtb_Saturation2);
        rtb_Add1 = Algorithm_SpeedPIcontrol2(Algorithm_U.Ref_RPM,
            Algorithm_U.Real_RPM, &Algorithm_DW.SpeedPIcontrol2);
        rtb_i_d = Algorithm_SpeedSMCcontrol1(Algorithm_U.Ref_RPM,
            Algorithm_U.Real_RPM, &Algorithm_DW.SpeedSMCcontrol1);
        Algorithm_Current_PI_control(Algorithm_U.Ref_Id, rtb_Saturation,
            rtb_Add1, rtb_Saturation2, &rtb_Saturation, &rtb_Saturation2,
            &Algorithm_DW.Current_PI_control);
        SVPWM_By_Average_Zero_Injection(rtb_Saturation, rtb_Saturation2,
            rtb_TmpSignalConversionAtPark_0, Algorithm_U.Udc, &Algorithm_DW.Gain,
            &Algorithm_DW.Gain1, &Algorithm_DW.Gain2, &rtb_i_d, &rtb_Add1);
    }

    Algorithm_Y.Tabc[0] = Algorithm_DW.Gain;
    Algorithm_Y.Tabc[1] = Algorithm_DW.Gain1;
    Algorithm_Y.Tabc[2] = Algorithm_DW.Gain2;
}

void Algorithm_initialize(void)
{
    rtmSetErrorStatus(Algorithm_M, (NULL));
    (void) memset((void *)&Algorithm_DW, 0,
                  sizeof(DW_Algorithm_T));
    Algorithm_SpeedPIcontrol2_Init(&Algorithm_DW.SpeedPIcontrol2);
    Algorithm_SpeedSMCcontrol1_Init(&Algorithm_DW.SpeedSMCcontrol1);
    Algorit_Current_PI_control_Init(&Algorithm_DW.Current_PI_control);
}

void Algorithm_terminate(void)
{
}
