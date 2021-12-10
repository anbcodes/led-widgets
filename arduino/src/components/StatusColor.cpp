#include "StatusColor.hpp"

CRGB StatusColor::colorOf(int x)
{
  return CRGB(50, 50, 50);
}

JsonObject StatusColor::parseCommand(JsonObject obj)
{
  StaticJsonDocument<1024> doc;

  doc["test"] = "error";

  return doc.to<JsonObject>();
};

void StatusColor::update(){};