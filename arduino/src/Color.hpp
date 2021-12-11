#pragma once

#include <FastLED.h>

struct Color {
  CRGB rgb;
  uint8_t a;

  Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    rgb = CRGB(r, g, b);
    this->a = a;
  }

  Color(uint8_t r, uint8_t g, uint8_t b) {
    rgb = CRGB(r, g, b);
    a = 255;
  }

  Color() {
    rgb = CRGB(0, 0, 0);
    a = 0;
  }

  Color(CRGB rgb, uint8_t a) {
    this->rgb = rgb;
    this->a = a;
  }

  Color(CRGB rgb) {
    this->rgb = rgb;
    this->a = 255;
  }
};
