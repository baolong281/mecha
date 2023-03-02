#include <Arduino.h>
#include <stdio.h>
#include <Servo.h>

const int OUT_PIN = 9;
//microphone pinout

const int SAMPLE_TIME = 30;
unsigned long millisCurrent;
unsigned long millisLast = 0;
unsigned long millisElapsed = 0;
Servo motor;
//variables to get sample

const int led = 8;
//led pin

//logic state variables to turn lights on
bool clapDetected = false;
unsigned long lastClap = 0;
bool lightOn = false;
int lastSample = 0;

int sampleBufferValue = 0;
int motorState = 0;


void moveMotor() {
    motor.attach(6);
	if(motorState ==0) {
    motorState = 1;
    motor.write(0);
    return;
  } 
  motorState=0;
  motor.write(270);
}


/*
void onLed() {
  if(lightOn) {
    digitalWrite(led, LOW);
    lightOn = false;
    return;
  }
  digitalWrite(led, HIGH);
  lightOn=true;
}
*/

void setup() {
  pinMode(6, OUTPUT);
  pinMode(led, OUTPUT);
  Serial.begin(9600);
}

bool detectClap() {
  bool result = lastSample < 800 && sampleBufferValue < 800 && lastSample - sampleBufferValue <= 0 && sampleBufferValue > 90;
  lastSample = sampleBufferValue;
  return result;
}

void updateMillis() {
    //Serial.println(sampleBufferValue);
    sampleBufferValue = 0;
    millisLast = millisCurrent;
}


void loop() {
  // put your main code here, to run repeatedly:

  millisCurrent = millis();
  if(millisCurrent < 0) {
    Serial.println(millisCurrent);
  }
  millisElapsed = millisCurrent - millisLast;

  if(digitalRead(OUT_PIN) == LOW){
    sampleBufferValue++;
  }

  if(millisElapsed > SAMPLE_TIME){
    if(detectClap()) {
      Serial.print("Clap seen. Clap detected: ");
      Serial.println(clapDetected);
      if(clapDetected) {
          Serial.println("Light on");
          moveMotor();
          delay(1800);
                    motor.detach();
          Serial.println(millisCurrent-lastClap);
          clapDetected = false;
      } else {
        lastClap = millisCurrent;
        clapDetected = true;
      }
    } else if (millisCurrent - lastClap >= 600) {
      lastClap = millisCurrent;
      clapDetected=false;
    }

    updateMillis();

  }

}


