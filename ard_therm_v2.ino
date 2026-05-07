#include <math.h>
const int B = 4275000; // B value of the thermistor
const int R0 = 100000; // R0 = 100k
const int pinTempSensor = A0; // Grove - Temperature Sensor connect to A0

struct sensorBox {
  float T0;
  float T1;
  float deltaT;
} tempData;


void setup()
{
  Serial.begin(9600);
}

void loop()
{
  int a = analogRead(pinTempSensor);
  float R = 1023.0/a-1.0;
  R = R0*R;
  float temperature = 1.0/(log(R/R0)/B+1/298.15)-273.15; // convert to temperature via datasheet
  delay(100)

  a = analogRead(pinTempSensor);
  R = 1023.0/a-1.0;
  R = R0*R;
  sensorData.T1 = 1.0/(log(R/R0)/B+1/298.15)-273.15; 
  sensorData.deltaT = sensorData.T1 - sensorData.T0; 
  
  Serial.print("temperature = ");
  Serial.println(temperature);

}
