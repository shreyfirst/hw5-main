
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
// #include "hw5client.h"
#include <jsonrpccpp/client/connectors/httpclient.h>
#include <stdio.h>

#include "Person.h"
#include "Thing.h"

using namespace jsonrpc;
using namespace std;

class Myhw5Server : public hw5Server
{
public:
  Myhw5Server(AbstractServerConnector &connector, serverVersion_t type, std::string server_name);
  virtual Json::Value move(const std::string &action,
                           const std::string &class_id,
                           const Json::Value &json_object,
                           const Json::Value &location,
                           const std::string &object_id);
  virtual Json::Value finished();
  virtual Json::Value output();
  std::map<std::string, Person *> Person_Map;
  std::map<std::string, Thing *> Thing_Map;
  bool all_items_recieved = false;
  std::string server_name = "";
};

Myhw5Server::Myhw5Server(AbstractServerConnector &connector, serverVersion_t type, std::string server_name_p)
    : hw5Server(connector, type)
{
  server_name = server_name_p;
  std::cout << "starting server " << server_name << std::endl;
}

Json::Value
Myhw5Server::move(const std::string &action, const std::string &class_id,
                  const Json::Value &json_object, const Json::Value &location,
                  const std::string &object_id)
{
  int error_code = 0;
  Json::Value result;
  std::string strJson;

  // validation
    if (class_id != "Person" && class_id != "Thing")
    {
      result["status"] = "failed";
      strJson = ( "class " + class_id + " unknown");
      result["reason"] = strJson;
      error_code = -1;
    }

  if (object_id.empty())
    {
      result["status"] = "failed";
      strJson = ( "object_id null ");
      result["reason"] = strJson;
      error_code = -2;
    }

    if ((location.isNull() == true) ||
      (location.isObject() == false))
    {
      result["status"] = "failed";
      strJson = ( "location parsing error " );
      result["reason"] = strJson;
      error_code = -3;
    }

  if ((json_object.isNull() == true) ||
      (json_object.isObject() == false))
    {
      result["status"] = "failed";
      strJson = ( "person parsing error " );
      result["reason"] = strJson;
      error_code = -4;
    }

  
  if (location["server_name"] != this->server_name)
    {
      result["status"] = "failed";
      strJson = ( "you are sending data to the incorrect server (ip mismatch)" );
      result["reason"] = strJson;
      error_code = -9;
    }
  
  if (result["status"] == "failed"){
    std::cout << "failed: " << result["reason"] << std::endl;
    return result;
  }


  if (class_id == "Person")
  {

    Person *lv_person_ptr;
    if (Person_Map.find(object_id) != Person_Map.end())
    {
      lv_person_ptr = Person_Map[object_id];
    }
    else
    {
      lv_person_ptr = new Person{};
      Person_Map[object_id] = lv_person_ptr;
    }
    lv_person_ptr->JSON2Object(json_object);
    result["status"] = "successful";
    result["data"] = Person_Map[object_id]->dump2JSON();
  }
  else if (class_id == "Thing")
  {

    Thing *lv_thing_ptr;
    if (Thing_Map.find(object_id) != Thing_Map.end())
    {
      lv_thing_ptr = Thing_Map[object_id];
    }
    else
    {
      lv_thing_ptr = new Thing{};
      Thing_Map[object_id] = lv_thing_ptr;
    }
    lv_thing_ptr->JSON2Object(json_object);
    result["status"] = "successful";
    result["data"] = Thing_Map[object_id]->dump2JSON();
  }

  // std::cout << action << " " << object_id << std::endl;
  // std::cout << result.toStyledString() << std::endl;
  std::cout << " - " << server_name << " recieved " << class_id << " called " << object_id << std::endl;
  return result;
}

Json::Value Myhw5Server::finished()
{

  Json::Value result;
  result["status"] = "successful";
  result["data"] = "all objects sent";
  all_items_recieved = true;
  std::cout << " * " << server_name << " finished recieving all objects" << std::endl;
  return result;
}


Json::Value Myhw5Server::output()
{

    // for (const auto& x : this->Person_Map) {
    //     std::cout << x.first << ": " << x.second->dump2JSON() << "\n";
    // }
    // for (const auto& x : this->Thing_Map) {
    //     std::cout << x.first << ": " << x.second->dump2JSON() << "\n";
    // }

}
