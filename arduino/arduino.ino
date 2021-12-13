// clang-format off
#include <FastLED.h>
#include <SPI.h>
#include <WiFiNINA.h>
#include <ArduinoOTA.h>
#include <NTPClient.h>
// clang-format on

#include "src/components/ProgressBar.hpp"
#include "src/components/StatusColor.hpp"
#include "src/consts.hpp"
#include "src/secrets.hpp"
#include "src/util.hpp"

#ifndef PRINT
#define PRINT(v) Serial.print(v)
// #define PRINT(v)
#endif

#ifndef PRINTLN
#define PRINTLN(v) Serial.println(v)
// #define PRINTLN(v)
#endif

CRGB leds[LED_COUNT];

WiFiUDP ntpUDP;

// By default 'pool.ntp.org' is used with 60 seconds update interval and
// no offset
NTPClient timeClient(ntpUDP);

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
  timeClient.begin();
  Logger::println("");
  Logger::println("====================================");
  Logger::println("====================================");
  Logger::println("====================================");
  Logger::println("               Setup!               ");
}

unsigned long long alarm = 1639283352 + 5000;
bool ledsOn = true;

enum CommandType {
  Unknown = 0,
  StatusColorSet = 1,
  ProgressBarAdd = 2,
  ProgressBarSet = 3,
  AlarmSet = 4,
  TurnOff = 5,
  TurnOn = 6,
};

StatusColor statusColor;
ProgressBar<10> progressBars;

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    NVIC_SystemReset();
  }

  for (int i = 0; i < LED_COUNT; i++) {
    leds[i] = CRGB(0, 0, 0);
  }

  timeClient.update();

  // Logger::printf("Time: %lu\n", timeClient.getEpochTime());

  ArduinoOTA.poll();

  parseCommand();

  if (ledsOn) {
    for (int i = 0; i < LED_COUNT; i++) {
      Color pcolor = progressBars.colorOf(i);
      if (pcolor.a) {
        leds[i] = pcolor.rgb;
      } else {
        leds[i] = statusColor.colorOf(i).rgb;
      }
    }
  }

  static int c = 0;

  // leds[c % 300] = CRGB(123, 232, 92);

  c++;

  if (timeClient.getEpochTime() > alarm && c % 150 < 75) {
    for (int i = 0; i < LED_COUNT / 5; i++) {
      leds[i * 5] = CRGB(255, 255, 255);
    }
  }

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

      uint32_t *p = (uint32_t *)&alarm;

      switch (type) {
        case StatusColorSet:
          statusColor.commandSet(data, client);
          break;
        case ProgressBarAdd:
          progressBars.commandAddBar(data, client);
          break;
        case ProgressBarSet:
          progressBars.commandSetBar(data, client);
          break;
        case AlarmSet:
          alarm = extract<uint64_t>(data);
          Logger::printf("Set alarm to %X%08X\n", p[1], p[0]);
          break;
        case TurnOff:
          ledsOn = false;
          Logger::print("Turned leds of\n");
          break;
        case TurnOn:
          ledsOn = true;
          Logger::print("Turned leds on\n");
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