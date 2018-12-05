//Pin Assigns:
void setPinModes() {
  pinMode(rotaryOne, INPUT);
  pinMode(rotaryTwo, INPUT);
  pinMode(rotaryThree, INPUT);
  pinMode(drs, INPUT);
  pinMode(ssBrake, OUTPUT);
  pinMode(ssAcc, OUTPUT);
  pinMode(ssSusFL, OUTPUT);
  pinMode(ssSusFR, OUTPUT);
  pinMode(ssSusRL, OUTPUT);
  pinMode(ssSusRR, OUTPUT);
  pinMode(ssSteer, OUTPUT);
  pinMode(driveEnable, OUTPUT);
}

void setInitialStates() { //Set pin states at initial power-on
  digitalWrite(ssBrake, HIGH);
  digitalWrite(ssAcc, HIGH);
  digitalWrite(ssSusFL, HIGH);
  digitalWrite(ssSusFR, HIGH);
  digitalWrite(ssSusRL, HIGH);
  digitalWrite(ssSusRR, HIGH);
  digitalWrite(ssSteer, HIGH);
  digitalWrite(driveEnable, LOW);
}
