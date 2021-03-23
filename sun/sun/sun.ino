
#define Address 0x60               // Define sun sensor address
//#define CommandCode 0x04           // Define simulated incoming wrapper command
//byte ChecksumC;                    // Initialize Simulated incoming checksum
byte Command[4];                   // Initialize command receiving array


// Unfiltered voltage simulated data
float DataV[] = {5.87400007248 , 5.87400007248 , 5.87400007248 , 5.87400007248};         // 5.87400007248 = 0x40BBF7CF
// Filtered voltage simulated data
float DataF[] = {5.87400007248 , 5.87400007248 , 5.87400007248 , 5.87400007248};
// Angles and error simulated data
float DataA[] = {5.87400007248 , 5.87400007248};
unsigned char error = 0;


void setup() {
  Serial.begin(115200, SERIAL_8N1);            // Set Baudrate
}


void loop() {
  //////////// This Block was for prelimenary testing ///////////////////////////////////////////////////
//switch (CommandCode)
//{
//  case 0x01:
//   ChecksumC = 0x02;
//  case 0x03:
//   ChecksumC = 0x04;
//  case 0x04:
//   ChecksumC = 0x05;
//}
//byte Command[4] = {Address , CommandCode , 0x01 , ChecksumC};          //Command array setup
  ///////////////////////////////////////////////////////////////////////////////////////////////////////


// Read incoming command packet and populate Command array when serial data is available
if(Serial.available()) {
// Wait for entire command package to enter buffer
  delay(10);
// Incoming address
  Command[0] = Serial.read();
// Check for correct address before proceeding 
  if (Command[0] == 0x60){ 
// Incoming command
  Command[1] = Serial.read();
// Incoming length
  Command[2] = Serial.read();
// Incoming checksum
  Command[3] = Serial.read();


// Switch statement for the different commands
switch (Command[1])
{
  case 0x01:{        // Unfiltered Photocells Volatages
// Next eight lines take the simulated data array and reduces it to individual bytes for transmission via pointers
    byte Data1[sizeof DataV[0]];
    memcpy(Data1,&DataV[0],sizeof DataV[0]);
    byte Data2[sizeof DataV[1]];
    memcpy(Data2,&DataV[1],sizeof DataV[1]);
    byte Data3[sizeof DataV[2]];
    memcpy(Data3,&DataV[2],sizeof DataV[2]);
    byte Data4[sizeof DataV[3]];
    memcpy(Data4,&DataV[3],sizeof DataV[3]);
// Data sheet defined length parameter
    byte Length = 0x11;
// Next two lines calculate the checksum by first adding the data sheet specified values, then extracting the least significant byte
    byte Checksum = byte(Command[1] + Length + Data1[0] + Data1[1] + Data1[2] + Data1[3] + Data2[0] + Data2[1] + Data2[2] + Data2[3] + Data3[0] + Data3[1] + Data3[2] + Data3[3] + Data4[0] + Data4[1] + Data4[2] + Data4[3]);
    Checksum = Checksum & 0xFF;
// Populate Answer packet array
    byte Answer[] = {Address,Command[1],Length,Data1[0],Data1[1],Data1[2],Data1[3],Data2[0],Data2[1],Data2[2],Data2[3],Data3[0],Data3[1],Data3[2],Data3[3],Data4[0],Data4[1],Data4[2],Data4[3],Checksum};
// Write Answer packet to wrapper
    Serial.write(Answer,20);
    break;}

// The next two cases operate in the exact same manner
  case 0x03:{      // Filtered Photocells Volatages
    byte Data1[sizeof DataF[0]];
    memcpy(Data1,&DataF[0],sizeof DataF[0]);
    byte Data2[sizeof DataF[1]];
    memcpy(Data2,&DataF[1],sizeof DataF[1]);
    byte Data3[sizeof DataF[2]];
    memcpy(Data3,&DataF[2],sizeof DataF[2]);
    byte Data4[sizeof DataF[3]];
    memcpy(Data4,&DataF[3],sizeof DataF[3]);
    byte Length = 0x11;
    byte Checksum = byte(Command[1] + Length + Data1[0] + Data1[1] + Data1[2] + Data1[3] + Data2[0] + Data2[1] + Data2[2] + Data2[3] + Data3[0] + Data3[1] + Data3[2] + Data3[3] + Data4[0] + Data4[1] + Data4[2] + Data4[3]);
    Checksum = Checksum & 0xFF;
    byte Answer[] = {Address,Command[1],Length,Data1[0],Data1[1],Data1[2],Data1[3],Data2[0],Data2[1],Data2[2],Data2[3],Data3[0],Data3[1],Data3[2],Data3[3],Data4[0],Data4[1],Data4[2],Data4[3],Checksum};
    Serial.write(Answer,20);
    break;}


  case 0x04:{      // Angular Position
    byte Data1[sizeof DataA[0]];
    memcpy(Data1,&DataA[0],sizeof DataA[0]);
    byte Data2[sizeof DataA[1]];
    memcpy(Data2,&DataA[1],sizeof DataA[1]);  
    byte Length = 0x0A;
    byte Checksum = byte(Command[1] + Length + Data1[0] + Data1[1] + Data1[2] + Data1[3] + Data2[0] + Data2[1] + Data2[2] + Data2[3] + error);
    Checksum = Checksum & 0xFF;
    byte Answer[] = {Address,Command[1],Length,Data1[0],Data1[1],Data1[2],Data1[3],Data2[0],Data2[1],Data2[2],Data2[3],error,Checksum};
    Serial.write(Answer,13);   
    break;}
  }
  }
}
}
