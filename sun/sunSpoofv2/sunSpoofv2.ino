
#define Address 0x60               // Define sun sensor address
byte Command[4];                   // Initialize command receiving array
byte DataA[2];
double sunDoubleData[2];

void setup() {
  Serial.begin(115200, SERIAL_8N1);            // Set Baudrate
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
    byte Data1[sizeof DataA[0]];
    memcpy(Data1,&DataA[0],sizeof DataA[0]);
    byte Data2[sizeof DataA[1]];
    memcpy(Data2,&DataA[1],sizeof DataA[1]);  
    byte Length = 0x0A;
    byte Checksum = byte(Command[1] + Length + Data1[0] + Data1[1] + Data1[2] + Data1[3] + Data2[0] + Data2[1] + Data2[2] + Data2[3] + error);
    Checksum = Checksum & 0xFF;
    byte Answer[] = {Address,Command[1],Length,Data1[0],Data1[1],Data1[2],Data1[3],Data2[0],Data2[1],Data2[2],Data2[3],error,Checksum};
    Serial1.write(Answer,13);   
    break;
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
}
