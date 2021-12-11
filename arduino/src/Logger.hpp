#pragma once

#include <WiFiUdp.h>

#include "consts.hpp"

class Logger : public Stream {
  static WiFiUDP client;

 public:
  static void init() { client.begin(LOG_PORT); }

  static void print(const char *str) {
#ifdef DO_LOGGING
    client.beginPacket(LOG_IP, LOG_PORT);
    client.print(str);
    client.endPacket();
#endif
  }

  static void println(const char *str) {
#ifdef DO_LOGGING
    client.beginPacket(LOG_IP, LOG_PORT);
    client.println(str);
    client.endPacket();
#endif
  }

  template <typename... Args>
  static void printf(const char *str, Args... args) {
#ifdef DO_LOGGING
    client.beginPacket(LOG_IP, LOG_PORT);

    char buf[PRINTF_MAX_SIZE];

    snprintf(buf, PRINTF_MAX_SIZE, str, args...);
    client.print(buf);
    client.endPacket();
#endif
  }
};