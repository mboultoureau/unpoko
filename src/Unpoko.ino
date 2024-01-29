#include <stdbool.h>
#include "TM1637.h"
#include "ChainableLED.h"

// Change if needed (master / slave)
#define IS_PRIMARY_DEVICE true

#define NUM_LEDS  5

ChainableLED leds(7, 8, NUM_LEDS);

int moisture_value = 0;

#define DIGIT_4_CLK 4
#define DIGIT_4_DIO 5

TM1637 tm1637(DIGIT_4_CLK, DIGIT_4_DIO);

void setup() {
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
  Serial.begin(9600);
  Serial.println("Unpoko 🤠🌵: Starting program");
  delay(1000);
  Serial.println("Slave Bluetooth Ready");
}

void setupMasterBluetooth() {
  pinMode(9, OUTPUT);
  digitalWrite(9, HIGH);

  Serial.begin(9600);
  Serial.println("Unpoko 🤠🌵: Starting program");
  Serial2.begin(9600);
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
    String receivedData = Serial3.readStringUntil('\n');
    Serial.println(receivedData);

    if (IS_PRIMARY_DEVICE) {
      if(receivedData.startsWith("moisture_level ")) {
        // Extracting moisture value from the received data
        int moistureValue = receivedData.substring(15).toInt();

        // Now you can handle the moisture value as needed
        // For example, you can display it on your tm1637 display
        int units = moistureValue % 10;
        int tens = (moistureValue / 10) % 10;
        int hundreds = (moistureValue / 100) % 10;
        int thousands = (moistureValue / 1000) % 10;

        tm1637.display(0, thousands); // Affiche '1' à la position 0
        tm1637.display(1, hundreds); // Affiche '2' à la position 1
        tm1637.display(2, tens); // Affiche '3' à la position 2
        tm1637.display(3, units); // Affiche '4' à la position 3

        Serial2.println("Moisture level " + String(moistureValue));
      } else if (receivedData.startsWith("disco ")) {
        Serial.println("Enable disco mode");
        Serial3.println("disco start");
      }
    } else {
      if (receivedData.startsWith("disco ")) {
        for (int j = 0; j < 256; j++) {
          for (int i = 0; i < NUM_LEDS; i++) {
            leds.setColorRGB(i, random(256), random(256), random(256));
          }
          delay(50);
        }
      }
    }
  }

  while (Serial2.available()) {
    String receivedData = Serial2.readStringUntil('\n');
    Serial.println(receivedData);

    if (IS_PRIMARY_DEVICE) {
      if(receivedData.startsWith("disco ")) {
        Serial.print("Enable disco mode");
        Serial3.println("disco start");
      }
    }
  }

  if (!IS_PRIMARY_DEVICE) {
    moisture_value = analogRead(A0);
    Serial3.print("moisture_level ");
    Serial3.println(moisture_value);
    delay(1000);
  }
}