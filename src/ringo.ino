const short LED_COUNT = 2;
const short LEDS[LED_COUNT] = {2, 3};

void setup() {
  for (short i = 0; i < LED_COUNT; i++) {
    pinMode(LEDS[i], OUTPUT);
  }
}

void loop() {
  blinkLoop(902.255638);
}

void blinkLoop(double rate) {
    int randNumber = random(0,2);
    digitalWrite(LEDS[randNumber], HIGH);
    delay((short) (rate / LED_COUNT - 50));
    digitalWrite(LEDS[randNumber], LOW);
    delay(50);
}
