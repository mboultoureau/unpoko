#include "TM1637.h"

//Include Grove-4 digit display Itbrary


// Branché sur le D2 => CLK = 2 et DIO = 3
// Branché sur le D4 => CLK = 4 et DIO = 5

// #define CLK 2
// #define DIO 3 

#define CLK 4
#define DIO 5

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