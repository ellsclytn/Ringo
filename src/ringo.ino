#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "env.h"

WiFiClient espClient;
PubSubClient client(espClient);

const short BLINK_DELAY    = 500;
const byte LED_COUNT       = 2;
const byte LEDS[LED_COUNT] = {12, 13};
const byte BUZZER_PIN      = 5;

long ledSwitchTime = 0;
bool prOpen        = false;
bool led1On        = false;
bool beeped        = false;

void setupWifi() {
  Serial.print("Connecting to: ");
  Serial.println(SSID);

  WiFi.begin(SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.print(WiFi.localIP());
  Serial.print(" with MAC: ");
  Serial.println(WiFi.macAddress());
}

void handleMsg(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived: ");
  Serial.println(topic);

  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }

  Serial.println();

  if ((char)payload[0] == '1') {
    prOpen = true;
    beeped = false;
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection... ");

    if (client.connect(WiFi.macAddress().c_str(), USER, MQ_PASS)) {
      Serial.println("connected.");
      client.subscribe("pr");
    } else {
      Serial.print("failed, rc: ");
      Serial.print(client.state());
      Serial.println(" will retry in 5 seconds.");

      delay(5000);
    }
  }
}

void dismissMessage() {
  digitalWrite(LEDS[0], LOW);
  digitalWrite(LEDS[1], LOW);

  tone(BUZZER_PIN, 1320, 100);
  delay(200);
  tone(BUZZER_PIN, 880, 100);

  prOpen = false;
  beeped = false;
}

void alertPr() {
  if (!beeped) {
    tone(BUZZER_PIN, 880, 100);
    delay(200);
    tone(BUZZER_PIN, 1320, 100);
    beeped = true;
  }

  if (millis() >= ledSwitchTime) {
    ledSwitchTime = millis() + BLINK_DELAY;
    led1On = !led1On;

    if (led1On) {
      digitalWrite(LEDS[0], HIGH);
      digitalWrite(LEDS[1], LOW);
    } else {
      digitalWrite(LEDS[1], HIGH);
      digitalWrite(LEDS[0], LOW);
    }
  }
}

void setup() {
  Serial.begin(115200);
  delay(3000);

  for (byte i = 0; i < LED_COUNT; i++) {
    pinMode(LEDS[i], OUTPUT);
  }

  setupWifi();
  client.setServer(SERVER, PORT);
  client.setCallback(handleMsg);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }

  client.loop();

  if (digitalRead(14) == HIGH) {
    dismissMessage();
  }

  if (prOpen) {
    alertPr();
  }

  delay(100);
}

