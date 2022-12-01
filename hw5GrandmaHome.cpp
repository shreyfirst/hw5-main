#include "hw5server.cpp"

// for Json::value
#include <json/json.h>
#include <json/reader.h>
#include <json/writer.h>
#include <json/value.h>
#include <string>

// for JsonRPCCPP
#include <iostream>
#include "hw5server.h"

#include <jsonrpccpp/client/connectors/httpclient.h>
#include <stdio.h>

#include "ecs36b_Exception.h"
#include "hw5client.h"

using namespace jsonrpc;
using namespace std;

int main()
{

  // first recieve all items from home
  HttpServer httpserver(8387);
  Myhw5Server s(httpserver, JSONRPC_SERVER_V1V2, "grandma's home");
  std::cout << "Hit enter to stop the server" << endl;

  while (s.all_items_recieved == false)
  {
    s.StartListening();
  }
  s.StopListening();
  // s.output();


  return 0;
}