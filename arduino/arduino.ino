// clang-format off
#include <FastLED.h>
#include <SPI.h>
#include <WiFiNINA.h>
#include <ArduinoOTA.h>
// clang-format on

// #include <vector>

// #include "utility/wifi_drv.h"

#include "src/components/ProgressBar.hpp"
#include "src/components/StatusColor.hpp"
#include "src/consts.hpp"
#include "src/secrets.hpp"

#ifndef PRINT
#define PRINT(v) Serial.print(v)
// #define PRINT(v)
#endif

#ifndef PRINTLN
#define PRINTLN(v) Serial.println(v)
// #define PRINTLN(v)
#endif

CRGB leds[LED_COUNT];

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
    while (1)
      ;
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
WiFiServer server(12540);

void setup() {
  Serial.begin(9600);
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, LED_COUNT);
  pinMode(LED_BUILTIN, OUTPUT);
  connect();
  // CommandServer::begin();
  Logger::init();
  ArduinoOTA.begin(WiFi.localIP(), "LED", "LEDS1234", InternalStorage);
  server.begin();
  Logger::println("");
  Logger::println("====================================");
  Logger::println("====================================");
  Logger::println("====================================");
  Logger::println("               Setup!               ");
}

enum CommandType {
  Unknown = 0,
  StatusColorSet = 1,
  ProgressBarAdd = 2,
  ProgressBarRemove = 3,
  ProgressBarModify = 4,
};

StatusColor statusColor;
ProgressBar<10> progressBars;

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    NVIC_SystemReset();
  }

  ArduinoOTA.poll();

  parseCommand();

  for (int i = 0; i < LED_COUNT; i++) {
    Color pcolor = progressBars.colorOf(i);
    if (pcolor.a) {
      leds[i] = pcolor.rgb;
    } else {
      leds[i] = statusColor.colorOf(i).rgb;
    }
  }

  static int c = 0;

  leds[c % 300] = CRGB(123, 232, 92);

  c++;

  FastLED.show();

  statusColor.update();
  progressBars.update();
}

// Format
// [type (word little endian)][len (dword little endian)][data]

void parseCommand() {
  WiFiClient client = server.available();
  uint8_t data[MAX_DATA_SIZE];

  int type = Unknown;
  int dataLength = 0;

  int readBytes = 0;
  if (client && client.connected()) {
    Logger::println("Reading...");
    if (client.available()) {
      int read = client.read();
      if (read == -1) goto error;
      type = (uint8_t)read;

      read = client.read();
      if (read == -1) goto error;
      type += (uint8_t)read << 8;

      if (type == Unknown) goto error;

      read = client.read();
      if (read == -1) goto error;
      dataLength = (uint8_t)read;

      read = client.read();
      if (read == -1) goto error;
      dataLength += (uint8_t)read << 8;

      read = client.read();
      if (read == -1) goto error;
      dataLength += (uint8_t)read << 16;

      read = client.read();
      if (read == -1) goto error;
      dataLength += (uint8_t)read << 24;

      Logger::printf("Received %d, %d\n", type, dataLength);

      if (dataLength >= MAX_DATA_SIZE - 1) goto error;

      for (int i = 0; i < dataLength; i++) {
        read = client.read();
        if (read == -1) goto error;
        data[i] = (uint8_t)read;
      }

      data[dataLength] = '\0';

      switch (type) {
        case StatusColorSet:
          statusColor.commandSet(data, client);
          break;
        case ProgressBarAdd:
          progressBars.commandAddBar(data, client);
          break;
        default:
          break;
      }

      Logger::printf("Recieved Data %d, %d, %d, %d\n", data[0], data[1],
                     data[2], data[3]);
      client.write("success\n");
    }
  }
  goto afterError;
error:
  client.write("error\n");
afterError:
  return;
}