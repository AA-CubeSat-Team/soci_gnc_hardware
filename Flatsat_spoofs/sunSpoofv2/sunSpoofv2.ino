//// Sun sensor spoof for communication between the OBC and Simulink ////

#define Address 0x60               // Define sun sensor address
uint8_t Command[4] = {0x60, 0x04, 0x01, 0x05};                   // Initialize command receiving array
float DataA[3] = {89.99, 89.99, 98.99};
float sunDoubleData[3];
byte doubleBuffer[8];
byte error;
int received;

void setup() {
  Serial.begin(115200);            // Set Baudrate
  Serial1.begin(115200);
//  SerialUSB.begin(115200);            // Set Baudrate
}

void serialRecieveSimulink(byte * doubleBuffer, int n) {
//  while (Serial.available() < n*sizeof(double)){
//  }
//  uint8_t doubleConvertor[sizeof(double)];
//  for (int ii = 0; ii < n; ii++) {
//    Serial.readBytes(doubleConvertor,sizeof(doubleConvertor));
//    memcpy(doubleBuffer + ii, doubleConvertor, sizeof(doubleConvertor));
//    Serial.write(, sizeof(doubleBuffer[ii]));
////    SerialUSB.print("Receive from Simulink:");
////    SerialUSB.println(doubleBuffer[ii]);
//  }

  while (Serial.available() < n*sizeof(float)){
  }
  Serial.readBytes((char*)doubleBuffer, n*sizeof(float));
  Serial.write((char*)doubleBuffer, n*sizeof(float));
}

void serialSendOBC() {
  // Angular Position sent to OBC
    byte Data1[4];
    for (int i = 0; i < 4; i++) {
      Data1[i] = doubleBuffer[i];
    }
//    memcpy(Data1,&DataA[0],4);
    byte Data2[4];
    for (int j = 0; j < 4; j++) {
      Data2[j] = doubleBuffer[j+4];
    }
//    memcpy(Data2,&DataA[1],4);  
    byte Data3[4];
    memcpy(Data3,&DataA[2],4);
    byte Length = 0x0E;
    byte Checksum = byte(Command[1] + Length + Data1[0] + Data1[1] + Data1[2] + Data1[3] + Data2[0] + Data2[1] + Data2[2] + Data2[3] + Data3[0] + Data3[1] + Data3[2] + Data3[3] + error);
    Checksum = Checksum & 0xFF;
    byte Answer[] = {Address,Command[1],Length,Data1[0],Data1[1],Data1[2],Data1[3],Data2[0],Data2[1],Data2[2],Data2[3],Data3[0],Data3[1],Data3[2],Data3[3],error,Checksum};
    Serial.println("Send to OBC:");
    for(int i = 0; i < 17; i++){
//      Serial.println(Answer[i], HEX);
      Serial1.write(Answer[i]);
    }
    Serial.println();
    for(int i = 0; i < 4; i++){
      Serial.print(Data1[i], HEX);
    }
    Serial.println();
    for(int i = 0; i < 4; i++){
      Serial.print(Data2[i], HEX);
    }
    Serial.println();
    for(int i = 0; i < 4; i++){
      Serial.print(Data3[i], HEX);
    }
    Serial.println();
    delay(1000);
    for (int j = 0; j < 17; j++) {
      Serial1.write(Answer[j]);
      delay(1000);
    }
    Serial1.write(Answer, sizeof(Answer));
//    delay(2000);
}

void serialRecieveOBC() {
// Read incoming command packet from OBC and populate Command array
// Incoming address
    Command[0] = Serial1.read();
//  if (Command[0] == 0x60){ 
    // Incoming command
    Command[1] = Serial1.read();
//    if (Command[1] == 0x04){ 
      // Incoming length
    Command[2] = Serial1.read();
      // Incoming checksum
    Command[3] = Serial1.read();
    Serial.println();
    for (int i = 0; i < sizeof(Command); i++) {
      Serial.println(Command[i], HEX);
    }
    Serial.println();

//  while (Serial1.available()) {
//    Serial1.read();
//  }
//      
//      received = 1;
//      delay(100);
//    } 
//    else {
//      while(Serial1.available()) {
//      Serial1.read();
//      }  
//    }
//  }else {
//      while(Serial1.available()) {
//      Serial1.read();
//      }
//  }
//  }
}

void loop() {
//  Serial.println("TOP");
  if(Serial.available()){
    serialRecieveSimulink(doubleBuffer, 2);
//    buffer2float(doubleBuffer, sunDoubleData, 2);
//    DataA[0] = doubleBuffer[0];
//    DataA[1] = doubleBuffer[1];
    DataA[2] = 99;
//    error = (byte)sunDoubleData[3];
  }
  if(Serial1.available()){
    serialRecieveOBC();
  } else if (received) {
    serialSendOBC();
    received = 0;
  }
}

void buffer2float(byte * in_bytes, float * out_Floats, int n)
{
//  for (int i = 0; i < n; i+=4) {
//    uint32_t num;
//    num = (in_bytes[i] << 24) | (in_bytes[i+1] << 16) | (in_bytes[i+2] << 8) | (in_bytes[i+3]);
//    out_Floats[i] = *((float *)&num);
//  }
  for (int i = 0; i < n; i++) {
    union {
      byte temp_bytes[4];
      float temp_f;
    } u;

    u.temp_f = in_bytes[i];
    
  }
}
