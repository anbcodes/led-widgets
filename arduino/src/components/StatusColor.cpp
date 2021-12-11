#include "StatusColor.hpp"

Color StatusColor::colorOf(int x)
{
  return x == LED_COUNT - 1 ? color : Color();
}

void StatusColor::parseCommand(LedRequest req)
{
  StaticJsonDocument<256> doc;

  deserializeJson(doc, req.data);

  if (!strcmp(req.name, "statusLED"))
  {

    int r = doc["r"];
    int g = doc["g"];
    int b = doc["b"];
    int a = doc["a"];
    Logger::printf("Got %d, %d, %d, %d\n", r, g, b, a);

    color = Color(r, g, b, a);

    CommandServer::send("{\"type\":\"success!\"}");
  }
};

void StatusColor::update(){};