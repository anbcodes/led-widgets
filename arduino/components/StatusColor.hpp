#include <FastLED.h>
#include "../lib/Component.hpp"

class StatusColor: public Component {
    private:
     CRGB color = CRGB(0, 0, 0);
};
