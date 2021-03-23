double gyroDoubleData[3]; // gyro as an example
int n = 3;
void setup()
{
  Serial.begin(115200);
}

void loop() 
{
  serialRecieve(gyroDoubleData, n);
}

/*!
 * @brief save n number of doubles sent from simulink to the doubleBuffer.
 *
 *
 * @param doubleBuffer  The buffer that will store the values sent from simulink.
 * @param n             The number of doubles needed from one set of data, which 
 *                      depends on the type of sensor. e.g. gyrosocpe has three 
 *                      readings for for three axes, so n is 3.
 * @return void
 *
 */
void serialRecieve(double * doubleBuffer, int n)
{
  while (Serial.available() < n*sizeof(double)){
  }
  uint8_t doubleConvertor[sizeof(double)];
  for (int ii = 0; ii < n; ii++) {
    Serial.readBytes(doubleConvertor,sizeof(doubleConvertor));
    memcpy(doubleBuffer + ii, doubleConvertor, sizeof(double));
  }
}
