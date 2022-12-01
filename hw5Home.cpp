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
#include <jsonrpccpp/server/connectors/httpserver.h>

#include <stdio.h>
#include "ecs36b_Exception.h"
#include "hw5client.h"

#include "Person.h"
#include "Thing.h"

using namespace jsonrpc;
using namespace std;

using namespace jsonrpc;
using namespace std;

int main()
{
  // starting server
  HttpServer httpserver(8385);
  Myhw5Server s(httpserver, JSONRPC_SERVER_V1V2, "home");
  std::cout << "Hit enter to stop the server" << endl;

  // set initial Person and Thing objects
  Person girl;
  girl.setDescription("a dear little girl");
  s.Person_Map["girl"] = &girl;

  Person mother;
  mother.setName("mother");
  s.Person_Map["mother"] = &mother;

  Thing cake, wine;
  cake.setName("Piece of Cake");
  s.Thing_Map["cake"] = &cake;
  wine.setName("Bottle of Wine");
  s.Thing_Map["wine"] = &wine;

  Thing cap;
  cap.setName("cap");
  cap.setDescription("little, red velvet");
  s.Thing_Map["cap"] = &cap;

  HttpClient httpclient("http://127.0.0.1:8386");
  hw5Client myClient(httpclient, JSONRPC_CLIENT_V2);
  Json::Value finish, person_move, thing_move;
  string send_to_forest[4] = {"girl", "cake", "wine", "cap"};

  Json::Value location_validation;
  location_validation["server_name"] = "forest";

  // sending girl Person object
  for (auto const &person : s.Person_Map)
  {

    bool person_send_to_forest = std::find(send_to_forest, send_to_forest + (sizeof(send_to_forest) / sizeof(*send_to_forest)), person.first) != send_to_forest + (sizeof(send_to_forest) / sizeof(*send_to_forest));
    if (person_send_to_forest)
    {
      try
      {
        std::cout << " - sending Person called " << person.first << " to " << location_validation["server_name"] << std::endl;

        person_move = myClient.move(
            "move",
            "Person",
            (person.second->dump2JSON()),
            location_validation,
            person.first);
      }
      catch (JsonRpcException &e)
      {
        cerr << "Person move error: " << e.what() << endl;
      }
    }
  }

  // sending cake and wine Thing objects
  for (auto const &thing : s.Thing_Map)
  {
    bool thing_send_to_forest = std::find(send_to_forest, send_to_forest + (sizeof(send_to_forest) / sizeof(*send_to_forest)), thing.first) != send_to_forest + (sizeof(send_to_forest) / sizeof(*send_to_forest));
    if (thing_send_to_forest)
    {
      try
      {
        std::cout << " - sending Thing called " << thing.first << " to " << location_validation["server_name"] << std::endl;

        thing_move = myClient.move(
            "move",
            "Thing",
            (thing.second->dump2JSON()),
            location_validation,
            thing.first);
      }
      catch (JsonRpcException &e)
      {
        cerr << "Thing move error: " << e.what() << endl;
      }
    }
  }
   finish = myClient.finished();

  return 0;
}