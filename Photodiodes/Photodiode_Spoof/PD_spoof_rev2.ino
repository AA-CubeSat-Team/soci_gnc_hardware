#include <Wire.h>

// Givens
#define R 10000
#define V_ref 3.3
#define address 0x1D // ADC address

// Function Variables 
double PD_comp;           //array of inputs
uint16_t D_out_12bit[5];  // Array of spoof outputs
  
// Testing function
double test_one = .000001;
double test_two = .000002;
double test_three = .000003;
double test_four = .000004;
double test_five = .000005;
  
void setup(){
  // Using PD spoof with arbitrary inputs
  PD_spoof(test_one, test_two, test_three, test_four, test_five);

// Printing outputs
Serial.begin (9600); //Declare baude rate
Serial.println("Input of Photodiode Spoof");
Serial.println(test_one);
Serial.println("Output of Photodiode Spoof, double");
Serial.println(D_out_12bit[0]);
Serial.println(D_out_12bit[1]);
Serial.println(D_out_12bit[2]);
Serial.println(D_out_12bit[3]);
Serial.println(D_out_12bit[4]);
}
void loop()
{
  Wire.onReceive(PD_spoof);
}

// Photodiode spoof
void PD_spoof(double pd1, double pd2, double pd3, double pd4, double pd5){
  // Initialization
  
  // Compiling PD: 
  double PD_comp[5] = {pd1, pd2, pd3, pd4, pd5};
  // Calculating Digital signal: (page 28)
  for (int i =0; i <5; i++) 
  {
    // Combination of Ohm's law for op amp ( Resistor = 10 kOhms ) and digital signal equation (page 28)
    D_out_12bit[i] = int(PD_comp[i]*R/V_ref * pow(2,12)); 
  }
   // I2C communication
    Wire.beginTransmission(address);
    while (Wire.available()){
    Wire.write(D_out_12bit[0]);
    Wire.write(D_out_12bit[1]);
    Wire.write(D_out_12bit[2]);
    Wire.write(D_out_12bit[3]);
    Wire.write(D_out_12bit[4]);
    Wire.endTransmission();
    }
}

  
