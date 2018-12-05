
//Temp Sensor Reading:
void readMotorTemp() {
  readI2C(motorTempSensorFL, 0x00, 1);
  motorTemp[0] = Wire.read();
  readI2C(motorTempSensorFR, 0x00, 1);
  motorTemp[1] = Wire.read();
  readI2C(motorTempSensorRL, 0x00, 1);
  motorTemp[2] = Wire.read();
  readI2C(motorTempSensorRR, 0x00, 1);
  motorTemp[3] = Wire.read();

  for (int i=0; i<4; i++) {
    if (motorTemp[i] & 0x80 != 0) {
      motorTemp[i] = -((~ motorTemp[i]) & 0xFF);
    }
  }
}

void readCTransADC() {
  driveState[6] = driveState[5];  //Store last read
  Wire.requestFrom(cTransADC, 2);
  while(Wire.available() <2);
  driveState[5]=((Wire.read()<<6) | (Wire.read()>>2));

  //Convert to Volts: Vread=((driveState[5]/1023)*((3.3-1.5*(3.3/1024))-(.5*(3.3/1024))))+(.5*(3.3/1024))
  //Convert to Pack Current: Current=(Vread/4)*1000
}

void setupDriveDAC() {
  Wire.beginTransmission(driveDAC);
  Wire.write(0x50); //Enter Config settings
  Wire.write(0xFF); //Apply to all DAC channels
  Wire.write(0xB0); //Watchdog - CLR setting , GATE disabled , Transparent DAC, CLEAR enabled

  Wire.write(0x60); //Enter DEFAULT settings
  Wire.write(0xFF); //Apply to all DAC channels
  Wire.write(0x20); //ZERO default

  Wire.write(0x34); //SW_Clear - Sets all CODE and DAC registers to default (0)
  Wire.write(0x96); //SW_Clear pt.2
  Wire.write(0x30); //SW_Clear pt.3

  Wire.write(0x10); //Enter watchdog settings
  Wire.write(watchdogTimeoutDrive>>4);  //timout high byte
  Wire.write(((watchdogTimeoutDrive<<4) & 0xFF) | 0x04); //timeout low byte, WD_Mask off, "High" watchdog safety level)
  Wire.endTransmission();

  Wire.beginTransmission(driveDAC);   //Set B7 as GndRef
  Wire.write(0x32); //Toggle Watchdog pt.1
  Wire.write(0x96); //Toggle Watchdog pt.2
  Wire.write(0x30); //Toggle Watchdog pt.3
  Wire.write(0xB7); //CODEn_LOADn DAC7 (GndRef)
  Wire.write(0);  //High bit
  Wire.write(0);  //Low bit
  Wire.endTransmission();
}

void updateDriveDAC() {
  Wire.beginTransmission(driveDAC);
  Wire.write(0x32); //Toggle Watchdog pt.1
  Wire.write(0x96); //Toggle Watchdog pt.2
  Wire.write(0x30); //Toggle Watchdog pt.3
  Wire.write(0xB0); //CODEn_LOADn DAC0 (FL)
  Wire.write(driveState[0]>>4);  //High bit
  Wire.write((driveState[0]<<4) & 0xFF);  //Low bit
  Wire.write(0xB1); //CODEn_LOADn DAC1 (FR)
  Wire.write(driveState[1]>>4);
  Wire.write((driveState[1]<<4) & 0xFF);
  Wire.write(0xB2); //CODEn_LOADn DAC2 (RL)
  Wire.write(driveState[2]>>4);
  Wire.write((driveState[2]<<4) & 0xFF);
  Wire.write(0xB4); //CODEn_LOADn DAC4 (RR)
  Wire.write(driveState[3]>>4);
  Wire.write((driveState[3]<<4) & 0xFF);
  Wire.write(0xB5); //CODEn_LOADn DAC5 (Gen)
  Wire.write(driveState[4]>>4);
  Wire.write((driveState[4]<<4) & 0xFF);
  Wire.write(0xB6); //CODEn_LOADn DAC6 (Brake)
  Wire.write(driveState[5]>>4);
  Wire.write((driveState[5]<<4) & 0xFF);
  Wire.endTransmission();
}

void driveDACTestRamp() {
  if ((rampValue+5) > 4095) {
    rampValue = 0;
  }
  else {
    rampValue = rampValue+50;
  }
  Wire.beginTransmission(driveDAC);
  Wire.write(0x32); //Toggle Watchdog pt.1
  Wire.write(0x96); //Toggle Watchdog pt.2
  Wire.write(0x30); //Toggle Watchdog pt.3
  Wire.write(0xB0); //CODEn_LOADn DAC0 (FL)
  Wire.write(rampValue>>4);  //High bit
  Wire.write((rampValue<<4) & 0xFF);  //Low bit
  Wire.write(0xB1); //CODEn_LOADn DAC1 (FR)
  Wire.write(rampValue>>4);  //High bit
  Wire.write((rampValue<<4) & 0xFF);  //Low bit
  Wire.write(0xB2); //CODEn_LOADn DAC2 (RL)
  Wire.write(rampValue>>4);  //High bit
  Wire.write((rampValue<<4) & 0xFF);  //Low bit

  Wire.write(0xB3); //CODEn_LOADn DAC3 (RR)
  Wire.write(rampValue>>4);  //High bit
  Wire.write((rampValue<<4) & 0xFF);  //Low bit

  Wire.write(0xB4); //CODEn_LOADn DAC4 (Gen)
  Wire.write(rampValue>>4);  //High bit
  Wire.write((rampValue<<4) & 0xFF);  //Low bit

  Wire.write(0xB5); //CODEn_LOADn DAC5 (Brake)
  Wire.write(rampValue>>4);  //High bit
  Wire.write((rampValue<<4) & 0xFF);  //Low bit
  Wire.write(0xB6); //CODEn_LOADn DAC5 (Brake)
  Wire.write(rampValue>>4);  //High bit
  Wire.write((rampValue<<4) & 0xFF);  //Low bit
  Wire.endTransmission();
}

void resetDriveDACWatchdog() {  //Reset driveDAC watchdog after allowed timeout
  Wire.beginTransmission(driveDAC);
  Wire.write(0x33);
  Wire.write(0x96);
  Wire.write(0x30);
  Wire.endTransmission();
}

void setDriveState() {
  //DAC output is Vref*(CODE/4095) , where Vref is 5V nom
  driveState[0] = 4095*((acc-accLow)/(accHigh-accLow));  //Map pedal input to DAC output range
  driveState[2] = driveState[0]*(1+(2*(rearBias-50)/100)); //Proportionally bias front/rear state to rearBias power distribution setting
  driveState[0] = driveState[0]*(1-(2*(rearBias-50)/100));

  //Torque vector for wheel turning radius variance based on steering wheel position:
  double thetaAvg = sus[0]*1; //FIXME
  if (thetaAvg < 0) {  //Adjust to left/right steer direction
    steerDirection = 0;
    thetaAvg = abs(thetaAvg);
  }
  else {
    steerDirection = 1;
  }
  float rRef = l/sin(atan(l/(0.5*wFront+l/(tan(thetaAvg))))); //reference radius of outside front tire
  if (steerDirection == 1) {  //If turning right
    driveState[1] = driveState[0] * l/(rRef+sin(atan(l/(l/tan(thetaAvg)-0.5*wFront))));
    //driveState[0] = driveState[0]*1;
    driveState[3] = driveState[2] * (l/tan(thetaAvg)-0.5*wRear)/rRef;
    driveState[2] = driveState[2] * (0.5*wRear+l/tan(thetaAvg))/rRef;
  }
  else {  //If turning left
    driveState[1] = driveState[0];
    driveState[0] = driveState[0] * l/(rRef+sin(atan(l/(l/tan(thetaAvg)-0.5*wFront))));
    driveState[3] = driveState[2] * (0.5*wRear+l/tan(thetaAvg))/rRef;
    driveState[2] = driveState[2] * (l/tan(thetaAvg)-0.5*wRear)/rRef;
  }

  //TRACTION CONTROL / WHEEL SLIP ADJUSTMENT HERE
  //Suspension position adjustment?

  //Scale back drive in response to motor overtemp:
  if ((motorTemp[0] >> motorTempSoftLimit) | (motorTemp[1] >> motorTempSoftLimit)) {  //Front motors
    if (motorTemp[0] >> motorTemp[1]) {
      driveState[0] = driveState[0]*(1-(sq(driveState[0])*motorOverTempKTorque*(motorTemp[0]-motorTempSoftLimit)));
      driveState[1] = driveState[1]*(1-(sq(driveState[0])*motorOverTempKTorque*(motorTemp[0]-motorTempSoftLimit)));
    }
    else if (motorTemp[1] >> motorTemp[0]) {
      driveState[0] = driveState[0]*(1-(sq(driveState[1])*motorOverTempKTorque*(motorTemp[1]-motorTempSoftLimit)));
      driveState[1] = driveState[1]*(1-(sq(driveState[1])*motorOverTempKTorque*(motorTemp[1]-motorTempSoftLimit)));
    }
  }

  if ((motorTemp[2] >> motorTempSoftLimit) | (motorTemp[3] >> motorTempSoftLimit)) {  //Rear Motors
    if (motorTemp[2] >> motorTemp[3]) {
      driveState[2] = driveState[2]*(1-(sq(driveState[2])*motorOverTempKTorque*(motorTemp[2]-motorTempSoftLimit)));
      driveState[3] = driveState[3]*(1-(sq(driveState[2])*motorOverTempKTorque*(motorTemp[2]-motorTempSoftLimit)));
    }
    else if (motorTemp[3] >> motorTemp[2]) {
      driveState[2] = driveState[2]*(1-(sq(driveState[3])*motorOverTempKTorque*(motorTemp[3]-motorTempSoftLimit)));
      driveState[3] = driveState[3]*(1-(sq(driveState[3])*motorOverTempKTorque*(motorTemp[3]-motorTempSoftLimit)));
    }
  }

  //Pack current adjustment
  //Convert to Volts: Vread=((driveState[5]/1023)*((3.3-1.5*(3.3/1024))-(.5*(3.3/1024))))+(.5*(3.3/1024))
  //Convert to Pack Current: Current=(Vread/4)*1000
  packCurrent=((((driveState[5]/1023)*((3.3-1.5*(3.3/1024))-(.5*(3.3/1024))))+(.5*(3.3/1024)))/4)*1000
  //*INSERT PACK OVERCURRENT PROTECTION LOGIC*

}
