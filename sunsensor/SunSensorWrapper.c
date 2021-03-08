#include "SunSensorWrapper.h"
#include "peripherals.h"
#include "fsL_lpuart.h"
#include "fsl_lpuart_freertos.h"
#include "task.h"
/* keeps track of number of times getData() is called */
static int calls = 0;

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
const int angleRespLength = 13;

/* make a receiver buffer, needs room for at most voltLength bytes*/
uint8_t recv_buffer[voltLength];

/* function that calculates powers of two, used in the conversion from bytes to a float */
/* takes the integer exponent of 2 to be used and a pointer to the power of two */
void pow2(int expon, float* value){
   float tempVal = 1.0;
   /* check trivial case */
   if(expon != 0){
      /* the power of two will be calculated using loops, need a variable to keep track of the current value */
      /* if the exponent is positive */
      if(expon > 0){
         for(int x = 1; x <= expon; x++){
            tempVal *= 2.0;
         }
      }
      /*otherwise, it's negative */
      else{
         for(int x = -1; x >= expon; x--){
            tempVal /= 2;
         }
      }
   }
   *value = tempVal;
   return;
}

/* initializes sun sensor UART */
/* note this has nothing to do with actually turning the sun sensor on */
void sunSenUARTInit(){
   /* initialize LPUART instance */
   LPUART3_init();
   init = 1;
   return;
}

void readFloats(float* data, int floatsToRead){
   /* read in floatsToRead num of floats */
	for (int x = 0; x < floatsToRead; x++){
		/* initialize some variables that will be used to calculate the value of a float later */
		int sign = 1;
		int exp = -127;
		float frac = 1.0;
		/* next, I need to take 4 data bytes from recv_buffer that correspond to the data */
      uint8_t bytes[4] = {recv_buffer[4*x+6], recv_buffer[4*x+5], recv_buffer[4*x+4], recv_buffer[4*x+3]};
      /* I think the sun sensor sends from LSB to MSB so I need to reverse the order of the bytes */
      /* the most significant bit corresponds to the sign */
      /* the sign defaults to 1 if the most siginificant bit is 0, but if it is 1, then the sign is -1 */
		if (bytes[0] > 127){
			sign = -1;
			/* need to subtract off the most significant bit so I can start working on the exponent */
			bytes[0] -= 128;
		}
		/* the next 8 bits comprise the exponent bit. these are the next 7 bits of byte3 and the first of byte2 */
		exp += 2*(int)bytes[0];
      /* if the first bit of the next byte is 1, then 2^0 needs to be added to exp */
		if (bytes[1] > 127){
			exp += 1;
			bytes[1] -= 128;
		}
		/* the remaining bits make up the fraction. */
      /* these are powers of two that are used to calculate the fraction */
      /* they only need to be calculated once so are static */
      float powN7 = pow2(-7, &powN7);
      float powN15 = pow2(-15, &powN15);
      float powN23 = pow2(-23, &powN23);
		frac += (float)bytes[1]*powN7+(float)bytes[2]*powN15+(float)bytes[3]*powN23;
      /* need to take 2^(exp)
      /* putting sign, exp, and frac together and assigning it to the data array */
		*(data + x) = sign*pow2(exp)*frac;
	}
   /* next, take the total sum of all the bytes except the address byte */
   int totSum = 0;
   /* need to sum from command (2nd) byte to last data byte (2nd to last of array), so 18 bytes for */
   /* filt/unfilt volts (which is 4 floats read) or 11 bytes for angles (2 floats read) */
   if(floatsToRead == 4){
      for(int x = 1; x < 19; x++){
         totSum += (int)recv_buffer[x];
      }
   }else{
      for(int x = 1; x < 12; x++){
         totSum += (int)recv_buffer[x];
      }
   }
   /* next, need to pare totSum down to its least significant byte, which can be accomplished by */
   /* comparing it to 255 */
   totSum = (totSum & 255)
   /* then compare it to the checkSum byte */
   /* if the number of floats read is 2 then this is the 13th byte */
   /* if the number of floats read is 4 then this is the 20th byte */ 
   if(totSum != (int)recv_buffer[12] && floatsToRead == 2){
      *data = -1000.0;
      *(data + 1) = -1000.0;
      *(data + 2) = -1000.0;
      return;
   }else if(totSum != (int)recv_buffer[19] && floatsToRead == 4){
      *data = -1000.0;
      *(data + 1) = -1000.0;
      *(data + 2) = -1000.0;
      *(data + 3) = -1000.0;
      return;
   }
	/* lastly, if we're getting values for angles (floatsToRead is 2) then the 3rd read value needs to be the error byte */
	if (floatsToRead == 2){
      *(data + 2) = (float)recv_buffer[11];
	return;
}

/* getUnfiltVolts() will issue the unfiltered cell voltage commmand to the sun sensor, read in the response, perform some checks */
/* and then call readFloats to interpret data */
/* it needs the pointer to the unfiltered cell voltages */
void getUnfiltVolts(float* unFiltVolts){
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
	}
   return;
}

/* similar to getUnfiltVolts() */
void getFiltVolts(float* filtVolts){
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
   }
	if(filtVoltsComm[1] == recv_buffer[1]){
		readFloats(filtVolts, 4);
	}else{
      *filtVolts = -5000.0;
      *(filtVolts + 1) = -5000.0;
      *(filtVolts + 2) = -5000.0;
      *(filtVolts + 3) = -5000.0;
   }
   return;
}

/* similar to getUnfiltVolts() */
void getAngles(float* angles){
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
   }
   vDelay(xDelay7ms);
   size_t numRecvBytes = 0;
   error = LPUART_RTOS_Receive(&uart3_handle, &recv_buffer, 13, &numRecvBytes);
   if(error != kStatus_Success){
      /* might want to include more comprehensive errors */
      *angles = -4000.0;
      *(angles + 1) = -4000.0;
      *(angles + 2) = -4000.0;
   }
   if(anglesComm[1] == recv_buffer[1]){
      readFloats(angles, 2);
   }else{
      *angles = -5000.0;
      *(angles + 1) = -5000.0;
      *(angles + 2) = -5000.0;
   }
   return;
}

/* this is the main function that higher level flight code should call */
/* it will always get new angles and occasionally get new cell voltage */
/* it takes 3 pointers: the first is that of the the first element of the angles, the second is the same but of filtered volts, */
/* and the third is the same but of unfiltered volts */
/* if you try to pass the pointer for an entire array it gets upset for some reason */
void getData(float* angles, float* filtVolts, float* unFiltVolts){
   calls += 1;
	/* every 10 times the function is called, it will also get new values for filtered voltage and unfiltered voltage */
	/* to make sure everything gets done with enough time to spare, unfiltered voltage will be found one call before filtered voltage */
	getAngles(angles);
	if(calls % 10 == 0){
      /* need to wait between commands */
      vDelay(xDelay20ms);
		getFiltVolts(filtVolts);
	}
	else if((calls+1) % 10 == 0){
      vDelay(xDelay20ms);
		getUnfiltVolts(unFiltVolts);
	}
   /* need to make sure calls will never hit the max value for an int */
   if(calls == 10){
      calls = 0;
   }
	return;
}
