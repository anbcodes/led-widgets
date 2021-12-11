#pragma once

struct LedRequest
{
  int id;
  char name[REQ_NAME_LEN];
  char data[REQ_DATA_LEN];
};
