//Pin Assigns:
#define rotaryOne d[0]
#define rotaryTwo d[1]
#define rotaryThree d[32]
#define wingLeft d[2]
#define wingRight d[3]
#define brakeLight d[5]
#define readyToDriveAlarm d[6]
#define propRightUp d[7]
#define propRightDown d[8]
#define ssBrake d[9]
#define ssAcc d[15]
#define ssAcc2 d[16]
#define drs d[17]
#define ssSusFL d[21]
#define ssSusFR d[22]
#define ssSusRL d[23]
#define ssSusRR d[26]
#define ssSteer d[27]
#define driveEnable d[33]
#define statusLED d[4]

//I2C Addresses:
#define boardIMU 0x50
#define motorTempSensorFL 0x48  //Model 0
#define motorTempSensorFR 0x49  //Model 1
#define motorTempSensorRL 0x4A  //Model 2
#define motorTempSensorRR 0x4E  //Model 6
#define genMotorTempSensor 0x4B //Model 3
#define cTransADC 0x4D //Pack Current Transducer ADC
#define driveDAC 0x10
#define accessoryDAC 0x1F
#define encoderNano 0x08  //Encoder bodge Nano slave



void setPinModes();

void setInitialStates();
