const short LED_COUNT = 8;
const short LEDS[8] = {2, 3, 4, 5, 6, 7, 8, 9};

void setup() {
  for (short i = 0; i < LED_COUNT; i++) {
    pinMode(LEDS[i], OUTPUT);
  }
}

void loop() {
  blinkLoop(1500);
}

void blinkLoop(short rate) {
  for (short i = 0; i < LED_COUNT; i++) {
    digitalWrite(LEDS[i], HIGH);
    delay((short) (rate / LED_COUNT));
    digitalWrite(LEDS[i], LOW);
  }
}
