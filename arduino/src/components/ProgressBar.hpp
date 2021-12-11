#pragma once

#include <ArduinoJson.h>
#include <FastLED.h>

#include "../consts.hpp"
#include "../LedRequest.hpp"
#include "../Logger.hpp"
#include "../CommandServer.hpp"
#include "../Color.hpp"

struct Bar
{
  Color color = Color();
  int len = 0;
  int offset = 0;
};

template <int Bars>
class ProgressBar
{
private:
  Bar bars[Bars];

  int animatingBarIndex = -1;
  int animatingBarTarget = 0;

public:
  void update()
  {
    if (animatingBarIndex > -1)
    {
      bars[animatingBarIndex].offset += 1;
      if (bars[animatingBarIndex].offset == animatingBarTarget)
      {
        animatingBarTarget = 0;
        animatingBarIndex = -1;
      }
    }
  };
  void parseCommand(LedRequest req)
  {
    StaticJsonDocument<256> doc;

    deserializeJson(doc, req.data);

    if (!strcmp(req.name, "progressBar-addBar"))
    {
      int r = doc["color"]["r"];
      int g = doc["color"]["g"];
      int b = doc["color"]["b"];
      int a = doc["color"]["a"];
      int ind = doc["index"];
      int off = doc["offset"];
      int len = doc["len"];
      Logger::printf("Add bar Color: %d, %d, %d, %d Index: %d, Offset: %d, Length: %d\n", r, g, b, a, ind, off, len);

      if (ind > Bars)
      {
        CommandServer::send("{\"message\":\"failed!\"}");
        return;
      }
      bars[ind].color = Color(r, g, b, a);
      bars[ind].len = len;
      bars[ind].offset = off;

      CommandServer::send("{\"message\":\"success!\"}");
    }

    if (!strcmp(req.name, "progressBar-removeBar"))
    {
      int i = doc["i"];
      Logger::printf("Remove Bar %d\n", i);

      if (i > Bars)
      {
        CommandServer::send("{\"message\":\"failed!\"}");
        return;
      }
      bars[i].color = Color();
      bars[i].len = 0;
      bars[i].offset = 0;

      CommandServer::send("{\"message\":\"success!\"}");
    }
  };
  Color colorOf(int x)
  {
    if (x == 150)
    {
      return bars[0].color;
    }
    else
    {
      return Color();
    }
  }
};
