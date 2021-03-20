#include "arduino_I2C_temp.h"
#include "rtwtypes.h"

volatile int IsrOverrun = 0;
static boolean_T OverrunFlag = 0;
void rt_OneStep(void)
{
  /* Check for overrun. Protect OverrunFlag against preemption */
  if (OverrunFlag++) {
    IsrOverrun = 1;
    OverrunFlag--;
    return;
  }

#ifndef _MW_ARDUINO_LOOP_

  sei();

#endif;

  arduino_I2C_temp_step();

  /* Get model outputs here */
#ifndef _MW_ARDUINO_LOOP_

  cli();

#endif;

  OverrunFlag--;
}

volatile boolean_T stopRequested = false;
int main(void)
{
  volatile boolean_T runModel = true;
  float modelBaseRate = 0.1;
  float systemClock = 0;
  init();
  MW_Arduino_Init();
  rtmSetErrorStatus(arduino_I2C_temp_M, 0);
  arduino_I2C_temp_initialize();
  configureArduinoAVRTimer();
  runModel =
    rtmGetErrorStatus(arduino_I2C_temp_M) == (NULL);

#ifndef _MW_ARDUINO_LOOP_

  sei();

#endif;

  sei ();
  while (runModel) {
    stopRequested = !(
                      rtmGetErrorStatus(arduino_I2C_temp_M) == (NULL));
    runModel = !(stopRequested);
    runModel = runModel && MW_Arduino_Loop();
  }

  /* Disable rt_OneStep() here */

  /* Terminate model */
  arduino_I2C_temp_terminate();
  cli();
  return 0;
}
