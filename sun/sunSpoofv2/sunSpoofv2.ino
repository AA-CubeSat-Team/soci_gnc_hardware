
#define Address 0x60               // Define sun sensor address
byte Command[4];                   // Initialize command receiving array
<<<<<<< HEAD
byte DataA[3];
double sunDoubleData[4];
double doubleBuffer[4];
byte error;

void setup() {
  Serial.begin(115200);            // Set Baudrate
  Serial1.begin(115200);
=======
byte DataA[2];
double sunDoubleData[2];

void setup() {
  Serial.begin(115200, SERIAL_8N1);            // Set Baudrate
>>>>>>> cea76cdfbc840620d6891f33173ddc90a29359a2
}

void serialRecieveSimulink(double * doubleBuffer, int n) {
  while (Serial.available() < n*sizeof(double)){
  }
  uint8_t doubleConvertor[sizeof(double)];
  for (int ii = 0; ii < n; ii++) {
    Serial.readBytes(doubleConvertor,sizeof(doubleConvertor));
    memcpy(doubleBuffer + ii, doubleConvertor, sizeof(double));
  }
}

void serialSendOBC() {
  // Angular Position sent to OBC
    DataA[0] = (float)doubleBuffer[0];
    DataA[1] = (float)doubleBuffer[1];
<<<<<<< HEAD
    DataA[2] = (float)doubleBuffer[2];
    error = (byte)doubleBuffer[3];
=======
>>>>>>> cea76cdfbc840620d6891f33173ddc90a29359a2
    byte Data1[sizeof DataA[0]];
    memcpy(Data1,&DataA[0],sizeof DataA[0]);
    byte Data2[sizeof DataA[1]];
    memcpy(Data2,&DataA[1],sizeof DataA[1]);  
<<<<<<< HEAD
    byte Data3[sizeof DataA[2]];
    memcpy(Data3,&DataA[2],sizeof DataA[2]);
    byte Length = 0x0E;
    byte Checksum = byte(Command[1] + Length + Data1[0] + Data1[1] + Data1[2] + Data1[3] + Data2[0] + Data2[1] + Data2[2] + Data2[3] + Data3[0] + Data3[1] + Data3[2] + Data3[3] + error);
    Checksum = Checksum & 0xFF;
    byte Answer[] = {Address,Command[1],Length,Data1[0],Data1[1],Data1[2],Data1[3],Data2[0],Data2[1],Data2[2],Data2[3],Data3[0],Data3[1],Data3[2],Data3[3],error,Checksum};
    Serial1.write(Answer,17);
=======
    byte Length = 0x0A;
    byte Checksum = byte(Command[1] + Length + Data1[0] + Data1[1] + Data1[2] + Data1[3] + Data2[0] + Data2[1] + Data2[2] + Data2[3] + error);
    Checksum = Checksum & 0xFF;
    byte Answer[] = {Address,Command[1],Length,Data1[0],Data1[1],Data1[2],Data1[3],Data2[0],Data2[1],Data2[2],Data2[3],error,Checksum};
    Serial1.write(Answer,13);   
    break;
>>>>>>> cea76cdfbc840620d6891f33173ddc90a29359a2
}

void serialRecieveOBC() {
// Read incoming command packet from OBC and populate Command array when serial data is available
if(Serial1.available()) {
// Wait for entire command package to enter buffer
  delay(10);
// Incoming address
  Command[0] = Serial1.read();
// Check for correct address before proceeding 
  if (Command[0] == 0x60){ 
// Incoming command
  Command[1] = Serial1.read();
// Incoming length
  Command[2] = Serial1.read();
// Incoming checksum
  Command[3] = Serial1.read();

serialRecieveSimulink(sunDoubleData, 2);
serialSendOBC();
  }
  }
}

void loop() {
  serialRecieveOBC();
}
