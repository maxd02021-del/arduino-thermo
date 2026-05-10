#include <math.h>
const int B = 4275000; // B value of the thermistor
const int R0 = 100000; // R0 = 100k
const int pinTempSensor = A0; // Grove - Temperature Sensor connect to A0

struct tempInfo {
  float T0;
  float T1;
  float deltaT;
} tempData;


float delayTime = 100;
int N = 0;
int idleCount = 0;
float sample[32];
float magnitude[32] = {0};
float f_k = 0;
float maxMag = 0;
int dominantK = 0;



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
  maxMag = 0;
  dominantK = 0;
  float f_s = 1000 / delayTime;
  float real[32] = {0};
  float imag[32] = {0};


  for (int k = 0; k < 32; ++k){
    for (int n = 0; n < 32; ++n){
      real[k] += sample[n] * cos(2 * PI * k * n / 32);
      imag[k] += -sample[n] * sin(2 * PI * k * n / 32);
    }
  }

  for (int k = 0; k < 32; ++k){
    if (k == 0) continue;
    magnitude[k] = sqrt(real[k] * real[k] + imag[k] * imag[k]);
    if (magnitude[k] > maxMag){
      maxMag = magnitude[k];
      dominantK = k;
    }
  }
  f_k = (dominantK * f_s) / 32;
}


 // Power mode and delay control
void decide_power_mode()
{
  if (f_k >= 0.5){
    delayTime = 500;
    Serial.print("ACTIVE MODE");
    idleCount = 0;
  } else if (f_k >= 0.1 && f_k < 0.5 && idleCount < 5){
    delayTime = 2000;
    Serial.print("IDLE MODE");
    ++idleCount;
  } else if (f_k < 0.1 || idleCount == 5){
    delayTime = 10000;
    Serial.print("POWER_DOWN MODE");
    idleCount = 0;
  }
}


void send_data_to_pc()
{
  float time = millis() / 1000.0;

  Serial.print(", Time = ");
  Serial.print(time);
  Serial.print(", Temperature = ");
  Serial.print(tempData.T1);
  Serial.print(", Frequency = ");
  Serial.print(f_k);
  Serial.print(", Magnitude = ");
  Serial.println(magnitude[dominantK]);

}


void loop()
{
  tempData.T0 = tempData.T1;
  delay((unsigned long)delayTime);

  int a = analogRead(pinTempSensor);
  float R = 1023.0/a-1.0;
  R = R0*R;
  tempData.T1 = 1.0/(log(R/R0)/B+1/298.15)-273.15; // convert to temperature via datasheet
  sample[N] = tempData.T1;
  ++N;

  tempData.deltaT = tempData.T1 - tempData.T0;

  if (N == 32) {
    apply_dft();
    N = 0;
  }

  decide_power_mode();
  send_data_to_pc();

}
