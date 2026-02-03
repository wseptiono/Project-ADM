#define IR_PIN 2
#define LED_PIN 6
#define LED_INT LED_BUILTIN   // LED L Arduino (pin 13)

bool ledState = LOW;
unsigned long previousMillis = 0;

const unsigned long onTime  = 300; // LED nyala 300  milidetik
const unsigned long offTime = 100;  // LED mati 100  milidetik

void setup() {
  pinMode(IR_PIN, INPUT);        // pakai pull-up eksternal 10k ke 5V
  pinMode(LED_PIN, OUTPUT);
  pinMode(LED_INT, OUTPUT);

  digitalWrite(LED_PIN, LOW);
  digitalWrite(LED_INT, LOW);
}

void loop() {
  int irState = digitalRead(IR_PIN);
  unsigned long now = millis();

  // =================================
  // ADA OBJEK → IR = LOW (E3Z-D62)
  // =================================
  if (irState == LOW) {

    if (ledState == HIGH) {
      // LED sedang ON
      if (now - previousMillis >= onTime) {
        previousMillis = now;
        ledState = LOW;
        digitalWrite(LED_PIN, LOW);
        digitalWrite(LED_INT, LOW);
      }
    } else {
      // LED sedang OFF
      if (now - previousMillis >= offTime) {
        previousMillis = now;
        ledState = HIGH;
        digitalWrite(LED_PIN, HIGH);
        digitalWrite(LED_INT, HIGH);
      }
    }

  }
  // =================================
  // TIDAK ADA OBJEK → IR = HIGH
  // =================================
  else {
    digitalWrite(LED_PIN, LOW);
    digitalWrite(LED_INT, LOW);
    ledState = LOW;
  }
}

