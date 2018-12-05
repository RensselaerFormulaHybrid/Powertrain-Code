//IMU Reading Code File:
//Include in Motherboard_Main_Master
//All IMUs communicate via I2C to Master uC

//Definitions:
#define boardIMU 0x50
#define offIMU 0x52

//Variables:
float boardAcc[3];  //Acc in Gs - x,y,z
float boardTilt[3];  //Tilt in degrees - x,y,z
float boardVel[3];  //Angular Velocity in degrees/second - x,y,z

float offAcc[3];  //Acc in Gs - x,y,z
float offTilt[3];  //Tilt in degrees - x,y,z
float offVel[3];  //Angular Velocity in degrees/second - x,y,z

float locationGPS[2];  //GPS Location - Longitude,Latitude

extern int dataBuffer[];

void readBoardIMUState();

void readOffIMUState();

void readGPSLocation();
