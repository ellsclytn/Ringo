const short LED_COUNT = 2;
const short LEDS[LED_COUNT] = {2, 3};

void setup() {
  for (short i = 0; i < LED_COUNT; i++) {
    pinMode(LEDS[i], OUTPUT);
  }
}

void loop() {
  blinkLoop(750);
}

void blinkLoop(short rate) {
  for (short i = 0; i < LED_COUNT; i++) {
    digitalWrite(LEDS[i], HIGH);
    delay((short) (rate / LED_COUNT));
    digitalWrite(LEDS[i], LOW);
  }
}
