
#define Address 0x60        //Sun sensor address
#define CommandCode 0x04    //Incoming wrapper command
unsigned char ChecksumC;

void setup() {
  Serial.begin(115200);   //baudrate
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
  unsigned char Command[4] = {Address , CommandCode , 0x01 , ChecksumC};     //Command array setup


  switch (Command[1])
  {
  case 0x01:{       // Unfiltered Photocells Volatages
    float Data[4] = {01000000101010110001000101011100 , 01000000101010110001000101011100 , 01000000101010110001000101011100 , 01000000101010110001000101011100}; //four photocells(floats) at 4 bytes each
    byte b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14,b15,b16;
    b1=((byte)Data[0]&0xFF);
    b2=(((byte)Data[0]>>8)&0xFF);
    b3=(((byte)Data[0]>>16)&0xFF);
    b4=(((byte)Data[0]>>24)&0xFF);
    b5=((byte)Data[1]&0xFF);
    b6=(((byte)Data[1]>>8)&0xFF);
    b7=(((byte)Data[1]>>16)&0xFF);
    b8=(((byte)Data[1]>>24)&0xFF);
    b9=((byte)Data[2]&0xFF);
    b10=(((byte)Data[2]>>8)&0xFF);
    b11=(((byte)Data[2]>>16)&0xFF);
    b12=(((byte)Data[2]>>24)&0xFF);
    b13=((byte)Data[3]&0xFF);
    b14=(((byte)Data[3]>>8)&0xFF);
    b15=(((byte)Data[3]>>16)&0xFF);
    b16=(((byte)Data[3]>>24)&0xFF);
    byte Length = 0x11;
    unsigned char Checksum = Command[1] + Length + (byte)Data[0] + (byte)Data[1] + (byte)Data[2] + (byte)Data[3] - 0x100;
    unsigned char Answer[20] = {Address,Command[1],Length,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14,b15,b16,Checksum};
//    Serial.write(Answer);
    for(int i = 0; i < sizeof(Answer); i++){
        Serial.println(Answer[i],HEX);
        Serial.println(sizeof(Answer[i]));
        delay(1500);}
    delay(5000);
    break;}


  case 0x03:{       // Filtered Photocells Volatages
    float Data[4] = {01000000101010110001000101011100 , 01000000101010110001000101011100 , 01000000101010110001000101011100 , 01000000101010110001000101011100}; //four photocells(floats) at 4 bytes each
    byte b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14,b15,b16;
    b1=((byte)Data[0]&0xFF);
    b2=(((byte)Data[0]>>8)&0xFF);
    b3=(((byte)Data[0]>>16)&0xFF);
    b4=(((byte)Data[0]>>24)&0xFF);
    b5=((byte)Data[1]&0xFF);
    b6=(((byte)Data[1]>>8)&0xFF);
    b7=(((byte)Data[1]>>16)&0xFF);
    b8=(((byte)Data[1]>>24)&0xFF);
    b9=((byte)Data[2]&0xFF);
    b10=(((byte)Data[2]>>8)&0xFF);
    b11=(((byte)Data[2]>>16)&0xFF);
    b12=(((byte)Data[2]>>24)&0xFF);
    b13=((byte)Data[3]&0xFF);
    b14=(((byte)Data[3]>>8)&0xFF);
    b15=(((byte)Data[3]>>16)&0xFF);
    b16=(((byte)Data[3]>>24)&0xFF);
    byte Length = 0x11;
    unsigned char Checksum = byte(Command[1] + Length + (byte)Data[0] + (byte)Data[1] + (byte)Data[2] + (byte)Data[3] - 0x100);
    unsigned char Answer[20] = {Address,Command[1],Length,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14,b15,b16,Checksum};
//    Serial.write(Answer);
    for(int i = 0; i < sizeof(Answer); i++){
      if(b1==(byte)92){
        Serial.println("b1 is correct");
      }
      else{
        Serial.println("b1 is not correct");
      }
        delay(1500);}
    delay(5000);
    break;}


  case 0x04:{       // Angular Position
    int Data[2] = {0x40BBF7CF , 0x40BBF7CF};      //two angles(floats) at 4 bytes each and an error(char) of 1 byte
    unsigned char error = 0;
    byte b1,b2,b3,b4,b5,b6,b7,b8;
    b1=(Data[0]&0xFF);
    b2=(((byte)Data[0]>>8)&0xFF);
    b3=(((byte)Data[0]>>16)&0xFF);
    b4=(((byte)Data[0]>>24)&0xFF);
    b5=((byte)Data[1]&0xFF);
    b6=(((byte)Data[1]>>8)&0xFF);
    b7=(((byte)Data[1]>>16)&0xFF);
    b8=(((byte)Data[1]>>24)&0xFF);
    byte Length = 0x0A;
    unsigned char Checksum = byte(Command[1] + Length + (byte)Data[0] + (byte)Data[1] + (byte)error +  - 0x100);
    unsigned char Answer[7] = {Address,Command[1],Length,(float)Data[0],(float)Data[1],error,Checksum};
//    Serial.write(Answer); 
    for(int i = 0; i < sizeof(Answer); i++){
        Serial.println(Answer[i]);
        Serial.println(sizeof(Answer[i]));
        delay(2500);}
        Serial.println("end");
    delay(5000);   
    break;}
  }
  }
