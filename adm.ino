#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

#define IR_PIN 2
#define LED_PIN 6

#define DF_RX 10   // Arduino TX → DFPlayer RX (via R 1K)
#define DF_TX 11   // Arduino RX ← DFPlayer TX

SoftwareSerial dfSerial(DF_RX, DF_TX);
DFRobotDFPlayerMini dfPlayer;

bool isPlaying = false;
bool ledState = false;

unsigned long previousMillis = 0;
const unsigned long blinkInterval = 166; // ms (3x blink per detik)

void setup() {
  pinMode(IR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);

  digitalWrite(LED_PIN, LOW);

  Serial.begin(9600);
  dfSerial.begin(9600);

  if (!dfPlayer.begin(dfSerial)) {
    Serial.println("DFPlayer tidak terdeteksi");
    while (true);
  }

  dfPlayer.volume(25); // 0–30
  dfPlayer.stop();

  Serial.println("Sistem siap");
}

void loop() {
  int irState = digitalRead(IR_PIN);
  unsigned long currentMillis = millis();

  // === OBJEK TERDETEKSI ===
  // Asumsi: LOW = ada objek
  if (irState == LOW) {

    // Mulai audio looping (hanya sekali)
    if (!isPlaying) {
      dfPlayer.loop(1);   // loop 0001.mp3
      isPlaying = true;
      Serial.println("Objek terdeteksi - Audio LOOP");
    }

    // LED berkedip 3x per detik
    if (currentMillis - previousMillis >= blinkInterval) {
      previousMillis = currentMillis;
      ledState = !ledState;
      digitalWrite(LED_PIN, ledState);
    }

  }
  // === OBJEK HILANG ===
  else {
    if (isPlaying) {
      dfPlayer.stop();
      isPlaying = false;
      Serial.println("Objek hilang - Audio STOP");
    }

    // LED mati total
    digitalWrite(LED_PIN, LOW);
    ledState = false;
  }
}
