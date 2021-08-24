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
   /* this is for the angles command */
   const TickType_t xDelay7ms = pdMS_TO_TICKS(10);
   /* this is the minimum possible time between commands - the typical time is 50 ms, I might want to change this */
   const TickType_t xDelay20ms = pdMS_TO_TICKS(20);
#endif

const int unFiltVoltsComm[4] = {0x60, 0x01, 0x01, 0x02};
const int filtVoltsComm[4] = {0x60, 0x03, 0x01, 0x04};
const int anglesComm[4] = {0x60, 0x04, 0x01, 0x05};

/* number of bytes returned for angle command */
const int angleRespLength = 17;

/* make a receiver buffer, needs room for angleRespLength bytes*/
/* c gets salty at me if I try to make it of length angleRespLength but */
/* so I'm just putting it as 17 */
extern uint8_t recv_buffer[17];

/*!
 * @brief Structure contains information about sun sensor
 *
 */
typedef struct _Sun
{
  double angles[3];           /* measured alpha and beta angles and sun detection */
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
