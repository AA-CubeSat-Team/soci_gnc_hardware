//// Sun sensor spoof for communication between the OBC and Simulink ////

#define COMMAND_BYTES  4
#define ADDRESS 0x60               // Define sun sensor address
byte commandRecieved[COMMAND_BYTES];                   // Initialize command receiving array
byte dataSent[17];
float sunFloatData[3] = {1.1, 2.2, 3.3};

void setup() {
  Serial.begin(115200);            // Set Baudrate
  Serial1.begin(115200);

}

void loop() {
  serialRecieve(sunFloatData, 3);
  recieveOBC();
}

void recieveOBC() {
  if (Serial1.available() >= COMMAND_BYTES) {
    Serial1.readBytes(commandRecieved, COMMAND_BYTES);
    if (commandRecieved[0] == ADDRESS) {
      sendOBC();
    }
  }
}

void sendOBC() {
  memcpy(dataSent,commandRecieved, COMMAND_BYTES);
  memcpy(dataSent+3, (char*)sunFloatData, 12);
  dataSent[15] = 0;
  int checkSum = 0;
  for (int i = 1; i < 16; i++) {
    checkSum += dataSent[i];
  }
  dataSent[16] = (byte) (checkSum & 0xff);
  Serial1.write(dataSent, 17);
}

void serialRecieve(float * floatBuffer, int n)
{
  if (Serial.available() >= n*sizeof(float)){
    Serial.readBytes((char*)floatBuffer,n*sizeof(float));
    Serial.write((char*)floatBuffer,n*sizeof(float));
  }
}
