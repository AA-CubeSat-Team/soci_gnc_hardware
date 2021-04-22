//#include "SunSensorWrapper.h"
//#include "peripherals.h"
//#include "fsL_lpuart.h"
//#include "fsl_lpuart_freertos.h"
//#include "task.h"
#include <math.h>

/* used for UART error identification */
static int error;

/* used to check if a function has already been called again in the event of an error */
/* only have time to attempt getting data twice, so if it fails on the second try */
/* will assign error value instead of trying again */
static int recall = 0;
/* 0 = no recall, 1 = already recalled */

/* make a receiver buffer, needs room for at most voltLength bytes*/
uint8_t recv_buffer[voltRespLength];

/* initializes sun sensor UART */
/* note this has nothing to do with actually turning the sun sensor on */
void sunSenUARTInit(){
   /* initialize LPUART instance */
   LPUART3_init();
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
   if((totSum != (int)recv_buffer[angleRespLength - 1] && floatsToRead == 3) || (totSum != (int)recv_buffer[voltRespLength - 1] && floatsToRead == 4)){
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
   /* issue command to sun sensor */
   error = LPUART_RTOS_Send(&uart3_handle, &unfiltVoltsComm, sizeof(unfiltVoltsComm));
   if(error != kStatus_Success){
      if(recall == 0){
         vTaskDelay(xDelay20ms);
         getUnfiltVolts(unFiltVolts);
         recall = 1;
      }
      else{
         *unFiltVolts = -2000.0;
         *(unFiltVolts + 1) = -2000.0;
         *(unFiltVolts + 2) = -2000.0;
         *(unFiltVolts + 3) = -2000.0;
         recall = 0;
      }
      return;
   }
   /* wait until response can be sent */
   vTaskDelay(xDelay3ms);
   /* this variable can be used to see how many bytes were received */
   size_t numRecvBytes = 0;
   /* read in response to receiver buffer - takes the entire length of the receive buffer */
   error = LPUART_RTOS_Receive(&uart3_handle, &recv_buffer, sizeof(recv_buffer), &numRecvBytes);
   if(error != kStatus_Success){
       if(recall == 0){
         vTaskDelay(xDelay20ms);
         getUnfiltVolts(unFiltVolts);
         recall = 1;
      }
      else{
         *unFiltVolts = -2000.0;
         *(unFiltVolts + 1) = -2000.0;
         *(unFiltVolts + 2) = -2000.0;
         *(unFiltVolts + 3) = -2000.0;
         recall = 0;
      }
      return;
   }
   /* check that the command byte (the 2nd byte) sent corresponds to the command byte received */
   if(unFiltVoltsComm[1] == recv_buffer[1]){
      /* if it's correct, call readFloats and read in 4 floats */
      readFloats(unFiltVolts, 4);
   }else{
      if(recall == 0){
         vTaskDelay(xDelay20ms);
         getUnfiltVolts(unFiltVolts);
         recall = 1;
      }
      else{
         *unFiltVolts = -1000.0;
         *(unFiltVolts + 1) = -1000.0;
         *(unFiltVolts + 2) = -1000.0;
         *(unFiltVolts + 3) = -1000.0;
         recall = 0;
      }
      return;
   }
   return;
}

/* similar to getUnfiltVolts() */
void getFiltVolts(double* filtVolts){
   error = LPUART_RTOS_Send(&uart3_handle, &filtVoltsComm, sizeof(filtVoltsComm));
   if(error != kStatus_Success){
      if(recall == 0){
         vTaskDelay(xDelay20ms);
         getFiltVolts(filtVolts);
         recall = 1;
      }
      else{
         *filtVolts = -2000.0;
         *(filtVolts + 1) = -2000.0;
         *(filtVolts + 2) = -2000.0;
         *(filtVolts + 3) = -2000.0;
         recall = 0;
      }
      return;
   }
   vTaskDelay(xDelay3ms);
   size_t numRecvBytes = 0;
   /* response is shorter so use angleRespLength instead */
   error = LPUART_RTOS_Receive(&uart3_handle, &recv_buffer, angleRespLength, &numRecvBytes);
   if(error != kStatus_Success){
      if(recall == 0){
         vTaskDelay(xDelay20ms);
         getFiltVolts(filtVolts);
         recall = 1;
      }
      else{
         *filtVolts = -2000.0;
         *(filtVolts + 1) = -2000.0;
         *(filtVolts + 2) = -2000.0;
         *(filtVolts + 3) = -2000.0;
         recall = 0;
      }
      return;
   }
   if(filtVoltsComm[1] == recv_buffer[1]){
      readFloats(filtVolts, 4);
   }else{
      if(recall == 0){
         vTaskDelay(xDelay20ms);
         getFiltVolts(filtVolts);
         recall = 1;
      }
      else{
         *filtVolts = -1000.0;
         *(filtVolts + 1) = -1000.0;
         *(filtVolts + 2) = -1000.0;
         *(filtVolts + 3) = -1000.0;
         recall = 0;
      }
      return;
   }
   return;
}

/* similar to getUnfiltVolts() */
/* this is the main function that higher level flight should call */
/* it takes a pointer to the first angle element  */
void getSunAngles(double* angles){
   error = LPUART_RTOS_Send(&uart3_handle, &recv_buffer, sizeof(anglesComm));
   if(error != kStatus_Success){
      if(recall == 0){
         vTaskDelay(xDelay20ms);
         getSunAngles(angles);
         recall = 1;
      }
      else{
         *angles = -2000.0;
         *(angles + 1) = -2000.0;
         *(angles + 2) = -2000.0;
         *(angles + 3) = -2000.0;
         recall = 0;
      }
      return;
   }
   vTaskDelay(xDelay7ms);
   size_t numRecvBytes = 0;
   error = LPUART_RTOS_Receive(&uart3_handle, &recv_buffer, 13, &numRecvBytes);
   if(error != kStatus_Success){
      if(recall == 0){
         vTaskDelay(xDelay20ms);
         getSunAngles(angles);
         recall = 1;
      }
      else{
         *angles = -2000.0;
         *(angles + 1) = -2000.0;
         *(angles + 2) = -2000.0;
         *(angles + 3) = -2000.0;
         recall = 0;
      }
      return;
   }
   if(anglesComm[1] == recv_buffer[1]){
      readFloats(angles, 3);
   }else{
      if(recall == 0){
         vTaskDelay(xDelay20ms);
         getSunAngles(angles);
         recall = 1;
      }
      else{
         *angles = -1000.0;
         *(angles + 1) = -1000.0;
         *(angles + 2) = -1000.0;
         *(angles + 3) = -1000.0;
         recall = 0;
      }
      return;   
   }
   return;
}

/* this will check the health of sun sensor */
/* it takes a pointer to the health - currently assuming that is an integer */
void checkSunSensorHealth(int* health){
   // static double volts = {0, 0, 0, 0};
//    getFilteredVolts(&volts[0]);
//    /* want to see if any of the voltage values are particularly small or large relative to the others */
//    /* check that there was no error first */
//    if(volts != {-1000.0, -1000.0, -1000.0, -1000.0} && volts != {-2000.0, -2000.0, -2000.0, -2000.0}){
//       /* calculate all possible differences (order not mattering) */
//       static double diffs = {0, 0, 0, 0, 0, 0};
//       static int j = 0;
//       for (int x = 0; x < 4; x++){
//          for(int y = x + 1; y < 4; y++){
//             if(volts(x) >= volts(y)){
//                diffs(j) = (volts(x)-volts(y))/volts(y);
//             }
//             else{
//                diffs(j) = (volts(y)-volts(x))/volts(x);
//             }
//             j++;
//          } 
//       }
//   }else{
//     *health = /* something */
//     return;
//   }
//   /* from here - check if any one of the values in diffs() is large */
//   /* we want to see how many cells agree the difference with another cell is large */
//   static int diffVotes = {0, 0, 0, 0};
//   /* check all differences involving cell 1 (i.e. diffs(0) to diffs(2)) */
//   for (int x = 0; x < 3; x++){
//     if(diffs(x) > /* lowest limit for notably large */){
//        /* cell (x+1) and cell 1 are notably different, so increase their votes by one */
//        diffVotes(0)++;
//        diffVotes(x+1)++;
//     }
//   }
//   /* do the same for cell 2, but cell 1 and cell 2 diff is already counted */
//   for (int x = 3; x < 5; x++){
//     if(diffs(x) > /* lowest limit for notably large */){
//        /* cell 2 and cell x - 1 are notably different, so increase their votes by one */
//        diffVotes(1)++;
//        diffVotes(x-1)++;
//     }
//   }
//   /* lastly, check if cell 3 and 4 are notably different */
//   if(diffs(5) >  /* lowest limit for notably large */){
//     diffVotes(2)++;
//     diffVotes(3)++;
//   }
//   /* then I need to find the largest number of votes */
//   /* on a theoretical scale: 0 = nominal, 1 = slight concern, 2 = very concerned, 3 = extremely concerned */
}
