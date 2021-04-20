#include <stdio.h>
#include <wiringPi.h>
#include <wiringSerial.h>
#include <math.h>

/* command bytes to be issued to sun sensor */
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
int recv_buffer[20];

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
      int bytes[4] = {recv_buffer[4*x+6], recv_buffer[4*x+5], recv_buffer[4*x+4], recv_buffer[4*x+3]};
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
      *(data + 3) = -1000.0;
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
   /* "file descriptor" (not really sure what this means, but it's needed) */
   /* for wiringSerial that just identifies the instance I guess */
   int fd = 0;
   if((fd = serialOpen("/dev/ttyS0",115200)) < 0){
		printf("serial failed to open\n");
		*unFiltVolts = -2000.0;
      *(unFiltVolts + 1) = -2000.0;
      *(unFiltVolts + 2) = -2000.0;
      *(unFiltVolts + 3) = -2000.0;
      return;
	}
   /* issue command to sun sensor */
   for(int i = 0; i < 4; i++){
      serialPutchar(fd, unFiltVoltsComm[i]);
   }
   /* wait until response can be sent */
   delay(3);
   /* read in response to receiver buffer - takes the entire length of the receive buffer */
   for(int i = 0; i < voltRespLength; i++){
      recv_buffer[i] = serialGetchar(fd);
   }
   /* close the serial instance */
   serialClose(fd);
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
   int fd = 0;
   if((fd = serialOpen("/dev/ttyS0",115200)) < 0){
		printf("serial failed to open\n");
		*filtVolts = -2000.0;
      *(filtVolts + 1) = -2000.0;
      *(filtVolts + 2) = -2000.0;
      *(filtVolts + 3) = -2000.0;
      return;
	}
   /* issue command to sun sensor */
   for(int i = 0; i < 4; i++){
      serialPutchar(fd, filtVoltsComm[i]);
   }
   /* wait until response can be sent */
   delay(3);
   /* read in response to receiver buffer - takes the entire length of the receive buffer */
   for(int i = 0; i < voltRespLength; i++){
      recv_buffer[i] = serialGetchar(fd);
   }
   serialClose(fd);
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
   int fd = 0;
   if((fd = serialOpen("/dev/ttyS0",115200)) < 0){
		printf("serial failed to open\n");
		*angles = -2000.0;
      *(angles + 1) = -2000.0;
      *(angles + 2) = -2000.0;
      *(angles + 3) = -2000.0;
      return;
	}
   /* issue command to sun sensor */
   for(int i = 0; i < 4; i++){
      serialPutchar(fd, anglesComm[i]);
   }
   /* wait until response can be sent */
   delay(7);
   /* read in response to receiver buffer - takes the entire length of the receive buffer */
   for(int i = 0; i < angleRespLength; i++){
      recv_buffer[i] = serialGetchar(fd);
   }
   serialClose(fd);
   if(anglesComm[1] == recv_buffer[1]){
      readFloats(angles, 3);
   }else{
      *angles = -5000.0;
      *(angles + 1) = -5000.0;
      *(angles + 2) = -5000.0;
      *(angles + 3) = -5000.0;
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
/* for the purpose of this file I would like to check filtVolts and */
/* unFiltVolts but during flight only filtVolts is needed */
void checkSunSensorHealth(double* filtVolts, double* unFiltVolts){
   getUnfiltVolts(unFiltVolts);
   delay(20);
   getFiltVolts(filtVolts);
   /* health check to be implemented */
}

/* lastly, this is a function used just so we can look inside the wrapper */
/* while it's being tested - it will not be included in the actual wrapper */
int main(){
   double angles[4] = {0, 0, 0, 0};
   double filtVolts[4] = {0, 0, 0, 0};
   double unFiltVolts[4] = {0, 0, 0, 0};
   printf("testing angles\n");
   getSunSensorData(&angles[0]);
   printf("Angles: \n");
   for(int i = 0; i < 4; i++){
      printf("%f",angles[i]);
      printf("\n");
   }
   printf("testing volts\n");
   checkSunSensorHealth(&filtVolts[0], &unFiltVolts[0]);
   printf("Filtered volts: \n");
   for(int i = 0; i < 4; i++){
      printf("%f",filtVolts[i]);
      printf("\n");
   }
   printf("Unfiltered volts: \n");
   for(int i = 0; i < 4; i++){
      printf("%f",unFiltVolts[i]);
      printf("\n");
   }
}
