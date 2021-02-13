//SPI MASTER (ARDUINO)

#include<SPI.h>                              

int SS1 = 2;
int x;

void setup (void){
  Serial.begin(115200);                   
  
  SPI.begin();                            
  SPI.setClockDivider(SPI_CLOCK_DIV8);  
  
  pinMode(SS1,OUTPUT);
  digitalWrite(2,HIGH);

  Serial.println("master setup complete");
}

void loop(void){
  byte masterSend, masterReceive;          

  x = 1;
  masterSend = x; 

  Serial.print("masterSend (BIN):\t");
  Serial.println(masterSend,BIN);
  
  digitalWrite(SS1,LOW);                                     
  masterReceive = SPI.transfer(masterSend); 
  digitalWrite(SS1,HIGH);

  Serial.print("masterReceive (BIN):\t");
  Serial.println(masterReceive,BIN);
  
  delay(5000);
}
