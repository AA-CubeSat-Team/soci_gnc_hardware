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
    int32_t currSpeed;
    int32_t refSpeed;
} rw1, rw2, rw3, rw4;

double x_dipole;
double y_dipole;
double z_dipole;

void setup() {
  Serial.begin(115200);

  Wire.begin();

  Serial.println("--- --- master setup complete --- ---");
  delay(100);
}

void loop() {
  // step 1 - control outputs from FSW 
  rw1.reqSpeed = 15000; // [0.1 RPM]
  rw2.reqSpeed = 26000;
  rw3.reqSpeed = 37000;
  rw4.reqSpeed = 48000;

  x_dipole = 0.0123; // [A*m^2]
  y_dipole = 0.0456;
  z_dipole = 0.0789;


  // step 2 - command actuators to new values
  // MTQ takes 3 dipole commands
      //  setMags();
  // RWA takes 4 speed commands from rwX.reqSpeed
      //  commandAll_6speed();


  // step 3 - request feedback on current values from actuators (RWA only)
  // RWA updates 4 speed values in rwX.currSpeed
      //  commandAll_4status();
  rw1.currSpeed = 15052; // [0.1 RPM]
  rw2.currSpeed = 26052;
  rw3.currSpeed = 37052;
  rw4.currSpeed = 48052;
 

  // step 4 - give current values back to FSW (send to Simulink)
  // in flight, this will just be handling variables/data types to make acxcessible to control function
  // in FlatSat, need to assemble and send over I2C out to Due and Simulink

  // converts int32_t from RWA to double for FSW
  double rw1_currSpeed_double = (double) rw1.currSpeed;
  double rw2_currSpeed_double = (double) rw2.currSpeed;
  double rw3_currSpeed_double = (double) rw3.currSpeed;
  double rw4_currSpeed_double = (double) rw4.currSpeed;

//          Serial.println(rw1_currSpeed_double,3);

  // split doubles into bytes for serial transmission
  // reaction wheels
  uint8_t rw1_currSpeed_bytes[DOUBLE_SIZE] = {0};
  uint8_t rw2_currSpeed_bytes[DOUBLE_SIZE] = {0};
  uint8_t rw3_currSpeed_bytes[DOUBLE_SIZE] = {0};
  uint8_t rw4_currSpeed_bytes[DOUBLE_SIZE] = {0};

  memcpy(&rw1_currSpeed_bytes[0], &rw1_currSpeed_double, DOUBLE_SIZE);  // little endian
  memcpy(&rw2_currSpeed_bytes[0], &rw2_currSpeed_double, DOUBLE_SIZE);
  memcpy(&rw3_currSpeed_bytes[0], &rw3_currSpeed_double, DOUBLE_SIZE);
  memcpy(&rw4_currSpeed_bytes[0], &rw4_currSpeed_double, DOUBLE_SIZE);
  
  uint8_t rwa_currSpeed_bytes[4*DOUBLE_SIZE] = {0};

  memcpy(&rwa_currSpeed_bytes[0], &rw1_currSpeed_bytes[0], DOUBLE_SIZE);
  memcpy(&rwa_currSpeed_bytes[0]+DOUBLE_SIZE, &rw2_currSpeed_bytes[0], DOUBLE_SIZE);
  memcpy(&rwa_currSpeed_bytes[0]+2*DOUBLE_SIZE, &rw3_currSpeed_bytes[0], DOUBLE_SIZE);
  memcpy(&rwa_currSpeed_bytes[0]+3*DOUBLE_SIZE, &rw4_currSpeed_bytes[0], DOUBLE_SIZE);

  // magnetorquers
  uint8_t x_dipole_bytes[DOUBLE_SIZE] = {0};
  uint8_t y_dipole_bytes[DOUBLE_SIZE] = {0};
  uint8_t z_dipole_bytes[DOUBLE_SIZE] = {0};

  memcpy(&x_dipole_bytes[0], &x_dipole, DOUBLE_SIZE);  // little endian
  memcpy(&y_dipole_bytes[0], &y_dipole, DOUBLE_SIZE);
  memcpy(&z_dipole_bytes[0], &z_dipole, DOUBLE_SIZE);

  uint8_t xyz_dipole_bytes[3*DOUBLE_SIZE] = {0};

  memcpy(&xyz_dipole_bytes[0], &x_dipole_bytes[0], DOUBLE_SIZE);
  memcpy(&xyz_dipole_bytes[0]+DOUBLE_SIZE, &y_dipole_bytes[0], DOUBLE_SIZE);
  memcpy(&xyz_dipole_bytes[0]+2*DOUBLE_SIZE, &z_dipole_bytes[0], DOUBLE_SIZE);

  // both actuators
  const int act_feedback_len = sizeof(rwa_currSpeed_bytes) + sizeof(xyz_dipole_bytes);
  uint8_t act_feedback_bytes[act_feedback_len] = {0};

  memcpy(&act_feedback_bytes[0], &rwa_currSpeed_bytes[0], sizeof(rwa_currSpeed_bytes));
  memcpy(&act_feedback_bytes[0]+sizeof(rwa_currSpeed_bytes), &xyz_dipole_bytes[0], sizeof(xyz_dipole_bytes));
  
  
  Wire.beginTransmission(ACT_DUE_ADDRESS);
  Wire.write(&act_feedback_bytes[0], act_feedback_len); 
  Wire.endTransmission(true);

  Serial.print("rwa_currSpeed_bytes:\t");
  for(int ii=0;ii<sizeof(rwa_currSpeed_bytes);ii++){
    Serial.print(rwa_currSpeed_bytes[ii],HEX);
    Serial.print(" ");
  }
  Serial.println(" ");

  delay(5000);
}
