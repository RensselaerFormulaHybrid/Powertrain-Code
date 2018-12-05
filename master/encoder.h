
#define accLow 0  //TODO
#define accHigh 0 //TODO
#define accAllowedError 0.05  //Allowed percentage over/under acc range without error
#define accAllowedVariance 0.05 //Allowed percent difference between acc encoders (% of full range)
#define brakeLow 0  //TODO
#define brakeHigh 0 //TODO
#define brakeAllowedError 0.05  //Allowed percentage over/under brake range without error
#define brakeLightDeadband 0.03 // Deadband range percentage before brake light is triggered
#define steerLow 0  //TODO
#define steerHigh 0 //TODO
#define steerMid (steerLow+steerHigh)/2;

unsigned int acc;
unsigned int acc2;
unsigned int brake;
int sus[5]; //[FL,FR,RL,RR,Steering]
bool brakeLightOnFlag = 0;

/*
void readEncoder(byte ssPin, int &storageVariable);

void readPedals();


void readSuspension();

void encoderTestRead();
*/
