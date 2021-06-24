#define ARDUINO_CODE 0
/* #include <math.h> */
#if ARDUINO_CODE
   #include <Arduino.h>
   #include <hardwareSerial.h>
#else
   #include "peripherals.h"
   #include "fsL_lpuart.h"
   #include "fsl_lpuart_freertos.h"
   #include "task.h"
   /* delay times in ticks needed */
   const TickType_t xDelay3ms = pdMS_TO_TICKS(3);
   const TickType_t xDelay7ms = pdMS_TO_TICKS(7);
   /* this is the minimum possible time between commands - the typical time is 50 ms, I might want to change this */
   const TickType_t xDelay20ms = pdMS_TO_TICKS(20);
#endif

const int unFiltVoltsComm[4] = {0x60, 0x01, 0x01, 0x02};
const int filtVoltsComm[4] = {0x60, 0x03, 0x01, 0x04};
const int anglesComm[4] = {0x60, 0x04, 0x01, 0x05};

/* lengths of voltage responses and angle response */
const int voltRespLength = 20;
const int angleRespLength = 17;

/* make a receiver buffer, needs room for at most voltRespLength bytes*/
/* c gets salty at me if I try to make it of length voltRespLength but */
/* it gets even saltier if I try to replace voltRespLength as a #define */
/* so I'm just putting it as 20 */
extern uint8_t recv_buffer[20];

/*!
 * @brief Structure contains information about one gyroscope
 *
 */
typedef struct _Sun
{
  double angles[3];           /* measured alpha and beta angles and sun detection */
  double unFiltVolts[4];      /* measured unfiltered voltages*/
  double filtVolts[4];        /* measured filtered voltages*/
  uint8_t error;              /* used to identify errors - see list below */
  uint8_t isValid;            /* used to identify if data is usable - 0 is unusable, 1 is usable */
} sun_t;

/* error list */
/* 0 - nominal */
/* 10 - not enough radiation detected */
/* 11 - albedo */
/* 12 - albedo + sun */
/* 13 - detected light source but out of FoV */
/* 14 - incorrect checksum */
/* 15 - incorrect command byte response */
/* 16 - UART communications failure */

extern sun_t Sun1;                /* sun sensor 1*/

void readFloats(double* data, unint8_t* error, uint8_t* isValid, int floatsToRead);
void getSunAngles(sun_t * Sun);
void getFiltVolts(sun_t * Sun);
void getUnfiltVolts(sun_t * Sun);
