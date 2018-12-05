void readEncoder2(byte ssPin, unsigned int &storageVariable, unsigned int address) {//ssPin: 10 on arduino uno
  SPI.beginTransaction(SPISettings(10000000, MSBFIRST,SPI_MODE1));                //storageVariable : store the value from encoder, pass by reference
  digitalWrite(ssPin, LOW);                                                       //address: address to get the reading from. Refer to the table at the beginning
  byte add_l = address & 0x00FF;
  byte add_h = (unsigned int)(address & 0x3F00) >> 8;

  if(parity(address | (0x40 << 8)) == 1){ //calculate parity
    add_h = add_h | 0x80;
  }
  add_h = add_h | 0x40; //indicate we are reading value

  add_h = SPI.transfer(add_h);
  add_l = SPI.transfer(add_l);

  digitalWrite(ssPin,HIGH);
  SPI.endTransaction();

  delay(2);

  SPI.beginTransaction(SPISettings(10000000, MSBFIRST,SPI_MODE1));
  digitalWrite(ssPin, LOW);

  add_h = (SPI.transfer(0x00));
  add_l = (SPI.transfer(0x00));

  SerialUSB.println(add_h);
  SerialUSB.println(add_l);

  digitalWrite(ssPin, HIGH);

  SPI.endTransaction();

  storageVariable = ((add_h << 8) | add_l); //combine
}

int parity(unsigned int x){ //parity function, calculate the parity (odd/even) of the address
  int parity = 0;
  while(x > 0){
    parity = parity + (x & 0x0001);
    x = x >> 1;
  }
  parity = parity % 2;
  return parity;
}

void encoderAccReadTest() {
  SerialUSB.println("In Test");
  readEncoder2(ssAcc,acc,0x3FFF);
  SerialUSB.println("Finished Read");
  if ((acc & 0x4000) != 0) {  //Check command frame error flag, print out "RIP" if error
    SerialUSB.println("RIP  ");
    acc = (acc & 0x3FFF);
  }else{
    acc = (acc & 0x3FFF); //Remove parity bit and error flag
  }
  SerialUSB.println(acc);
}

/*
void loop() {
  if(flag == 0){                  //print angle value from encoder
    readEncoder(10,value,0x3FFF);
    if ((value & 0x4000) != 0) {  //Check command frame error flag, print out "RIP" if error
      Serial.print("RIP  ");
      value = (value & 0x3FFF);
    }else{
      value = (value & 0x3FFF); //Remove parity bit and error flag
    }
    printBin(value);
  }else if(flag == 1){          //print magnetic field strngth from encoder
    readEncoder(10,value,0x3FFC);
    if((value & 0x4000) != 0){  //Check command frame error flag, print out "RIP" if error
      Serial.print("RIP  ");
    }
    value = (value & 0x0FFF);   //first 4 bit from this address is garbage.
    if(((value & 0x0800) == 0x0800) && ((value & 0x0400) == 0x0400)){ //bit 11 indicate magnetic field too low, bit 10 indicate magnetic firld too high
      Serial.print("(╯°□°）╯︵ ┻━┻   "); //if both too low and too high, which make no sense.... (╯°□°）╯︵ ┻━┻
    }else if((value & 0x0800) == 0x0800){
      Serial.print("-_-                     "); //if too low
    }else if((value & 0x0400) == 0x0400){
      Serial.print("+_+                     "); //if too high
    }else{
      Serial.print(" (•◡•)                 "); //right amount.
    }
    if((value & 0x0200) == 0x0200){ //bit 9 indicate if the reading is not reliable
      Serial.print("MEGA RIP");    //print MEGA RIP if happens
    }
    printBin(value);
  }
  delay(200); //5 Hz sample rate
}
*/
