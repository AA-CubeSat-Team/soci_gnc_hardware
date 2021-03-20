
// Givens
double R = 10000;
double V_ref = 3.3;

// Function Variables 
double PD_comp;           //array of inputs
uint16_t D_out_12bit[5];  // Array of spoof outputs

// Photodiode spoof
void PD_spoof(double pd1, double pd2, double pd3, double pd4, double pd5){
  // Compiling PD: 
  double PD_comp[5] = {pd1, pd2, pd3, pd4, pd5};
  

   // Calculating Digital signal: (ADC datasheet page 28)
  for (int i =0; i <5; i++) 
  {
    // Combination of Ohm's law for op amp ( Resistor = 10 kOhms ) and digital signal equation (ADC datasheet)
    D_out_12bit[i] = int(PD_comp[i]*R/V_ref * pow(2,12)); 
  }
}
