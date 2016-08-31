const short LED_COUNT = 2;
const short LEDS[LED_COUNT] = {2, 3};

void setup() {
  for (short i = 0; i < LED_COUNT; i++) {
    pinMode(LEDS[i], OUTPUT);
  }
}

void loop() {
  delay(random(0, 300) * 1000);
  blinkLoop(902.255638);
}

void blinkLoop(double rate) {
  long onPeriod = random(5, 45) * 1000 + millis();
  while (millis() < onPeriod) {
    int randNumber = random(0,2);
    digitalWrite(LEDS[randNumber], HIGH);
    delay((short) (rate / LED_COUNT - 50));
    digitalWrite(LEDS[randNumber], LOW);
    delay(50);
  }
}
