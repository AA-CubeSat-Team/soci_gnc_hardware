
#define Address 0x60               // Sun sensor address
#define CommandCode 0x03           // Incoming wrapper command
byte ChecksumC;

void setup() {
  Serial.begin(115200);            // Baudrate
}

void loop() {
//  Serial.read(Command);
  switch (CommandCode)
  {
    case 0x01:
     ChecksumC = 0x02;
    case 0x03:
     ChecksumC = 0x04;
    case 0x04:
     ChecksumC = 0x05;
  }
  byte Command[4] = {Address , CommandCode , 0x01 , ChecksumC};                          //Command array setup


  switch (Command[1])
  {
  case 0x01:{                                                                            // Unfiltered Photocells Volatages
    float Data[] = {5.87400007248 , 5.87400007248 , 5.87400007248 , 5.87400007248};      // Four photocells(floats)
    byte Data1[sizeof Data[0]];
    memcpy(Data1,&Data[0],sizeof Data[0]);
    byte Data2[sizeof Data[1]];
    memcpy(Data2,&Data[1],sizeof Data[1]);
    byte Data3[sizeof Data[2]];
    memcpy(Data3,&Data[2],sizeof Data[2]);
    byte Data4[sizeof Data[3]];
    memcpy(Data4,&Data[3],sizeof Data[3]);
    byte Length = 0x11;
    byte Checksum = byte(Command[1] + Length + Data1[0] + Data1[1] + Data1[2] + Data1[3] + Data2[0] + Data2[1] + Data2[2] + Data2[3] + Data3[0] + Data3[1] + Data3[2] + Data3[3] + Data4[0] + Data4[1] + Data4[2] + Data4[3]);
    byte Answer[] = {Address,Command[1],Length,Data1[0],Data1[1],Data1[2],Data1[3],Data2[0],Data2[1],Data2[2],Data2[3],Data3[0],Data3[1],Data3[2],Data3[3],Data4[0],Data4[1],Data4[2],Data4[3],Checksum};
//    Serial.write(Answer);
    for(int i = 0; i < sizeof Answer; i++){
        Serial.println(Answer[i],HEX);
        Serial.println(sizeof Answer[i],HEX);} 
        Serial.println("end");
    delay(5000);
    break;}


  case 0x03:{                                                                           // Filtered Photocells Volatages
    float Data[] = {5.87400007248 , 5.87400007248 , 5.87400007248 , 5.87400007248};     // Four photocells(floats)
    byte Data1[sizeof Data[0]];
    memcpy(Data1,&Data[0],sizeof Data[0]);
    byte Data2[sizeof Data[1]];
    memcpy(Data2,&Data[1],sizeof Data[1]);
    byte Data3[sizeof Data[2]];
    memcpy(Data3,&Data[2],sizeof Data[2]);
    byte Data4[sizeof Data[3]];
    memcpy(Data4,&Data[3],sizeof Data[3]);
    byte Length = 0x11;
    byte Checksum = byte(Command[1] + Length + Data1[0] + Data1[1] + Data1[2] + Data1[3] + Data2[0] + Data2[1] + Data2[2] + Data2[3] + Data3[0] + Data3[1] + Data3[2] + Data3[3] + Data4[0] + Data4[1] + Data4[2] + Data4[3]);
    byte Answer[] = {Address,Command[1],Length,Data1[0],Data1[1],Data1[2],Data1[3],Data2[0],Data2[1],Data2[2],Data2[3],Data3[0],Data3[1],Data3[2],Data3[3],Data4[0],Data4[1],Data4[2],Data4[3],Checksum};
//    Serial.write(Answer);
    for(int i = 0; i < sizeof Answer; i++){
        Serial.println(Answer[i],HEX);
        Serial.println(sizeof Answer[i],HEX);} 
        Serial.println("end");
    delay(5000);
    break;}


  case 0x04:{                                            // Angular Position
    float Data[] = {5.87400007248 , 5.87400007248};      // Two angles(floats)
    unsigned char error = 0;                             // Error
    byte Data1[sizeof Data[0]];
    memcpy(Data1,&Data[0],sizeof Data[0]);
    byte Data2[sizeof Data[1]];
    memcpy(Data2,&Data[1],sizeof Data[1]);  
    byte Length = 0x0A;
    byte Checksum = byte(Command[1] + Length + Data1[0] + Data1[1] + Data1[2] + Data1[3] + Data2[0] + Data2[1] + Data2[2] + Data2[3] + error);
    Checksum = Checksum & 0xFF;
    byte Answer[] = {Address,Command[1],Length,Data1[0],Data1[1],Data1[2],Data1[3],Data2[0],Data2[1],Data2[2],Data2[3],error,Checksum};
//    Serial.write(Answer); 
    for(int i = 0; i < sizeof Answer; i++){
        Serial.println(Answer[i],HEX);
        Serial.println(sizeof Answer[i],HEX);} 
        Serial.println("end");
    delay(5000);   
    break;}
  }
  }
