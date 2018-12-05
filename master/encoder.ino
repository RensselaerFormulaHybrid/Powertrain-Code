
void readEncoderNano() {
Wire.requestFrom(encoderNano,16); //Request 16 bytes (8 encoders, 2 bytes per)
sus[0] = ((Wire.read()|(Wire.read()<<8)));
sus[1] = ((Wire.read()|(Wire.read()<<8)));
sus[2] = ((Wire.read()|(Wire.read()<<8)));
sus[3] = ((Wire.read()|(Wire.read()<<8)));
sus[4] = ((Wire.read()|(Wire.read()<<8)));
acc = ((Wire.read()|(Wire.read()<<8)));
acc2 = ((Wire.read()|(Wire.read()<<8)));
brake = ((Wire.read()|(Wire.read()<<8)));

if (abs(acc2-acc) > (accHigh*accAllowedError)) {  //Check if both encoders agree, pull to 0 if not
  acc = 0;
}
else if ((acc > accHigh) & (acc < accHigh*(1+accAllowedError))) {  //Adjust for slight over/under on encoder read range limits due to temp/deflection/error/etc.
  acc = accHigh;
}
else if ((acc > accHigh*(1+accAllowedError))) {
  acc = accLow;
}
else if (acc < accLow) {
  acc = accLow;
}

if (brake > brakeLow*(1+brakeLightDeadband)) { //Actuate brake light
//  if (brakeLightOnFlag == 0) {  //Check if already in correct state, change if not
//    digitalWrite(brakeLight, HIGH);
//    brakeLightOnFlag = 1;
//  }
  digitalWrite(brakeLight, HIGH);
}
else {
  digitalWrite(brakeLight, LOW);
}

}
