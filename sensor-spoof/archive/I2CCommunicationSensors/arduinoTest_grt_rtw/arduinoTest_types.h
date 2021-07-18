/*
 * arduinoTest_types.h
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

#ifndef RTW_HEADER_arduinoTest_types_h_
#define RTW_HEADER_arduinoTest_types_h_
#include "rtwtypes.h"
#include "builtin_typeid_types.h"
#include "multiword_types.h"

/* Custom Type definition for MATLABSystem: '<Root>/I2C Write' */
#include "MW_SVD.h"
#include <stddef.h>
#ifndef typedef_cell_arduinoTest_T
#define typedef_cell_arduinoTest_T

typedef struct {
  char_T f1;
  char_T f2;
  char_T f3;
  char_T f4;
  char_T f5;
  char_T f6;
  char_T f7;
  char_T f8;
  char_T f9[2];
  char_T f10[2];
  char_T f11[2];
  char_T f12[2];
} cell_arduinoTest_T;

#endif                                 /*typedef_cell_arduinoTest_T*/

#ifndef typedef_codertarget_arduinobase_inter_T
#define typedef_codertarget_arduinobase_inter_T

typedef struct {
  cell_arduinoTest_T AvailablePwmPinNames;
} codertarget_arduinobase_inter_T;

#endif                                 /*typedef_codertarget_arduinobase_inter_T*/

#ifndef typedef_codertarget_arduinobase_int_i_T
#define typedef_codertarget_arduinobase_int_i_T

typedef struct {
  boolean_T matlabCodegenIsDeleted;
  int32_T isInitialized;
  boolean_T isSetupComplete;
  codertarget_arduinobase_inter_T Hw;
  uint32_T BusSpeed;
  MW_Handle_Type MW_I2C_HANDLE;
} codertarget_arduinobase_int_i_T;

#endif                                 /*typedef_codertarget_arduinobase_int_i_T*/

/* Parameters (default storage) */
typedef struct P_arduinoTest_T_ P_arduinoTest_T;

/* Forward declaration for rtModel */
typedef struct tag_RTM_arduinoTest_T RT_MODEL_arduinoTest_T;

#endif                                 /* RTW_HEADER_arduinoTest_types_h_ */
