/*
 * arduino_I2C_temp_types.h
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

#ifndef RTW_HEADER_arduino_I2C_temp_types_h_
#define RTW_HEADER_arduino_I2C_temp_types_h_
#include "rtwtypes.h"
#include "multiword_types.h"

/* Custom Type definition for MATLABSystem: '<Root>/I2C Read' */
#include "MW_SVD.h"
#include <stddef.h>
#ifndef typedef_codertarget_arduinobase_block_T
#define typedef_codertarget_arduinobase_block_T

typedef struct {
  boolean_T matlabCodegenIsDeleted;
  int32_T isInitialized;
  boolean_T isSetupComplete;
} codertarget_arduinobase_block_T;

#endif                                 /*typedef_codertarget_arduinobase_block_T*/

#ifndef typedef_cell_arduino_I2C_temp_T
#define typedef_cell_arduino_I2C_temp_T

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
} cell_arduino_I2C_temp_T;

#endif                                 /*typedef_cell_arduino_I2C_temp_T*/

#ifndef typedef_codertarget_arduinobase_inter_T
#define typedef_codertarget_arduinobase_inter_T

typedef struct {
  cell_arduino_I2C_temp_T AvailablePwmPinNames;
} codertarget_arduinobase_inter_T;

#endif                                 /*typedef_codertarget_arduinobase_inter_T*/

#ifndef typedef_codertarget_arduinobase_int_o_T
#define typedef_codertarget_arduinobase_int_o_T

typedef struct {
  boolean_T matlabCodegenIsDeleted;
  int32_T isInitialized;
  boolean_T isSetupComplete;
  codertarget_arduinobase_inter_T Hw;
  uint32_T BusSpeed;
  MW_Handle_Type MW_I2C_HANDLE;
  real_T SampleTime;
} codertarget_arduinobase_int_o_T;

#endif                                 /*typedef_codertarget_arduinobase_int_o_T*/

/* Parameters (default storage) */
typedef struct P_arduino_I2C_temp_T_ P_arduino_I2C_temp_T;

/* Forward declaration for rtModel */
typedef struct tag_RTM_arduino_I2C_temp_T RT_MODEL_arduino_I2C_temp_T;

#endif                                 /* RTW_HEADER_arduino_I2C_temp_types_h_ */
