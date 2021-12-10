#pragma once

#include <ArduinoJson.h>
#include <FastLED.h>

class StatusColor
{
private:
    CRGB color = CRGB(0, 0, 0);

public:
    void update();
    JsonObject parseCommand(JsonObject json);
    CRGB colorOf(int x);
};
