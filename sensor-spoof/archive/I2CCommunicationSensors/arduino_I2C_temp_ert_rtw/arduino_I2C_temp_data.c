/*
 * arduino_I2C_temp_data.c
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

/* Block parameters (default storage) */
P_arduino_I2C_temp_T arduino_I2C_temp_P = {
  /* Expression: 0.1
   * Referenced by: '<Root>/I2C Read'
   */
  0.1,

  /* Computed Parameter: Switch_Threshold
   * Referenced by: '<Root>/Switch'
   */
  27.0F,

  /* Computed Parameter: Gain_Gain
   * Referenced by: '<Root>/Gain'
   */
  16384,

  /* Computed Parameter: LOW_Value
   * Referenced by: '<Root>/LOW'
   */
  0U,

  /* Computed Parameter: HIGH_Value
   * Referenced by: '<Root>/HIGH'
   */
  1U
};
