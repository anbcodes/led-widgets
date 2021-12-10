#pragma once

#include <ArduinoJson.h>
#include <FastLED.h>

#include "StatusColor.hpp"

class Main
{
private:
  StatusColor statusColor = StatusColor();

public:
  void update();
  JsonObject parseCommand(JsonObject json);
  CRGB colorOf(int x);
};
