//IMU Reading Code File:
//Include in Motherboard_Main_Master
//All IMUs communicate via I2C to Master uC


void readBoardIMUState() {
  readI2C(boardIMU,0x34,12);
  boardAcc[0] = (float)((Wire.read()|(Wire.read()<<8)))/32768*16;
  boardAcc[1] = (float)((Wire.read()|(Wire.read()<<8)))/32768*16;
  boardAcc[2] = (float)((Wire.read()|(Wire.read()<<8)))/32768*16;

  boardVel[0] = (float)((Wire.read()|(Wire.read()<<8)))/32768*2000;
  boardVel[1] = (float)((Wire.read()|(Wire.read()<<8)))/32768*2000;
  boardVel[2] = (float)((Wire.read()|(Wire.read()<<8)))/32768*2000;

  readI2C(boardIMU,0x3d,6);

  boardTilt[0] = (float)((Wire.read()|(Wire.read()<<8)))/32768*180-180;
  boardTilt[1] = (float)((Wire.read()|(Wire.read()<<8)))/32768*180-180;
  boardTilt[2] = (float)((Wire.read()|(Wire.read()<<8)))/32768*180-180;
}

void readOffIMUState() {
  readI2C(offIMU,0x34,12);
  offAcc[0] = (float)((Wire.read()|(Wire.read()<<8)))/32768*16;
  offAcc[1] = (float)((Wire.read()|(Wire.read()<<8)))/32768*16;
  offAcc[2] = (float)((Wire.read()|(Wire.read()<<8)))/32768*16;

  offVel[0] = (float)((Wire.read()|(Wire.read()<<8)))/32768*2000;
  offVel[1] = (float)((Wire.read()|(Wire.read()<<8)))/32768*2000;
  offVel[2] = (float)((Wire.read()|(Wire.read()<<8)))/32768*2000;

  readI2C(offIMU,0x3d,6);

  offTilt[0] = (float)((Wire.read()|(Wire.read()<<8)))/32768*180-180;
  offTilt[1] = (float)((Wire.read()|(Wire.read()<<8)))/32768*180-180;
  offTilt[2] = (float)((Wire.read()|(Wire.read()<<8)))/32768*180-180;
}

void readGPSLocation() {
  readI2C(boardIMU,0x49,8);
  locationGPS[0] = (float)((Wire.read()|(Wire.read()<<8))|(Wire.read()<<16)|(Wire.read()<<24));
  locationGPS[1] = (float)((Wire.read()|(Wire.read()<<8))|(Wire.read()<<16)|(Wire.read()<<24));
}
