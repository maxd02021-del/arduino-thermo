#include <math.h>
const int B = 4275000; // B value of the thermistor
const int R0 = 100000; // R0 = 100k
const int pinTempSensor = A0; // Grove - Temperature Sensor connect to A0

struct sensorData {
  float T0;
  float T1;
  float deltaT;
  float dT_dt;
} tempData;

void setup()
{
  Serial.begin(9600);
}

float delayTime = 100;

void loop()
{
  int a = analogRead(pinTempSensor);
  float R = 1023.0/a-1.0;
  R = R0*R;
  tempData.T0 = 1.0/(log(R/R0)/B+1/298.15)-273.15; // convert to temperature via datasheet
  delay(delayTime);

  a = analogRead(pinTempSensor);
  R = 1023.0/a-1.0;
  R = R0*R;
  tempData.T1 = 1.0/(log(R/R0)/B+1/298.15)-273.15; // convert to temperature via datasheet
  
  tempData.deltaT = tempData.T1 - tempData.T0;
  tempData.dT_dt = tempData.deltaT / (delayTime / 1000);

  Serial.print("Temperature = ");
  Serial.print(sensorData.T1);
  Serial.print(", Change = ");
  Serial.println(sensorData.deltaT);

}
