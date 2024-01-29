#define SPEAKER 3
// Sonnerie Jurrasic Park
// Fréquences des notes (en microsecondes pour delayMicroseconds)
int noteD4 = 2909; // Ré
int noteE4 = 2591; // Mi
int noteF4 = 2441; // Fa
int noteG4 = 2178; // Sol
int noteA4 = 1940; // La

void setup() {
  pinMode(SPEAKER, OUTPUT);
}

void loop() {
  playNote(noteD4, 500);
  playNote(noteE4, 500);
  playNote(noteF4, 500);
  playNote(noteG4, 1000);
  playNote(noteA4, 1000);

  delay(2000); // Pause entre les répétitions de la mélodie
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
