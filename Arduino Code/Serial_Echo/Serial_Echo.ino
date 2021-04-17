#include <Servo.h>
Servo Parallax;


// set signal pin to Pin no.2
volatile float angle;
int theta = 0, thetaP = 0;
volatile unsigned long tHigh = 0, tLow = 0;
volatile unsigned long rise = 0, fall = 0;
volatile float dc;
volatile int tCycle;
int turns = 0;
int Kp = 1;
int unitsFC = 360;
float dcMin = 0.029;
float dcMax = 0.971;
float dutyScale = 1.0;
float q2min = unitsFC / 4.0;
float q3max = q2min * 3.0;


void setup() {
  Parallax.attach(3);
  attachInterrupt(0, feedback360, CHANGE);
  Serial.begin(9600);
}

void loop() {
  char c;
  while(!Serial.available()){}
  if (Serial.available())
  {
    c = Serial.read();
  }
  //int i;
  //int n=35;
  //for (i=0; i<n ;i++){
    Parallax.write(100);
    delay(100);
    Parallax.write(90);
    delay(500);
    Serial.println(angle);
  //}
  
}

void feedback360() {
  if(digitalRead(2)) {
    // rise
    rise = micros();
    tLow = rise - fall;

    // Calcuate duty
    tCycle = tHigh + tLow;
    if((tCycle < 1000) && (tCycle > 1200))
      return;
    
    dc = (dutyScale * tHigh) / (float)tCycle;
    theta = ((dc - dcMin) * unitsFC) / (dcMax - dcMin);
  
    if(theta < 0)
      theta = 0;
    else if(theta > (unitsFC - 1))
      theta = unitsFC - 1;
  
    if((theta < q2min) && (thetaP > q3max))
      turns++;
    else if((thetaP < q2min) && (theta > q3max))
      turns--;
  
    if(turns >= 0)
      angle = (turns * unitsFC) + theta;
    else if(turns < 0)
      angle = ((turns + 1) * unitsFC) - (unitsFC - theta);
  
    thetaP = theta;
  
  } else {
    // fall
    fall = micros();
    tHigh = fall - rise;
  }
}
