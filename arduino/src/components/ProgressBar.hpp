#pragma once

#include <FastLED.h>
#include <WiFiNINA.h>

#include "../Color.hpp"
#include "../Logger.hpp"
#include "../consts.hpp"
#include "../util.hpp"

struct Bar {
  Color color = Color();
  int len = 0;
  int offset = 0;

  Bar(Color color, int len, int offset) {
    this->color = color;
    this->len = len;
    this->offset = offset;
  }

  Bar() {}
};

template <int BarsCount>
class ProgressBar {
 private:
  Bar bars[BarsCount];

  int animatingBarIndex = -1;
  int animatingBarTarget = 0;
  int animatingBarEnd = 0;

 public:
  void update() {
    if (animatingBarIndex > -1) {
      unsigned long ms = millis();
      bars[animatingBarIndex].offset =
          cubicInOut(1 - (float(animatingBarEnd - ms) / 3000.0)) *
          float(animatingBarTarget);

      if (bars[animatingBarIndex].offset >= animatingBarTarget - 1.1) {
        bars[animatingBarIndex].offset = animatingBarTarget;
        animatingBarTarget = 0;
        animatingBarIndex = -1;
        animatingBarEnd = 0;
      }
    }
  };
  void commandAddBar(uint8_t data[MAX_DATA_SIZE], WiFiClient client) {
    uint16_t length = extract<uint16_t>(data);
    uint16_t offset = extract<uint16_t>(data + 2);
    uint8_t i = data[4];
    uint8_t r = data[5];
    uint8_t g = data[6];
    uint8_t b = data[7];
    uint8_t a = data[8];

    if (i < BarsCount) {
      bars[i] = Bar(Color(r, g, b, a), length, 0);

      animatingBarIndex = i;
      animatingBarTarget = offset;
      animatingBarEnd = millis() + 3000;
      Logger::printf("Animating to %d, done at %d\n", animatingBarTarget,
                     animatingBarEnd);
    }
  };

  Color colorOf(int x) {
    for (int i = 0; i < BarsCount; i++) {
      if (x > bars[i].offset && x < bars[i].offset + bars[i].len) {
        return bars[i].color;
      }
    }
    return Color();
  }
};
