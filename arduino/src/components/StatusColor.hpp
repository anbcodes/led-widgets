#pragma once

#include <FastLED.h>
#include <WiFiNINA.h>

#include "../Color.hpp"
#include "../Logger.hpp"
#include "../consts.hpp"

class StatusColor {
 private:
  Color color = Color(0, 30, 0);

 public:
  void update();
  void commandSet(uint8_t req[MAX_DATA_SIZE], WiFiClient client);
  Color colorOf(int x);
};
