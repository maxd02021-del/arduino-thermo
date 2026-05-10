#include <math.h>
const int B = 4275000; // B value of the thermistor
const int R0 = 100000; // R0 = 100k
const int pinTempSensor = A0; // Grove - Temperature Sensor connect to A0

struct tempInfo {
  float T0;
  float T1;
  float deltaT;
  float dT_dt;
  float abs_dT;
} tempData;


float delayTime = 100;
int N = 0;


void setup()
{
  Serial.begin(9600);

  int a = analogRead(pinTempSensor);
  float R = 1023.0/a-1.0;
  R = R0*R;
  tempData.T1 = tempData.T0 = 1.0/(log(R/R0)/B+1/298.15)-273.15; // convert to temperature via datasheet
}

void apply_dft()
{



}

void loop()
{
  tempData.T0 = tempData.T1;
  delay((unsigned long)delayTime);

  int a = analogRead(pinTempSensor);
  float R = 1023.0/a-1.0;
  R = R0*R;
  tempData.T1 = 1.0/(log(R/R0)/B+1/298.15)-273.15; // convert to temperature via datasheet
  ++N;

  tempData.deltaT = tempData.T1 - tempData.T0;
  tempData.dT_dt = tempData.deltaT / (delayTime / 1000);
  tempData.abs_dT = abs(tempData.dT_dt)

  if (N = 32) {
    apply_dft()
    N = 0
  }

  // Power mode and delay control
  if (tempData.dT_dt >= 0.5){
    delayTime = 100;
    Serial.print("ACTIVE MODE");
  }
  else if (tempData.dT_dt >= 0.1 && tempData.dT_dt < 0.5) {
    delayTime = 1000;
    Serial.print("IDLE MODE");
  }
  else if (tempData.dT_dt < 0.1){
    delayTime = 10000;
    Serial.print("LOW POWER MODE");
  }

  Serial.print(", Temperature = ");
  Serial.print(tempData.T1);
  Serial.print(", Change = ");
  Serial.println(tempData.deltaT);

}
