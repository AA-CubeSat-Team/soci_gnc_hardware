/*
 * arduino_I2C_temp.h
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

#ifndef RTW_HEADER_arduino_I2C_temp_h_
#define RTW_HEADER_arduino_I2C_temp_h_
#include <string.h>
#include <stddef.h>
#ifndef arduino_I2C_temp_COMMON_INCLUDES_
# define arduino_I2C_temp_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#include "MW_I2C.h"
#include "MW_arduino_digitalio.h"
#endif                                 /* arduino_I2C_temp_COMMON_INCLUDES_ */

#include "arduino_I2C_temp_types.h"

/* Shared type includes */
#include "multiword_types.h"
#include "MW_target_hardware_resources.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

/* Block states (default storage) for system '<Root>' */
typedef struct {
  codertarget_arduinobase_int_o_T obj; /* '<Root>/I2C Read' */
  codertarget_arduinobase_block_T obj_p;/* '<S1>/Digital Output' */
  boolean_T objisempty;                /* '<S1>/Digital Output' */
  boolean_T objisempty_n;              /* '<Root>/I2C Read' */
} DW_arduino_I2C_temp_T;

/* Parameters (default storage) */
struct P_arduino_I2C_temp_T_ {
  real_T I2CRead_SampleTime;           /* Expression: 0.1
                                        * Referenced by: '<Root>/I2C Read'
                                        */
  real32_T Switch_Threshold;           /* Computed Parameter: Switch_Threshold
                                        * Referenced by: '<Root>/Switch'
                                        */
  int16_T Gain_Gain;                   /* Computed Parameter: Gain_Gain
                                        * Referenced by: '<Root>/Gain'
                                        */
  uint8_T LOW_Value;                   /* Computed Parameter: LOW_Value
                                        * Referenced by: '<Root>/LOW'
                                        */
  uint8_T HIGH_Value;                  /* Computed Parameter: HIGH_Value
                                        * Referenced by: '<Root>/HIGH'
                                        */
};

/* Real-time Model Data Structure */
struct tag_RTM_arduino_I2C_temp_T {
  const char_T *errorStatus;
};

/* Block parameters (default storage) */
extern P_arduino_I2C_temp_T arduino_I2C_temp_P;

/* Block states (default storage) */
extern DW_arduino_I2C_temp_T arduino_I2C_temp_DW;

/* Model entry point functions */
extern void arduino_I2C_temp_initialize(void);
extern void arduino_I2C_temp_step(void);
extern void arduino_I2C_temp_terminate(void);

/* Real-time Model object */
extern RT_MODEL_arduino_I2C_temp_T *const arduino_I2C_temp_M;

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Use the MATLAB hilite_system command to trace the generated code back
 * to the model.  For example,
 *
 * hilite_system('<S3>')    - opens system 3
 * hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'arduino_I2C_temp'
 * '<S1>'   : 'arduino_I2C_temp/Digital Output'
 */
#endif                                 /* RTW_HEADER_arduino_I2C_temp_h_ */
