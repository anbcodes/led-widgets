#include "StatusColor.hpp"

Color StatusColor::colorOf(int x) {
  return x == LED_COUNT - 1 ? color : Color();
}

void StatusColor::commandSet(uint8_t data[MAX_DATA_SIZE], WiFiClient client) {
  int r = data[0];
  int g = data[1];
  int b = data[2];
  int a = data[3];
  Logger::printf("Got %d, %d, %d, %d\n", r, g, b, a);

  color = Color(r, g, b, a);
};

void StatusColor::update(){};