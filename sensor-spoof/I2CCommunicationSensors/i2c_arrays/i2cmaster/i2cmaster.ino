#include <Wire.h>
#define SLAVE_ADDR 9
#define ANSWERSIZE 5

int start = 0;
int d; 
int Address;           //Store the array address
char t[10]={};     //create a array to receive the incoming data from the slave
int pulse[12]={};

void setup() {
  Wire.begin();
  Serial.begin(9600);
}

void loop(){
  if (start == 0){   //To run one time only the code
 
  for (int i = 0; i < 12; i++){         //to request the data from 12 array addresses

    Address = i;
    Serial.println("I send");
    Serial.println(Address);        //Print the array adcress requested by the master
   
    Wire.beginTransmission (SLAVE_ADDR);    //Start I2C communication with the slave
    Wire.write (Address);                         
    Wire.endTransmission ();       
   
    delay(10);                  // Adding some delay to avoid error when changing from send to read...  (I tested without it, it's working, but I find safer to add it)
   
    Wire.requestFrom(SLAVE_ADDR,ANSWERSIZE);     //request the slave to send the requested Data
   
 
    t[d] = Wire.read();         // Put the receving data in the array t
    d++;
    if (d >= 3){
      d = 0;
      }             // Return the cursor of array t to 0 when we receive the 3 wanted bytes
    
    Serial.println("Received Data");
    Serial.println(t);          // Received ASII Data
   
    pulse[Address]=((((((t[0])-48)*10)+((t[1])-48))*10)+((t[2])-48));  //Convert ASII to DECIMAL to store in a int.      In ASII 0 is 48, 1 is 49, 2 is 50.... so if we sustrac 48 to the ASII number we get an DECIMAL number
    Serial.println(pulse[Address]);  //Print the received Data
  } // end of for loop

 
  Serial.print("\r\n");  //Skip one line
  Serial.println("All Received Data");  //Print all the received Data
  for (int y = 0; y < 12; y++){
    Serial.println(pulse[y]);
    }   //Display all the data received
 
  }   // end of if

 
  start = 1;

} 
