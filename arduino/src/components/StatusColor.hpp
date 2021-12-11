#pragma once

#include <ArduinoJson.h>
#include <FastLED.h>

#include "../consts.hpp"
#include "../LedRequest.hpp"
#include "../Logger.hpp"
#include "../CommandServer.hpp"
#include "../Color.hpp"

class StatusColor
{
private:
    Color color = Color(0, 30, 0);

public:
    void update();
    void parseCommand(LedRequest req);
    Color colorOf(int x);
};
