#pragma once

#include <FastLED.h>
#include <WiFiNINA.h>

#include "../ArduinoSort.hpp"
#include "../Color.hpp"
#include "../Logger.hpp"
#include "../consts.hpp"
#include "../util.hpp"

struct Bar {
  Color color = Color();
  int len = 0;
  int offset = 0;
  int start = 0;
  int end = 0;
  int endTime = 0;

  Bar(Color color, int len, int offset, int start, int end, int endTime) {
    this->color = color;
    this->len = len;
    this->offset = offset;
    this->start = start;
    this->end = end;
    this->endTime = endTime;
  }

  Bar() {}
};

template <int BarsCount>
class ProgressBar {
 private:
  Bar bars[BarsCount];

  int end = 250;

  int animatingBarIndex = -1;
  int animatingBarTarget = 0;
  int animatingBarEnd = 0;

  static bool compare(Bar* a, Bar* b) {
    int int_a = a->offset;
    int int_b = b->offset;

    if (int_a == int_b) {
      return false;
    } else if (int_a < int_b) {
      return false;
    } else {
      return true;
    }
  }

 public:
  void update() {
    // Find the farthest right
    // Shift it over till it reaches end or the prev

    for (int i = 0; i < BarsCount; i++) {
      if (bars[i].offset < bars[i].end) {
        unsigned long ms = millis();
        bars[i].offset =
            cubicInOut(1 - (float(bars[i].endTime - ms) / 3000.0)) *
                float(bars[i].end - bars[i].start) +
            bars[i].start;

        if (bars[i].offset >= bars[i].end - 1.1) {
          bars[i].offset = bars[i].end;
        }
      }
    }
  };
  void commandAddBar(uint8_t data[MAX_DATA_SIZE], WiFiClient client) {
    uint16_t length = extract<uint16_t>(data);
    // uint16_t offset = extract<uint16_t>(data + 2);
    // uint8_t i = data[4];
    // uint8_t r = data[5];
    // uint8_t g = data[6];
    // uint8_t b = data[7];
    // uint8_t a = data[8];
    uint8_t i = data[2];
    uint8_t r = data[3];
    uint8_t g = data[4];
    uint8_t b = data[5];
    uint8_t a = data[6];

    Logger::printf("Adding bar len: %d, ind: %d, color: %d, %d, %d, %d\n",
                   length, i, r, g, b, a);

    if (i < BarsCount) {
      setBar(0, i, 0, 0, 0, 0);
      int off = end;
      for (int i = 0; i < BarsCount; i++) {
        off -= bars[i].len;
      }
      off -= length;
      bars[i] = Bar(Color(r, g, b, a), length, 0, 0, off, millis() + 1000 * 3);

      // animatingBarIndex = i;
      // animatingBarTarget = offset;
      // animatingBarEnd = millis() + 3000;
      // Logger::printf("Animating to %d, done at %d\n", animatingBarTarget,
      //               animatingBarEnd);
    }
  };

  void commandClear() {
    for (int i = 0; i < BarsCount; i++) {
      bars[i] = Bar();
    }
  }

  void commandSetBar(uint8_t data[MAX_DATA_SIZE], WiFiClient client) {
    uint16_t length = extract<uint16_t>(data);
    // uint16_t offset = extract<uint16_t>(data + 2);
    // uint8_t i = data[4];
    // uint8_t r = data[5];
    // uint8_t g = data[6];
    // uint8_t b = data[7];
    // uint8_t a = data[8];
    uint8_t i = data[2];
    uint8_t r = data[3];
    uint8_t g = data[4];
    uint8_t b = data[5];
    uint8_t a = data[6];
    setBar(length, i, r, g, b, a);
  };

  void setBar(int length, int i, int r, int g, int b, int a) {
    Logger::printf("Setting bar len: %d, ind: %d, color: %d, %d, %d, %d\n",
                   length, i, r, g, b, a);

    if (i < BarsCount) {
      int plen = bars[i].len;
      bars[i] = Bar(Color(r, g, b, a), length, bars[i].offset, bars[i].start,
                    bars[i].end, bars[i].endTime);
      if (bars[i].len != plen) {
        int dif = bars[i].len - plen;
        for (int j = 0; j < BarsCount; j++) {
          if (bars[j].end - dif > 0 && bars[j].end - dif < LED_COUNT &&
              bars[j].len != 0 && j != i && bars[j].offset < bars[i].offset) {
            bars[j].start = bars[j].end;
            bars[j].end = bars[j].end - dif;
            bars[j].endTime = millis() + 1000 * 3;
            if (dif > 0) {
              bars[j].start = 0;
              bars[j].offset = bars[j].end;
              bars[j].endTime = millis();
            }
          }
        }
        bars[i].offset -= dif;
        bars[i].end -= dif;
        bars[i].start = 0;
        bars[i].endTime = millis();
      }
    }
  }

  Color colorOf(int x) {
    for (int i = 0; i < BarsCount; i++) {
      if (x > bars[i].offset && x <= bars[i].offset + bars[i].len) {
        return bars[i].color;
      }
    }
    return Color();
  }
};
