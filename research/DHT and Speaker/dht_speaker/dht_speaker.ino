#include "Grove_Temperature_And_Humidity_Sensor.h"

#define DHTTYPE DHT11   // DHT 11

#define DHTPIN 2     // what pin we're connected to（DHT10 and DHT20 don't need define it）
DHT dht(DHTPIN, DHTTYPE);   //   DHT11 DHT21 DHT22

#if defined(ARDUINO_ARCH_AVR)
    #define debug  Serial

#elif defined(ARDUINO_ARCH_SAMD) ||  defined(ARDUINO_ARCH_SAM)
    #define debug  SerialUSB
#else
    #define debug  Serial
#endif

// Speaker
#define SPEAKER 3
// Sonnerie Jurrasic Park
// Fréquences des notes (en microsecondes pour delayMicroseconds)
int noteD4 = 2909; // Ré
int noteE4 = 2591; // Mi
int noteF4 = 2441; // Fa
int noteG4 = 2178; // Sol
int noteA4 = 1940; // La

void setup() {
    Serial.begin(115200);
    Serial.println("DHTxx test!");

    Wire.begin();

    dht.begin();
}

void loop() {
    float temp_hum_val[2] = {0};

    if (!dht.readTempAndHumidity(temp_hum_val)) {
        Serial.print("Humidity: ");
        Serial.print(temp_hum_val[0]);
        Serial.print(" %\t");
        Serial.print("Temperature: ");
        Serial.print(temp_hum_val[1]);
        Serial.println(" *C");
        if(temp_hum_val[0] > 50){
            Serial.print("Humidité trop faible : ");
            Serial.println(temp_hum_val[0]);
            playNote(noteD4, 500);
            playNote(noteE4, 500);
            playNote(noteF4, 500);
            playNote(noteG4, 1000);
            playNote(noteA4, 1000);

            delay(2000); // Pause entre les répétitions de la mélodie
        } else {
            Serial.print("Humidité ok : ");
            Serial.println(temp_hum_val[0]);
        }
    } else {
        Serial.println("Failed to get temperature and humidity value.");
    }

    delay(1500);
}

void playNote(int note, int duration) {
  long endTime = millis() + duration;
  while (millis() < endTime) {
    digitalWrite(SPEAKER, HIGH);
    delayMicroseconds(note);
    digitalWrite(SPEAKER, LOW);
    delayMicroseconds(note);
  }
}