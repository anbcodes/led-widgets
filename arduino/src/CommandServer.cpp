// #include "CommandServer.hpp"

// WiFiServer CommandServer::server = WiFiServer(12540);

// WiFiClient CommandServer::currentClient = NULL;

// LedRequest CommandServer::getRequest()
// {
//   currentClient = server.available();
//   static StaticJsonDocument<MAX_REQUEST_JSON_SIZE> doc;
//   doc.clear();

//   if (currentClient && currentClient.connected() && currentClient.available())
//   {
//     // I'm expecting a HTTP request here but the deserialize json skips
//     // the extra data and just parses the first JSON it finds, so it ignores the HTTP parts
//     deserializeJson(doc, currentClient);

//     LedRequest req = {id : doc["id"], name : {}, data : {}};

//     Logger::printf("Name: %s\n", doc["name"].as<char *>());
//     Logger::printf("Data: %s\n", doc["data"].as<char *>());

//     strncpy(req.name, doc["name"].as<char *>(), REQ_NAME_LEN);

//     strncpy(req.data, doc["data"].as<char *>(), REQ_DATA_LEN);

//     return req;
//   }

//   LedRequest req = {id : doc["id"], name : {}, data : {}};
//   return req;
// }

// void CommandServer::begin()
// {
//   server.begin();
// }

// void CommandServer::send(char *json)
// {
//   if (currentClient.connected())
//   {
//     int len = strlen(json);

//     currentClient.println("HTTP/1.1 200 OK");
//     currentClient.print("Content-Length: ");
//     currentClient.println(len);
//     currentClient.println("Connection: Closed");
//     currentClient.println();
//     currentClient.println(json);
//     currentClient.println();
//   }
// }

// void CommandServer::send(JsonVariant json)
// {
//   if (currentClient.connected())
//   {
//     char buf[MAX_REQUEST_JSON_SIZE];
//     serializeJson(json, buf);

//     int len = strlen(buf);

//     currentClient.println("HTTP/1.1 200 OK");
//     currentClient.print("Content-Length: ");
//     currentClient.println(len);
//     currentClient.println("Connection: Closed");
//     currentClient.println();
//     currentClient.println(buf);
//     currentClient.println();
//   }
// }

// char *CommandServer::getBody(WiFiClient client)
// {
// }