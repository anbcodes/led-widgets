// clang-format off
#include <FastLED.h>
#include <SPI.h>
#include <WiFiNINA.h>
#include <ArduinoOTA.h>
// clang-format on

#include <vector>

// #include "utility/wifi_drv.h"

#include "consts.hpp"
#include "secrets.hpp"

#ifndef PRINT
#define PRINT(v) Serial.print(v)
// #define PRINT(v)
#endif

#ifndef PRINTLN
#define PRINTLN(v) Serial.println(v)
// #define PRINTLN(v)
#endif

CRGB leds[LED_COUNT];

// void color(int r, int g, int b) {
//   WiFiDrv::digitalWrite(RED_LED_pin, r);
//   WiFiDrv::digitalWrite(GREEN_LED_pin, g);
//   WiFiDrv::digitalWrite(BLUE_LED_pin, b);
// }

void connect() {
  Serial.begin(9600);
  delay(3000);
  leds[0] = CRGB(255, 255, 255);
  FastLED.setBrightness(255);
  FastLED.show();

  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    leds[0] = CRGB(255, 0, 0);
    FastLED.show();
    while (1);
  }

  arduino::String fv = WiFi.firmwareVersion();

  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
    Serial.println(WIFI_FIRMWARE_LATEST_VERSION);
    leds[0] = CRGB(255, 0, 0);
    FastLED.show();
  }

  int attempts = 0;

  int status = 0;

  while (status != WL_CONNECTED) {
    digitalWrite(LED_BUILTIN, LOW);
    Serial.print("Attempting to connect to SSID: ");

    Serial.println(WIFI_SSID);

    status = WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    if (status == WL_CONNECTED) {
      break;
    }

    Serial.println(status);

    delay(4000);
    attempts++;
    if (attempts > 10) {
      NVIC_SystemReset();
    }
  }

  leds[0] = CRGB(255, 0, 255);
  FastLED.show();
}

void setup() {
  Serial.begin(9600);
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, LED_COUNT);
  pinMode(LED_BUILTIN, OUTPUT);
  connect();
  ArduinoOTA.begin(WiFi.localIP(), "LED", "LEDS1234", InternalStorage);
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    NVIC_SystemReset();
  }

  ArduinoOTA.poll();
  static int c = 0;
  c++;
  for (int i = 0; i < LED_COUNT; i++) {
    leds[i] = CRGB(0, 0, 0);
  }

  leds[0] = CRGB(0, 255, 0);
  leds[c % 300] = CRGB(0, 255, 255);
  leds[(c + 1) % 300] = CRGB(0, 255, 0);
  leds[(c + 2) % 300] = CRGB(0, 0, 255);
  Serial.println(c % 300);
  // leds[(millis() / 1000) % LED_COUNT] = CRGB(255, 0, 255);

  FastLED.show();
//  delay(100);
  // put your main code here, to run repeatedly:
  // digitalWrite(LED_BUILTIN, HIGH);
  // delay(1000);
  // digitalWrite(LED_BUILTIN, LOW);
  // delay(1000);
}
