#include "Main.hpp"
#include "StatusColor.hpp"

CRGB Main::colorOf(int x)
{
  Color status = statusColor.colorOf(x);
  Color progress = progressBar.colorOf(x);

  if (progress.a)
  {
    return progress.rgb;
  }

  if (status.a)
  {
    return status.rgb;
  }
};

void Main::parseCommand(LedRequest req)
{
  statusColor.parseCommand(req);
  progressBar.parseCommand(req);
};

void Main::update()
{
  return statusColor.update();
};