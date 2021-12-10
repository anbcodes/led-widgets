#include "Main.hpp"
#include "StatusColor.hpp"

CRGB Main::colorOf(int x)
{
  return statusColor.colorOf(x);
};

JsonObject Main::parseCommand(JsonObject obj)
{
  return statusColor.parseCommand(obj);
};

void Main::update()
{
  return statusColor.update();
};