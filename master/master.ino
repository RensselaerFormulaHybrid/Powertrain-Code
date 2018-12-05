#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <Adafruit_SleepyDog.h>
#include "pinDef.h"
#include "encoder.h"
//#include "IMU.h"
#include "motorControl.h"
//#include "sdCard.h"

int driveMode = 0;


void setup() {
  SerialUSB.begin(9600);
  setPinModes();
  setInitialStates();
  SPI.begin();
  Wire.begin();
  setupDriveDAC();
  readyToDriveCheck();

  resetDriveDACWatchdog(); //Watchdogs will probably time out while waiting for RTD
//int countdownMS = Watchdog.enable(5000); //Enable uC watchdog with 500ms

}

void loop() {
  Watchdog.reset();

  modeCheck();  //Check drive mode dial settings
  pollCriticalTelemetry();
  updateDriveDAC();


}


void readI2C(byte deviceAddress, byte startRegister, byte numBytesToRead) {
  Wire.beginTransmission(deviceAddress);
  Wire.write(startRegister);
  Wire.endTransmission(false);
  Wire.requestFrom(deviceAddress, numBytesToRead);
  while(Wire.available() < numBytesToRead);
}


void modeCheck() {
  //0 - off
  //1 - Autocross (rotaryOne)
  //2 - Acceleration (rotaryTwo)
  //3 - Endurance (rotaryThree)
  if (digitalRead(rotaryOne) == 1) {
    driveMode=1;
    if (digitalRead(driveEnable) == 0) {
      digitalWrite(driveEnable,HIGH);
    }
  }
  else if (digitalRead(rotaryTwo) == 1) {
    driveMode=2;
    if (digitalRead(driveEnable) == 0) {
      digitalWrite(driveEnable,HIGH);
    }
  }
  else if (digitalRead(rotaryThree) == 1) {
    driveMode=3;
    if (digitalRead(driveEnable) == 0) {
      digitalWrite(driveEnable,HIGH);
    }
  }
  else {
    driveMode=0;
    if (digitalRead(driveEnable) == 1) {
      digitalWrite(driveEnable,LOW);
    }
  }
}

void readyToDriveCheck() {
    modeCheck();  //Check initial state
  while (driveMode !=0) { //If not started in "Off" position, wait until off is observed
    delay(10);
    modeCheck();
  }
  while (driveMode == 0) {
    delay(10);
    modeCheck();
  } //Wait until a drive mode is enabled, triggering RTD
  digitalWrite(driveEnable, HIGH);
  digitalWrite(readyToDriveAlarm, HIGH);
  delay(1500);  //Must sound for 1-3 seconds per rules
  digitalWrite(readyToDriveAlarm, LOW);
  resetDriveDACWatchdog();  //DAC watchdog may have timed out by now
  //resetaccessoryDACWatchdog();  //DAC watchdog may have timed out by now
}


void pollCriticalTelemetry() {
  //readSuspension();  //FOR USE WITH ENCODERS DIRECTLY WIRED TO BOARD, PUT BACK IN WHEN THIS IS FIXED
  readBoardIMUState();
  readMotorTemp();
  readCTransADC();
  //readPedals(); //FOR USE WITH ENCODERS DIRECTLY WIRED TO BOARD, PUT BACK IN WHEN THIS IS FIXED
  readEncoderNano();  //TEMPORARY ARDUINO NANO INTERFACE READ
}
