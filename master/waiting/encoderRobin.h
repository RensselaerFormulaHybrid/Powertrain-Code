#define flag 0 //0 for angle reading print, 1 for magnetic field strength print

//distance: <= 0.1 inches, ideal distance between the chip and the magnet

/*
   Address: Name:     DEFAULT:  Description:
   0x0000   NOP       0x0000    No operation
   0x0001   ERRFL     0x0000    Error register
   0x0003   PROG      0x0000    Programming register
   0x3FFC   DIAAGC    0x0180    Diagnostic and AGC
   0x3FFD   MAG       0x0000    CORDIC magnitude
   0x3FFE   ANGLEUNC  0x0000    Without dynamic angle error compensation
   0x3FFF   ANGLECOM  0x0000    With dynamic angle error compensation
 */

unsigned int value;
int myNum[16];

void readEncoder2(byte ssPin, unsigned int &storageVariable, unsigned int address);

int parity(unsigned int x);

void encoderAccReadTest();
