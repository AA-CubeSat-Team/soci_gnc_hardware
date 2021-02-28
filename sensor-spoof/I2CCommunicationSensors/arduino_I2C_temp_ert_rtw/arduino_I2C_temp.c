/*
 * arduino_I2C_temp.c
 *
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * Code generation for model "arduino_I2C_temp".
 *
 * Model version              : 1.29
 * Simulink Coder version : 9.0 (R2018b) 24-May-2018
 * C source code generated on : Sun Feb 21 18:47:24 2021
 *
 * Target selection: ert.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: Atmel->AVR
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "arduino_I2C_temp.h"
#include "arduino_I2C_temp_private.h"

/* Block states (default storage) */
DW_arduino_I2C_temp_T arduino_I2C_temp_DW;

/* Real-time model */
RT_MODEL_arduino_I2C_temp_T arduino_I2C_temp_M_;
RT_MODEL_arduino_I2C_temp_T *const arduino_I2C_temp_M = &arduino_I2C_temp_M_;

/* Forward declaration for local functions */
static codertarget_arduinobase_int_o_T *a_arduinoI2CRead_arduinoI2CRead
  (codertarget_arduinobase_int_o_T *obj);
static void arduino_I2C__SystemCore_release(const
  codertarget_arduinobase_int_o_T *obj);
static void arduino_I2C_t_SystemCore_delete(const
  codertarget_arduinobase_int_o_T *obj);
static void matlabCodegenHandle_matlabCodeg(codertarget_arduinobase_int_o_T *obj);
static void matlabCodegenHandle_matlabCod_o(codertarget_arduinobase_block_T *obj);
static codertarget_arduinobase_int_o_T *a_arduinoI2CRead_arduinoI2CRead
  (codertarget_arduinobase_int_o_T *obj)
{
  codertarget_arduinobase_int_o_T *b_obj;
  obj->isInitialized = 0L;
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

static void arduino_I2C__SystemCore_release(const
  codertarget_arduinobase_int_o_T *obj)
{
  if ((obj->isInitialized == 1L) && obj->isSetupComplete) {
    MW_I2C_Close(obj->MW_I2C_HANDLE);
  }
}

static void arduino_I2C_t_SystemCore_delete(const
  codertarget_arduinobase_int_o_T *obj)
{
  arduino_I2C__SystemCore_release(obj);
}

static void matlabCodegenHandle_matlabCodeg(codertarget_arduinobase_int_o_T *obj)
{
  if (!obj->matlabCodegenIsDeleted) {
    obj->matlabCodegenIsDeleted = true;
    arduino_I2C_t_SystemCore_delete(obj);
  }
}

static void matlabCodegenHandle_matlabCod_o(codertarget_arduinobase_block_T *obj)
{
  if (!obj->matlabCodegenIsDeleted) {
    obj->matlabCodegenIsDeleted = true;
  }
}

/* Model step function */
void arduino_I2C_temp_step(void)
{
  uint16_T output;
  uint8_T output_raw[2];
  uint8_T b_x[2];
  uint8_T tmp;

  /* MATLABSystem: '<Root>/I2C Read' */
  if (arduino_I2C_temp_DW.obj.SampleTime !=
      arduino_I2C_temp_P.I2CRead_SampleTime) {
    arduino_I2C_temp_DW.obj.SampleTime = arduino_I2C_temp_P.I2CRead_SampleTime;
  }

  MW_I2C_MasterRead(arduino_I2C_temp_DW.obj.MW_I2C_HANDLE, 72UL, output_raw, 2UL,
                    false, false);
  memcpy((void *)&output, (void *)&output_raw[0], (uint16_T)((size_t)1 * sizeof
          (uint16_T)));
  memcpy((void *)&output_raw[0], (void *)&output, (uint16_T)((size_t)2 * sizeof
          (uint8_T)));
  b_x[0] = output_raw[1];
  b_x[1] = output_raw[0];
  memcpy((void *)&output, (void *)&b_x[0], (uint16_T)((size_t)1 * sizeof
          (uint16_T)));

  /* Switch: '<Root>/Switch' incorporates:
   *  Constant: '<Root>/HIGH'
   *  Constant: '<Root>/LOW'
   *  DataTypeConversion: '<Root>/Data Type Conversion'
   *  Gain: '<Root>/Gain'
   *  MATLABSystem: '<Root>/I2C Read'
   */
  if ((real32_T)arduino_I2C_temp_P.Gain_Gain * 2.38418579E-7F * (real32_T)
      (int16_T)output > arduino_I2C_temp_P.Switch_Threshold) {
    tmp = arduino_I2C_temp_P.HIGH_Value;
  } else {
    tmp = arduino_I2C_temp_P.LOW_Value;
  }

  /* End of Switch: '<Root>/Switch' */

  /* MATLABSystem: '<S1>/Digital Output' */
  writeDigitalPin(13, tmp);
}

/* Model initialize function */
void arduino_I2C_temp_initialize(void)
{
  /* Registration code */

  /* initialize error status */
  rtmSetErrorStatus(arduino_I2C_temp_M, (NULL));

  /* states (dwork) */
  (void) memset((void *)&arduino_I2C_temp_DW, 0,
                sizeof(DW_arduino_I2C_temp_T));

  {
    codertarget_arduinobase_int_o_T *obj;
    uint32_T i2cname;

    /* Start for MATLABSystem: '<Root>/I2C Read' */
    arduino_I2C_temp_DW.obj.matlabCodegenIsDeleted = true;
    a_arduinoI2CRead_arduinoI2CRead(&arduino_I2C_temp_DW.obj);
    arduino_I2C_temp_DW.objisempty_n = true;
    arduino_I2C_temp_DW.obj.SampleTime = arduino_I2C_temp_P.I2CRead_SampleTime;
    obj = &arduino_I2C_temp_DW.obj;
    arduino_I2C_temp_DW.obj.isSetupComplete = false;
    arduino_I2C_temp_DW.obj.isInitialized = 1L;
    i2cname = 0;
    obj->MW_I2C_HANDLE = MW_I2C_Open(i2cname, 0);
    arduino_I2C_temp_DW.obj.BusSpeed = 100000UL;
    MW_I2C_SetBusSpeed(arduino_I2C_temp_DW.obj.MW_I2C_HANDLE,
                       arduino_I2C_temp_DW.obj.BusSpeed);
    arduino_I2C_temp_DW.obj.isSetupComplete = true;

    /* Start for MATLABSystem: '<S1>/Digital Output' */
    arduino_I2C_temp_DW.obj_p.matlabCodegenIsDeleted = true;
    arduino_I2C_temp_DW.obj_p.isInitialized = 0L;
    arduino_I2C_temp_DW.obj_p.matlabCodegenIsDeleted = false;
    arduino_I2C_temp_DW.objisempty = true;
    arduino_I2C_temp_DW.obj_p.isSetupComplete = false;
    arduino_I2C_temp_DW.obj_p.isInitialized = 1L;
    digitalIOSetup(13, true);
    arduino_I2C_temp_DW.obj_p.isSetupComplete = true;
  }
}

/* Model terminate function */
void arduino_I2C_temp_terminate(void)
{
  /* Terminate for MATLABSystem: '<Root>/I2C Read' */
  matlabCodegenHandle_matlabCodeg(&arduino_I2C_temp_DW.obj);

  /* Terminate for MATLABSystem: '<S1>/Digital Output' */
  matlabCodegenHandle_matlabCod_o(&arduino_I2C_temp_DW.obj_p);
}
