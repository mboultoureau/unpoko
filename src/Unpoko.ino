#include <stdbool.h>
#include "TM1637.h"

#define IS_PRIMARY_DEVICE true

int moisture_value = 0;

#define DIGIT_4_CLK 4
#define DIGIT_4_DIO 5

TM1637 tm1637(DIGIT_4_CLK, DIGIT_4_DIO);

// struct SecondaryDevice {
//   int deviceId;

//   bool has_soil_humidity;
//   bool has_room_humidity;
//   bool has_temperature;
//   bool has_co2;
//   bool has_luminosity;

//   float low_soil_humidity;
//   float high_soil_humidity;
//   float low_room_humidity;
//   float high_room_humidity;
//   float low_temperature;
//   float high_temperature;
//   float low_co2;
//   float high_co2;
//   float low_luminosity;
//   float high_luminosity;

//   float soil_humidity[32];
//   float room_humidity[32];
//   float temperature[32];
//   float co2[32];
//   float luminosity[32];
// };

// enum Error {
//   NONE = 0,
//   SOIL_HUMIDITY_TOO_HIGH = 1,
//   SOIL_HUMIDITY_TOO_LOW = 2,
//   ROOM_HUMIDITY_TOO_HIGH = 3,
//   ROOM_HUMIDITY_TOO_LOW = 4,
//   LUMINOSITY_TOO_HIGH = 5,
//   LUMINOSITY_TOO_LOW = 6,
//   TEMPERATURE_TOO_HIGH = 7,
//   TEMPERATURE_TOO_LOW = 8,
//   CO2_TOO_HIGH = 9,
//   CO2_TOO_LOW = 10,
//   MULTIPLE_ERRORS = 11
// };

bool isPrimaryDevice = IS_PRIMARY_DEVICE;

void setup() {
  Serial.begin(9600);
  Serial.println("Unpoko 🤠🌵: Starting program");

  if (IS_PRIMARY_DEVICE) {
    setupMasterBluetooth();
  } else {
    setupSlaveBluetooth();
  }

  tm1637.init();
  tm1637.set(BRIGHT_TYPICAL);//BRIGHT_TYPICAL2, BRIGHT DARKEST, BRIGHTEST-7:
}

bool sendATCommand(const char* command, const char* expectedResponse) {
  Serial3.println(command);
  delay(1000); // Wait for the response

  if (Serial3.find(const_cast<char*>(expectedResponse))) {
    return true; // Command successful
  } else {
    return false; // Command failed
  }
}

void setupSlaveBluetooth() {
  Serial3.begin(9600);  // Start Serial3 at the new baud rate
  delay(1000);
  Serial.println("Slave Bluetooth Ready");
}

void setupMasterBluetooth() {
  pinMode(9, OUTPUT);
  digitalWrite(9, HIGH);
  Serial3.begin(38400);

  if (!sendATCommand("AT", "OK")) {
    // Retry or handle the error as needed
    Serial.println("Failed to initialize Bluetooth module. Retrying...");
    delay(1000);
    setupMasterBluetooth();
    return;
  }

  if (!sendATCommand("AT+NAME=Unpoko", "OK")) {
    Serial.println("Failed to set device name. Exiting...");
    return;
  }

  if (!sendATCommand("AT+ROLE=1", "OK")) {
    Serial.println("Failed to set role to Master. Exiting...");
    return;
  }

  if (!sendATCommand("AT+INQM=1,9,48", "OK")) {
    Serial.println("Failed to put in discoverable mode. Exiting...");
    return;
  }

  if (!sendATCommand("AT+CMODE=0", "OK")) {
    Serial.println("Failed to set to fixed address. Exiting...");
    return;
  }

  if (!sendATCommand("AT+BIND=000e,ea,cf7469", "OK")) {
    Serial.println("Failed to connect to slave 1. Exiting...");
    return;
  }

  // Exit AT mode
  digitalWrite(9, LOW);

  if (!sendATCommand("AT+RESET", "OK")) {
    Serial.println("Failed to reset. Exiting...");
    return;
  }

  if(sendATCommand("AT", "OK")) {
    Serial.println("Failed to exit AT mode");
    return;
  }

  // Change baud rate for Serial3
  Serial3.end();  // End the communication at the current baud rate
  Serial3.begin(9600);  // Start Serial3 at the new baud rate
  

  Serial.println("Master bluetooth ready");
}

void loop() {
  if (Serial.available()) {
    char receivedByte = Serial.read();
    Serial.write(receivedByte);
    Serial3.write(receivedByte);
  }

  while (Serial3.available()) {
    char receivedByte = Serial3.read();
    // Serial.write(receivedByte);
    // Serial3.write(receivedByte);

    if (IS_PRIMARY_DEVICE) {
      int units = receivedByte % 10;
      int tens = (receivedByte / 10) % 10;
      int hundreds = (receivedByte / 100) % 10;
      int thousands = (receivedByte / 1000) % 10;

      tm1637.display(0, thousands); // Affiche '1' à la position 0
      tm1637.display(1, hundreds); // Affiche '2' à la position 1
      tm1637.display(2, tens); // Affiche '3' à la position 2
      tm1637.display(3, units); // Affiche '4' à la position 3
    }
  }

  if (!IS_PRIMARY_DEVICE) {
    moisture_value = analogRead(A0);
    Serial.println(moisture_value);
    Serial3.write(moisture_value);
    delay(1000);
  }

  // tm1637.display(0, 1); // Affiche '1' à la position 0
  // tm1637.display(1, 2); // Affiche '2' à la position 1
  // tm1637.display(2, 3); // Affiche '3' à la position 2
  // tm1637.display(3, 4); // Affiche '4' à la position 3
}