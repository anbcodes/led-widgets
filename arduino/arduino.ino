// clang-format off
#include <FastLED.h>
#include <SPI.h>
#include <WiFiNINA.h>
#include <ArduinoOTA.h>
#include <ArduinoJson.h>
// clang-format on

// #include <vector>

// #include "utility/wifi_drv.h"

#include "src/consts.hpp"
#include "src/secrets.hpp"

#include "src/components/Main.hpp"
// #include "src/CommandServer.hpp"
#include "src/LedRequest.hpp"

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

void connect()
{
  Serial.begin(9600);
  delay(3000);
  leds[0] = CRGB(255, 255, 255);
  FastLED.setBrightness(255);
  FastLED.show();

  if (WiFi.status() == WL_NO_MODULE)
  {
    Serial.println("Communication with WiFi module failed!");
    leds[0] = CRGB(255, 0, 0);
    FastLED.show();
    while (1)
      ;
  }

  arduino::String fv = WiFi.firmwareVersion();

  if (fv < WIFI_FIRMWARE_LATEST_VERSION)
  {
    Serial.println("Please upgrade the firmware");
    Serial.println(WIFI_FIRMWARE_LATEST_VERSION);
    leds[0] = CRGB(255, 0, 0);
    FastLED.show();
  }

  int attempts = 0;

  int status = 0;

  while (status != WL_CONNECTED)
  {
    digitalWrite(LED_BUILTIN, LOW);
    Serial.print("Attempting to connect to SSID: ");

    Serial.println(WIFI_SSID);

    status = WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    if (status == WL_CONNECTED)
    {
      break;
    }

    Serial.println(status);

    delay(4000);
    attempts++;
    if (attempts > 10)
    {
      NVIC_SystemReset();
    }
  }

  leds[0] = CRGB(255, 0, 255);
  FastLED.show();
}

Main mainComponent;

void setup()
{
  Serial.begin(9600);
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, LED_COUNT);
  pinMode(LED_BUILTIN, OUTPUT);
  connect();
  // CommandServer::begin();
  Logger::init();
  ArduinoOTA.begin(WiFi.localIP(), "LED", "LEDS1234", InternalStorage);
  Logger::println("");
  Logger::println("====================================");
  Logger::println("====================================");
  Logger::println("====================================");
  Logger::println("               Setup!               ");
}

void loop()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    NVIC_SystemReset();
  }

  ArduinoOTA.poll();

  LedRequest req = CommandServer::getRequest();

  // Logger::printf("Got Name: %s, Id: %d, Data: %s\n", req.name, req.id, req.data);

  if (req.name != NULL && req.data != NULL && strlen(req.name) > 0)
  {
    Logger::printf("Got Name: %s, Id: %d, Data: %s\n", req.name, req.id, req.data);
    mainComponent.parseCommand(req);
  }

  for (int i = 0; i < LED_COUNT; i++)
  {
    leds[i] = mainComponent.colorOf(i);
  }

  static int c = 0;

  leds[c % 300] = CRGB(123, 232, 92);

  c++;

  FastLED.show();

  mainComponent.update();
}
