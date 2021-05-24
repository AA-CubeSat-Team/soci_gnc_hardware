// ACTUATOR FEEDBACK MASTER (ARDUINO) - TEST CODE

// OBC: collects current actuator values from actuator wrapper, transmits to Due over I2C (as master)

/* actuator system overview:
 * 1. FSW generates control outputs (4 RWA speeds, 3 MTQ dipoles)
 * 2. command actuators to new values
 *  2.a. command MTQ
 *  2.b. command RWA
 * 3. request feedback on current values of actuators (RWA only) - can get PWMs if desired
 *  3.a. request RWA
 * 4. give current values back to FSW (send to Simulink)
 */

#include <Wire.h> 

#define ACT_DUE_ADDRESS 0x18
#define DOUBLE_SIZE 8

struct rw_data
{
    int32_t reqSpeed;
    double reqSpeed_double;
    int32_t currSpeed;
    double currSpeed_double;
    int32_t refSpeed;
} rw1, rw2, rw3, rw4;

double x_reqDipole;
double y_reqDipole;
double z_reqDipole;

double x_currDipole;
double y_currDipole;
double z_currDipole;

void setup() {
  Wire.begin();
}

void loop() {
  // step 1 - control outputs from FSW 
  rw1.reqSpeed = 15000; // [0.1 RPM]
  rw2.reqSpeed = 26000;
  rw3.reqSpeed = 37000;
  rw4.reqSpeed = 48000;

  x_reqDipole = 0.0123; // [A*m^2]
  y_reqDipole = 0.0456;
  z_reqDipole = 0.0789;


  // step 2 - command actuators to new values
  // MTQ takes 3 dipole commands
      //  setMags();
  // RWA takes 4 speed commands from rwX.reqSpeed
      //  commandAll_6speed();


  // step 3 - request feedback on current values from actuators (RWA only)
  // RWA updates 4 speed values in rwX.currSpeed
      //  commandAll_4status();
  rw1.currSpeed = 12057; // [0.1 RPM]
  rw2.currSpeed = 24057;
  rw3.currSpeed = 35057;
  rw4.currSpeed = 69057;

  // converts int32_t from RWA to double for FSW
  rw1.currSpeed_double = (double) rw1.currSpeed;
  rw2.currSpeed_double = (double) rw2.currSpeed;
  rw3.currSpeed_double = (double) rw3.currSpeed;
  rw4.currSpeed_double = (double) rw4.currSpeed;

  x_currDipole = x_reqDipole;
  y_currDipole = y_reqDipole;
  z_currDipole = z_reqDipole;


  // step 4 - send values to Simulink (FlatSat only)
  actFeedbackI2C();


  delay(5000);
}
 
void actFeedbackI2C(){
  // FlatSat only
  
  // splits doubles into byte arrays
  uint8_t rw1_currSpeed_bytes[DOUBLE_SIZE] = {0};
  uint8_t rw2_currSpeed_bytes[DOUBLE_SIZE] = {0};
  uint8_t rw3_currSpeed_bytes[DOUBLE_SIZE] = {0};
  uint8_t rw4_currSpeed_bytes[DOUBLE_SIZE] = {0};
  
  uint8_t rwa_currSpeed_bytes[4*DOUBLE_SIZE] = {0};

  memcpy(&rw1_currSpeed_bytes[0], &rw1.currSpeed_double, DOUBLE_SIZE);  // little endian
  memcpy(&rw2_currSpeed_bytes[0], &rw2.currSpeed_double, DOUBLE_SIZE);
  memcpy(&rw3_currSpeed_bytes[0], &rw3.currSpeed_double, DOUBLE_SIZE);
  memcpy(&rw4_currSpeed_bytes[0], &rw4.currSpeed_double, DOUBLE_SIZE);

  memcpy(&rwa_currSpeed_bytes[0], &rw1_currSpeed_bytes[0], DOUBLE_SIZE);
  memcpy(&rwa_currSpeed_bytes[0]+DOUBLE_SIZE, &rw2_currSpeed_bytes[0], DOUBLE_SIZE);
  memcpy(&rwa_currSpeed_bytes[0]+2*DOUBLE_SIZE, &rw3_currSpeed_bytes[0], DOUBLE_SIZE);
  memcpy(&rwa_currSpeed_bytes[0]+3*DOUBLE_SIZE, &rw4_currSpeed_bytes[0], DOUBLE_SIZE);

  uint8_t x_currDipole_bytes[DOUBLE_SIZE] = {0};
  uint8_t y_currDipole_bytes[DOUBLE_SIZE] = {0};
  uint8_t z_currDipole_bytes[DOUBLE_SIZE] = {0};
  
  uint8_t mtq_currDipole_bytes[3*DOUBLE_SIZE] = {0};

  memcpy(&x_currDipole_bytes[0], &x_currDipole, DOUBLE_SIZE);
  memcpy(&y_currDipole_bytes[0], &y_currDipole, DOUBLE_SIZE);
  memcpy(&z_currDipole_bytes[0], &z_currDipole, DOUBLE_SIZE);

  memcpy(&mtq_currDipole_bytes[0], &x_currDipole_bytes[0], DOUBLE_SIZE);
  memcpy(&mtq_currDipole_bytes[0]+DOUBLE_SIZE, &y_currDipole_bytes[0], DOUBLE_SIZE);
  memcpy(&mtq_currDipole_bytes[0]+2*DOUBLE_SIZE, &z_currDipole_bytes[0], DOUBLE_SIZE);

  // transmits to Due in two transmissions
  Wire.beginTransmission(ACT_DUE_ADDRESS);
  Wire.write(&rwa_currSpeed_bytes[0], 4*DOUBLE_SIZE); 
  Wire.endTransmission(true);
  
  Wire.beginTransmission(ACT_DUE_ADDRESS);
  Wire.write(&mtq_currDipole_bytes[0], 3*DOUBLE_SIZE); 
  Wire.endTransmission(true);
}

  
