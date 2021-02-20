//SPI SLAVE (ARDUINO)

#include<SPI.h>

volatile boolean process_flag;
volatile byte read_reg;
volatile byte receive_array[4] = { };
volatile byte send_array[4] = { };
int x;
int rr;

void setup(){
  Serial.begin(115200);
  
  pinMode(MISO,OUTPUT);

  SPCR = SPCR | bit(SPE);           // sets SPE bit in SPCR to 1, enabling SPI
  
  process_flag = false;

  SPI.attachInterrupt();                  

  Serial.println("slave setup complete");
}

ISR (SPI_STC_vect){
  if (rr >= 3){ 
    SPI.detachInterrupt();                  
    process_flag = true;   
  }  
  receive_array[rr] = SPDR;
  SPDR = send_array[rr]; 
  rr++;          
}

void loop(){ 
  if(process_flag){
    Serial.println("processing");
    Serial.println(rr);
    
    send_array[0] = receive_array[0] + 1;
    send_array[1] = receive_array[1] + 1;
    send_array[2] = receive_array[2] + 1;
    send_array[3] = receive_array[3] + 1;

    delay(1000);
                               
    rr = 0;
    process_flag == false;
    read_reg = SPSR;
    read_reg = SPDR;
    SPCR = SPCR | bit(SPIE);
  }
}
