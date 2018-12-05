#define watchdogTimeoutDrive 500  //1 to 4095 ms
#define rearBias 63 //Percent rear drive power rearBias
#define packCurrentLimit 600  //Amperes, Hard limit on draw from pack only
#define l 1.687  //Wheelbase in meters
#define wFront 1.219  //Front Track Width in mm
#define wRear 1.168  //Rear Track width in mm
#define motorTempSoftLimit 75 //Deg. C, threshold before power scale-back
#define motorOverTempKTorque 0.028  //Percent per (Deg. C)^1.5 in torque command scaleback
#define motorTempHardLimit 90 //Deg. C, threshold before motor cutout


int motorTemp[5]; //[FL, FR, RL, RR, Gen]
unsigned int driveState[8]; //[FL, FR, RL, RR, Gen, Brake, Pack Current Now, Pack Current Then]
bool steerDirection = 0;  //0-left, 1-right
unsigned int rampValue = 0;

//Temp Sensor Reading:
void readMotorTemp();

void readCTransADC();

void setupDriveDAC();

void updateDriveDAC();

void resetDriveDACWatchdog();

void setDriveState();

void driveDACTestRamp();
