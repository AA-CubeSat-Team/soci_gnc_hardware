/*
 * arduinoTest.c
 *
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * Code generation for model "arduinoTest".
 *
 * Model version              : 1.2
 * Simulink Coder version : 9.0 (R2018b) 24-May-2018
 * C source code generated on : Sun Feb 21 19:04:10 2021
 *
 * Target selection: grt.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "arduinoTest.h"
#include "arduinoTest_private.h"

/* Block states (default storage) */
DW_arduinoTest_T arduinoTest_DW;

/* Real-time model */
RT_MODEL_arduinoTest_T arduinoTest_M_;
RT_MODEL_arduinoTest_T *const arduinoTest_M = &arduinoTest_M_;

/* Forward declaration for local functions */
static codertarget_arduinobase_int_i_T *arduinoI2CWrite_arduinoI2CWrite
  (codertarget_arduinobase_int_i_T *obj);
static void arduinoTest_SystemCore_release(const codertarget_arduinobase_int_i_T
  *obj);
static void arduinoTest_SystemCore_delete(const codertarget_arduinobase_int_i_T *
  obj);
static void matlabCodegenHandle_matlabCodeg(codertarget_arduinobase_int_i_T *obj);
static codertarget_arduinobase_int_i_T *arduinoI2CWrite_arduinoI2CWrite
  (codertarget_arduinobase_int_i_T *obj)
{
  codertarget_arduinobase_int_i_T *b_obj;
  obj->isInitialized = 0;
  b_obj = obj;
  obj->Hw.AvailablePwmPinNames.f1 = '2';
  obj->Hw.AvailablePwmPinNames.f2 = '3';
  obj->Hw.AvailablePwmPinNames.f3 = '4';
  obj->Hw.AvailablePwmPinNames.f4 = '5';
  obj->Hw.AvailablePwmPinNames.f5 = '6';
  obj->Hw.AvailablePwmPinNames.f6 = '7';
  obj->Hw.AvailablePwmPinNames.f7 = '8';
  obj->Hw.AvailablePwmPinNames.f8 = '9';
  obj->Hw.AvailablePwmPinNames.f9[0] = '1';
  obj->Hw.AvailablePwmPinNames.f9[1] = '0';
  obj->Hw.AvailablePwmPinNames.f10[0] = '1';
  obj->Hw.AvailablePwmPinNames.f10[1] = '1';
  obj->Hw.AvailablePwmPinNames.f11[0] = '1';
  obj->Hw.AvailablePwmPinNames.f11[1] = '2';
  obj->Hw.AvailablePwmPinNames.f12[0] = '1';
  obj->Hw.AvailablePwmPinNames.f12[1] = '3';
  obj->matlabCodegenIsDeleted = false;
  return b_obj;
}

static void arduinoTest_SystemCore_release(const codertarget_arduinobase_int_i_T
  *obj)
{
  if ((obj->isInitialized == 1) && obj->isSetupComplete) {
    MW_I2C_Close(obj->MW_I2C_HANDLE);
  }
}

static void arduinoTest_SystemCore_delete(const codertarget_arduinobase_int_i_T *
  obj)
{
  arduinoTest_SystemCore_release(obj);
}

static void matlabCodegenHandle_matlabCodeg(codertarget_arduinobase_int_i_T *obj)
{
  if (!obj->matlabCodegenIsDeleted) {
    obj->matlabCodegenIsDeleted = true;
    arduinoTest_SystemCore_delete(obj);
  }
}

/* Model step function */
void arduinoTest_step(void)
{
  uint8_T SwappedDataBytes[8];
  uint8_T b_SwappedDataBytes[9];
  real_T b_x;
  uint8_T xtmp;
  int32_T i;

  /* MATLABSystem: '<Root>/I2C Write' incorporates:
   *  Constant: '<Root>/Constant'
   */
  memcpy((void *)&SwappedDataBytes[0], (void *)&arduinoTest_P.Constant_Value,
         (uint32_T)((size_t)8 * sizeof(uint8_T)));
  xtmp = SwappedDataBytes[0];
  SwappedDataBytes[0] = SwappedDataBytes[7];
  SwappedDataBytes[7] = xtmp;
  xtmp = SwappedDataBytes[1];
  SwappedDataBytes[1] = SwappedDataBytes[6];
  SwappedDataBytes[6] = xtmp;
  xtmp = SwappedDataBytes[2];
  SwappedDataBytes[2] = SwappedDataBytes[5];
  SwappedDataBytes[5] = xtmp;
  xtmp = SwappedDataBytes[3];
  SwappedDataBytes[3] = SwappedDataBytes[4];
  SwappedDataBytes[4] = xtmp;
  memcpy((void *)&b_x, (void *)&SwappedDataBytes[0], (uint32_T)((size_t)1 *
          sizeof(real_T)));
  memcpy((void *)&SwappedDataBytes[0], (void *)&b_x, (uint32_T)((size_t)8 *
          sizeof(uint8_T)));
  b_SwappedDataBytes[0] = 0U;
  for (i = 0; i < 8; i++) {
    b_SwappedDataBytes[i + 1] = SwappedDataBytes[i];
  }

  MW_I2C_MasterWrite(arduinoTest_DW.obj.MW_I2C_HANDLE, 10U, b_SwappedDataBytes,
                     9U, false, false);

  /* End of MATLABSystem: '<Root>/I2C Write' */

  /* Matfile logging */
  rt_UpdateTXYLogVars(arduinoTest_M->rtwLogInfo,
                      (&arduinoTest_M->Timing.taskTime0));

  /* signal main to stop simulation */
  {                                    /* Sample time: [0.2s, 0.0s] */
    if ((rtmGetTFinal(arduinoTest_M)!=-1) &&
        !((rtmGetTFinal(arduinoTest_M)-arduinoTest_M->Timing.taskTime0) >
          arduinoTest_M->Timing.taskTime0 * (DBL_EPSILON))) {
      rtmSetErrorStatus(arduinoTest_M, "Simulation finished");
    }
  }

  /* Update absolute time for base rate */
  /* The "clockTick0" counts the number of times the code of this task has
   * been executed. The absolute time is the multiplication of "clockTick0"
   * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
   * overflow during the application lifespan selected.
   * Timer of this task consists of two 32 bit unsigned integers.
   * The two integers represent the low bits Timing.clockTick0 and the high bits
   * Timing.clockTickH0. When the low bit overflows to 0, the high bits increment.
   */
  if (!(++arduinoTest_M->Timing.clockTick0)) {
    ++arduinoTest_M->Timing.clockTickH0;
  }

  arduinoTest_M->Timing.taskTime0 = arduinoTest_M->Timing.clockTick0 *
    arduinoTest_M->Timing.stepSize0 + arduinoTest_M->Timing.clockTickH0 *
    arduinoTest_M->Timing.stepSize0 * 4294967296.0;
}

/* Model initialize function */
void arduinoTest_initialize(void)
{
  /* Registration code */

  /* initialize non-finites */
  rt_InitInfAndNaN(sizeof(real_T));

  /* initialize real-time model */
  (void) memset((void *)arduinoTest_M, 0,
                sizeof(RT_MODEL_arduinoTest_T));
  rtmSetTFinal(arduinoTest_M, 10.0);
  arduinoTest_M->Timing.stepSize0 = 0.2;

  /* Setup for data logging */
  {
    static RTWLogInfo rt_DataLoggingInfo;
    rt_DataLoggingInfo.loggingInterval = NULL;
    arduinoTest_M->rtwLogInfo = &rt_DataLoggingInfo;
  }

  /* Setup for data logging */
  {
    rtliSetLogXSignalInfo(arduinoTest_M->rtwLogInfo, (NULL));
    rtliSetLogXSignalPtrs(arduinoTest_M->rtwLogInfo, (NULL));
    rtliSetLogT(arduinoTest_M->rtwLogInfo, "tout");
    rtliSetLogX(arduinoTest_M->rtwLogInfo, "");
    rtliSetLogXFinal(arduinoTest_M->rtwLogInfo, "");
    rtliSetLogVarNameModifier(arduinoTest_M->rtwLogInfo, "rt_");
    rtliSetLogFormat(arduinoTest_M->rtwLogInfo, 4);
    rtliSetLogMaxRows(arduinoTest_M->rtwLogInfo, 0);
    rtliSetLogDecimation(arduinoTest_M->rtwLogInfo, 1);
    rtliSetLogY(arduinoTest_M->rtwLogInfo, "");
    rtliSetLogYSignalInfo(arduinoTest_M->rtwLogInfo, (NULL));
    rtliSetLogYSignalPtrs(arduinoTest_M->rtwLogInfo, (NULL));
  }

  /* states (dwork) */
  (void) memset((void *)&arduinoTest_DW, 0,
                sizeof(DW_arduinoTest_T));

  /* Matfile logging */
  rt_StartDataLoggingWithStartTime(arduinoTest_M->rtwLogInfo, 0.0, rtmGetTFinal
    (arduinoTest_M), arduinoTest_M->Timing.stepSize0, (&rtmGetErrorStatus
    (arduinoTest_M)));

  {
    codertarget_arduinobase_int_i_T *obj;
    uint32_T i2cname;

    /* Start for MATLABSystem: '<Root>/I2C Write' */
    arduinoTest_DW.obj.matlabCodegenIsDeleted = true;
    arduinoI2CWrite_arduinoI2CWrite(&arduinoTest_DW.obj);
    arduinoTest_DW.objisempty = true;
    obj = &arduinoTest_DW.obj;
    arduinoTest_DW.obj.isSetupComplete = false;
    arduinoTest_DW.obj.isInitialized = 1;
    i2cname = 0;
    obj->MW_I2C_HANDLE = MW_I2C_Open(i2cname, 0);
    arduinoTest_DW.obj.BusSpeed = 100000U;
    MW_I2C_SetBusSpeed(arduinoTest_DW.obj.MW_I2C_HANDLE,
                       arduinoTest_DW.obj.BusSpeed);
    arduinoTest_DW.obj.isSetupComplete = true;
  }
}

/* Model terminate function */
void arduinoTest_terminate(void)
{
  /* Terminate for MATLABSystem: '<Root>/I2C Write' */
  matlabCodegenHandle_matlabCodeg(&arduinoTest_DW.obj);
}
