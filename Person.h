#ifndef PERSON_H
#define PERSON_H
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

class Person
{
private:
    string name;
    string description;

public:
    Person();
    Person(string description);
    Person(string name, string description);
    string getName();
    void setName(string name);
    string getDescription();
    void setDescription(string description);
    virtual Json::Value dump2JSON();
    virtual bool JSON2Object(Json::Value input);
};

#endif