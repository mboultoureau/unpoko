#include "TM1637.h"

//Include Grove-4 digit display Itbrary

#define CLK 2//pins definitions for TM1637 and can be changed to other ports
#define DIO 3 
// Branché sur le D2

TM1637 tm1637(CLK, DIO);

void setup(){
  tm1637.init();
  tm1637.set(BRIGHT_TYPICAL);//BRIGHT_TYPICAL2, BRIGHT DARKEST, BRIGHTEST-7:
}

void loop(){
  tm1637.display(0, 1); // Affiche '1' à la position 0
  tm1637.display(1, 2); // Affiche '2' à la position 1
  tm1637.display(2, 3); // Affiche '3' à la position 2
  tm1637.display(3, 4); // Affiche '4' à la position 3

  delay(300); // Un court délai entre les mises à jour (facultatif)
}