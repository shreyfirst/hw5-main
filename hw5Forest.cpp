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
  HttpServer httpserver(8386);
  Myhw5Server s(httpserver, JSONRPC_SERVER_V1V2, "forest");
  std::cout << "Hit enter to stop the server" << endl;

  while (s.all_items_recieved == false)
  {
    s.StartListening();
  }
  s.StopListening();

  // start sending the items to grandma's house
  Person wolf;
  wolf.setName("wolf");
  wolf.setDescription("monster");
  s.Person_Map["wolf"] = &wolf;

  // send objects to next server (grandma's house)
  HttpClient httpclient("http://localhost:8387");
  hw5Client myClient(httpclient, JSONRPC_CLIENT_V2);
  Json::Value finish, person_move, thing_move;
  string send_to_forest[4] = {"girl", "cake", "wine", "cap"};

  Json::Value location_validation;
  location_validation["server_name"] = "grandma's home";

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
            (location_validation),
            person.first);
        // s.Person_Map.erase(person.first);
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
        // s.Thing_Map.erase(thing.first);
      }
      catch (JsonRpcException &e)
      {
        cerr << "Thing move error: " << e.what() << endl;
      }
    }
  }
  // s.output();
  finish = myClient.finished();

  return 0;
}