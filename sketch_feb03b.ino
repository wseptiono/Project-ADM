#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

#define IR_PIN 2
#define LED_PIN 6
#define LED_INT LED_BUILTIN   // LED L Arduino (pin 13)

#define DF_RX 10   // Arduino TX -> DFPlayer RX (via R 1K)
#define DF_TX 11   // Arduino RX <- DFPlayer TX

SoftwareSerial dfSerial(DF_RX, DF_TX);
DFRobotDFPlayerMini dfPlayer;

// ===== LED timing =====
bool ledState = LOW;
unsigned long previousMillis = 0;
const unsigned long onTime  = 1000; // LED ON 1 detik
const unsigned long offTime = 250;  // LED OFF 0.25 detik

// ===== Audio state =====
bool isPlaying = false;
bool dfReady   = false;

void setup() {
  pinMode(IR_PIN, INPUT);        // E3Z-D62 pakai pull-up eksternal 10k ke 5V
  pinMode(LED_PIN, OUTPUT);
  pinMode(LED_INT, OUTPUT);

  digitalWrite(LED_PIN, LOW);
  digitalWrite(LED_INT, LOW);

  Serial.begin(9600);
  dfSerial.begin(9600);

  // ==== DFPLAYER SAFE INIT ====
  if (dfPlayer.begin(dfSerial)) {
    dfPlayer.volume(25);   // 0–30
    dfPlayer.stop();
    dfReady = true;
    Serial.println("DFPlayer READY");
  } else {
    Serial.println("DFPlayer NOT READY");
  }
}

void loop() {
  int irState = digitalRead(IR_PIN);
  unsigned long now = millis();

  // =================================
  // ADA OBJEK → IR = LOW
  // =================================
  if (irState == LOW) {

    // ---- LED BLINKING ----
    if (ledState == HIGH) {
      if (now - previousMillis >= onTime) {
        previousMillis = now;
        ledState = LOW;
        digitalWrite(LED_PIN, LOW);
        digitalWrite(LED_INT, LOW);
      }
    } else {
      if (now - previousMillis >= offTime) {
        previousMillis = now;
        ledState = HIGH;
        digitalWrite(LED_PIN, HIGH);
        digitalWrite(LED_INT, HIGH);
      }
    }

    // ---- AUDIO PLAY ----
    if (dfReady && !isPlaying) {
      dfPlayer.loop(1);   // mainkan 0001.mp3 berulang
      isPlaying = true;
      Serial.println("Audio PLAY");
    }

  }
  // =================================
  // TIDAK ADA OBJEK → IR = HIGH
  // =================================
  else {

    // LED MATI
    digitalWrite(LED_PIN, LOW);
    digitalWrite(LED_INT, LOW);
    ledState = LOW;

    // AUDIO STOP
    if (dfReady && isPlaying) {
      dfPlayer.stop();
      isPlaying = false;
      Serial.println("Audio STOP");
    }
  }
}
