#include <ArduinoJson.h>

class Component {
    public:
     virtual int colorOf(int x) = 0;
     virtual int update() = 0;
     virtual arduino::String parseCommand(JsonObject json) = 0;
};
