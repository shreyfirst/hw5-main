#ifndef THING_H
#define THING_H
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <json/json.h>
#include <json/reader.h>
#include <json/writer.h>
#include <json/value.h>
using std::string;

#pragma once

class Thing
{
private:
    string name;
    string description;

public:
    string getName();
    void setName(string name);
    string getDescription();
    void setDescription(string description);

    Thing();
    Thing(string name, string description);
    virtual Json::Value dump2JSON();
    virtual bool JSON2Object(Json::Value input);
};

#endif