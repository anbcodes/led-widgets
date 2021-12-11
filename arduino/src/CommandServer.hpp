#pragma once

#include <WiFiNINA.h>
#include <ArduinoJson.h>

#include "consts.hpp"
#include "LedRequest.hpp"
#include "Logger.hpp"

template <int maxClients = 10, int maxBodySize = 1024, int resJSONSize = 1024>
class CommandServer
{
public:
  static WiFiClient currentClient;

  static LedRequest getRequest()
  {
    currentClient = server.available();
    static StaticJsonDocument<MAX_REQUEST_JSON_SIZE> doc;
    doc.clear();

    if (currentClient && currentClient.connected() && currentClient.available())
    {
      char body[maxBodySize];
      getBody(currentClient, body);
      // I'm expecting a HTTP request here but the deserialize json skips
      // the extra data and just parses the first JSON it finds, so it ignores the HTTP parts
      deserializeJson(doc, currentClient);

      LedRequest req = {id : doc["id"], name : {}, data : {}};

      Logger::printf("Name: %s\n", doc["name"].as<char *>());
      Logger::printf("Data: %s\n", doc["data"].as<char *>());

      strncpy(req.name, doc["name"].as<char *>(), REQ_NAME_LEN);

      strncpy(req.data, doc["data"].as<char *>(), REQ_DATA_LEN);

      return req;
    }

    LedRequest req = {id : doc["id"], name : {}, data : {}};
    return req;
  }
  static void send(char *json)
  {
    if (currentClient.connected())
    {
      int len = strlen(json);

      currentClient.println("HTTP/1.1 200 OK");
      currentClient.print("Content-Length: ");
      currentClient.println(len);
      currentClient.println("Connection: Closed");
      currentClient.println();
      currentClient.println(json);
      currentClient.println();
    }
  }
  static void send(JsonVariant json)
  {
    if (currentClient.connected())
    {
      char buf[resJSONSize];
      serializeJson(json, buf);

      int len = strlen(buf);

      currentClient.println("HTTP/1.1 200 OK");
      currentClient.print("Content-Length: ");
      currentClient.println(len);
      currentClient.println("Connection: Closed");
      currentClient.println();
      currentClient.println(buf);
      currentClient.println();
    }
  }
  static void begin()
  {
    server.begin();
  }

private:
  static WiFiServer server;
  void getBody(char out[maxBodyLen], WiFiClient client)
  {

    int contentLength = -1;
    while (client.available())
    {
    }
  }
};
