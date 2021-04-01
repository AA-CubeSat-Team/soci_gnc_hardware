#include "SunSensorWrapper.h"
#include "peripherals.h"
#include "fsL_lpuart.h"
#include "fsl_lpuart_freertos.h"
#include "task.h"
#include <math.h>

/* used for UART error identification */
static int error;

/* used to make sure that the UART instance is actually initialized */
static int init = 0;

/* command bytes to be issued to sun sensor */
const uint8_t unFiltVoltsComm[4] = {0x60, 0x01, 0x01, 0x02};
const uint8_t filtVoltsComm[4] = {0x60, 0x03, 0x01, 0x04};
const uint8_t anglesComm[4] = {0x60, 0x04, 0x01, 0x05};

/* delay times in ticks needed */
const TickType_t xDelay3ms = pdMS_TO_TICKS(3);
const TickType_t xDelay7ms = pdMS_TO_TICKS(7);
/* this is the minimum possible time between commands - the typical time is 50 ms, I might want to change this */
const TickType_t xDelay20ms = pdMS_TO_TICKS(20);

/* lengths of voltage responses and angle response */
const int voltRespLength = 20;
const int angleRespLength = 17;

/* make a receiver buffer, needs room for at most voltLength bytes*/
uint8_t recv_buffer[voltRespLength];

/* initializes sun sensor UART */
/* note this has nothing to do with actually turning the sun sensor on */
void sunSenUARTInit(){
   /* initialize LPUART instance */
   LPUART3_init();
   init = 1;
   return;
}

/* converts the response bytes into floats, but saves them as doubles */
/* since the data is stored as a double */
/* takes a pointer to the data that will be written over (e.g. angles) */
/* and the number of floats to read */
void readFloats(double* data, int floatsToRead){
   /* read in floatsToRead num of "floats" */
   for (int x = 0; x < floatsToRead; x++){
      /* initialize some variables that will be used to calculate the value of a "float" later */
      double sign = 1.0;
      double exp = -127.0;
      double frac = 1.0;
      /* next, I need to take 4 data bytes from recv_buffer that correspond to the data */
      uint8_t bytes[4] = {recv_buffer[4*x+6], recv_buffer[4*x+5], recv_buffer[4*x+4], recv_buffer[4*x+3]};
      /* The sun sensor sends from LSB to MSB so I need to reverse the order of the bytes */
      /* the most significant bit corresponds to the sign */
      /* the sign defaults to 1 if the most siginificant bit is 0, but if it is 1, then the sign is -1 */
      if (bytes[0] > 127){
	 sign = -1.0;
	 /* need to subtract off the most significant bit so I can start working on the exponent */
	 bytes[0] -= 128;
      }
      /* the next 8 bits comprise the exponent bit. these are the remaining bits of byte3 and the first of byte2 */
      exp += 2*(double)bytes[0];
      /* if the first bit of the next byte is 1, then 2^0 needs to be added to exp */
      if (bytes[1] > 127){
	 exp += 1.0;
	 bytes[1] -= 128;
      }
      /* the remaining bits make up the fraction. */
      frac += (double)bytes[1]*pow(2.0, -7.0)+(double)bytes[2]*pow(2.0, -15.0)+(double)bytes[3]*pow(2.0, -23.0);
      /* putting sign, exp, and frac together and assigning it to the data array */
      *(data + x) = sign*pow(2.0, exp)*frac;
      }
   /* next, take the total sum of all the bytes except the address byte */
   int totSum = 0;
   /* need to sum from command (2nd) byte to last data byte (2nd to last of array) */
   if(floatsToRead == 4){
      for(int x = 1; x < voltRespLength - 1; x++){
         totSum += (int)recv_buffer[x];
      }
   }else{
      for(int x = 1; x < angleRespLength - 1; x++){
         totSum += (int)recv_buffer[x];
      }
   }
   /* next, need to pare totSum down to its least significant byte, which can be accomplished by */
   /* comparing it to 255 */
   totSum = (totSum & 255);
   /* then compare it to the checkSum byte, which is the last byte */
   if(totSum != (int)recv_buffer[angleRespLength - 1] && floatsToRead == 3){
      *data = -1000.0;
      *(data + 1) = -1000.0;
      *(data + 2) = -1000.0;
      return;
   }else if(totSum != (int)recv_buffer[voltRespLength - 1] && floatsToRead == 4){
      *data = -1000.0;
      *(data + 1) = -1000.0;
      *(data + 2) = -1000.0;
      *(data + 3) = -1000.0;
      return;
   }
   /* lastly, if we're getting values for angles (floatsToRead is 3) then the 4th read value needs to be the error byte */
   if (floatsToRead == 3){
      *(data + 3) = (double)recv_buffer[11];
   }
   return;
}

/* getUnfiltVolts() will issue the unfiltered cell voltage commmand to the sun sensor, read in the response, perform some checks */
/* and then call readFloats to interpret data */
/* it needs the pointer to the unfiltered cell voltages */
void getUnfiltVolts(double* unFiltVolts){
   /* make sure UART is initialized */
   if (init == 0){
      *unFiltVolts = -2000.0;
      *(unFiltVolts + 1) = -2000.0;
      *(unFiltVolts + 2) = -2000.0;
      *(unFiltVolts + 3) = -2000.0;
      return;
   }
   /* issue command to sun sensor */
   error = LPUART_RTOS_Send(&uart3_handle, &unfiltVoltsComm, sizeof(unfiltVoltsComm));
   if(error != kStatus_Success){
      /* might want to include more comprehensive errors */
      *unFiltVolts = -3000.0;
      *(unFiltVolts + 1) = -3000.0;
      *(unFiltVolts + 2) = -3000.0;
      *(unFiltVolts + 3) = -3000.0;
      return;
   }
   /* wait until response can be sent */
   vDelay(xDelay3ms);
   /* this variable can be used to see how many bytes were received */
   size_t numRecvBytes = 0;
   /* read in response to receiver buffer - takes the entire length of the receive buffer */
   error = LPUART_RTOS_Receive(&uart3_handle, &recv_buffer, sizeof(recv_buffer), &numRecvBytes);
   if(error != kStatus_Success){
      /* might want to include more comprehensive errors */
      *unFiltVolts = -4000.0;
      *(unFiltVolts + 1) = -4000.0;
      *(unFiltVolts + 2) = -4000.0;
      *(unFiltVolts + 3) = -4000.0;
      return;
   }
   /* check that the command byte (the 2nd byte) sent corresponds to the command byte received */
   if(unFiltVoltsComm[1] == recv_buffer[1]){
      /* if it's correct, call readFloats and read in 4 floats */
      readFloats(unFiltVolts, 4);
   }else{
      /* if it's incorrect, assign an error value */
      *unFiltVolts = -5000.0;
      *(unFiltVolts + 1) = -5000.0;
      *(unFiltVolts + 2) = -5000.0;
      *(unFiltVolts + 3) = -5000.0;
      return;
   }
   return;
}

/* similar to getUnfiltVolts() */
void getFiltVolts(double* filtVolts){
   if (init == 0){
      *filtVolts = -2000.0;
      *(filtVolts + 1) = -2000.0;
      *(filtVolts + 2) = -2000.0;
      *(filtVolts + 3) = -2000.0;
      return;
   }
   error = LPUART_RTOS_Send(&uart3_handle, &filtVoltsComm, sizeof(filtVoltsComm));
   if(error != kStatus_Success){
      /* might want to include more comprehensive errors */
      *filtVolts = -3000.0;
      *(filtVolts + 1) = -3000.0;
      *(filtVolts + 2) = -3000.0;
      *(filtVolts + 3) = -3000.0;
      return;
   }
   vDelay(xDelay3ms);
   size_t numRecvBytes = 0;
   /* response is shorter so use angleRespLength instead */
   error = LPUART_RTOS_Receive(&uart3_handle, &recv_buffer, angleRespLength, &numRecvBytes);
      if(error != kStatus_Success){
      /* might want to include more comprehensive errors */
      *filtVolts = -4000.0;
      *(filtVolts + 1) = -4000.0;
      *(filtVolts + 2) = -4000.0;
      *(filtVolts + 3) = -4000.0;
      return;
   }
   if(filtVoltsComm[1] == recv_buffer[1]){
      readFloats(filtVolts, 4);
   }else{
      *filtVolts = -5000.0;
      *(filtVolts + 1) = -5000.0;
      *(filtVolts + 2) = -5000.0;
      *(filtVolts + 3) = -5000.0;
      return;
   }
   return;
}

/* similar to getUnfiltVolts() */
void getAngles(double* angles){
   if (init == 0){
      *angles = -2000.0;
      *(angles + 1) = -2000.0;
      *(angles + 2) = -2000.0;
      return;
   }
   error = LPUART_RTOS_Send(&uart3_handle, &recv_buffer, sizeof(anglesComm));
   if(error != kStatus_Success){
      /* might want to include more comprehensive errors */
      *angles = -3000.0;
      *(angles + 1) = -3000.0;
      *(angles + 2) = -3000.0;
      return;
   }
   vDelay(xDelay7ms);
   size_t numRecvBytes = 0;
   error = LPUART_RTOS_Receive(&uart3_handle, &recv_buffer, 13, &numRecvBytes);
   if(error != kStatus_Success){
      /* might want to include more comprehensive errors */
      *angles = -4000.0;
      *(angles + 1) = -4000.0;
      *(angles + 2) = -4000.0;
      return;
   }
   if(anglesComm[1] == recv_buffer[1]){
      readFloats(angles, 3);
   }else{
      *angles = -5000.0;
      *(angles + 1) = -5000.0;
      *(angles + 2) = -5000.0;
      return;
   }
   return;
}

/* this is the main function that higher level flight code should call */
/* it will get the angles */
/* it should only take a pointer to the first element of angles, doesn't work */
/* if it's the whole array for some reason */
void getSunSensorData(double* angles){
   getAngles(angles);
   /* error handling will be implemented here - if one of the UART functions */
   /* doesn't work then it will reset the UART instance and try again */
   return;
}

/* this will check the health of sun sensor and also get the volts */
void checkSunSensorHealth(double* volts){
   getFilteredVolts(volts);
   /* health check to be implemented */
}
