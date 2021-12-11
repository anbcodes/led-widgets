#pragma once

#include <ArduinoJson.h>
#include <FastLED.h>

#include "StatusColor.hpp"
#include "../consts.hpp"
#include "../LedRequest.hpp"
#include "ProgressBar.hpp"

class Main
{
private:
  StatusColor statusColor = StatusColor();
  ProgressBar<10> progressBar = ProgressBar<10>();

public:
  void update();
  void parseCommand(LedRequest json);
  CRGB colorOf(int x);
};
