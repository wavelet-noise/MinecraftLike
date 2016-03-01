#pragma once

#include <vector>
#include <tools\StringIntern.h>
#include <rapidjson\document.h>

class RecipeIn
{
public:
  StringIntern id;
  int count;

  virtual void JsonLoad(const rapidjson::Value &val);
};

class RecipeOut
{
public:
  StringIntern id;
  int count;
  float chance;

  virtual void JsonLoad(const rapidjson::Value &val);
};

class Recipe
{
public:
  std::vector<RecipeIn> input;
  std::vector<RecipeOut> output;
  std::vector<StringIntern> tools;
  StringIntern machine;
  float duration;

  virtual void JsonLoad(const rapidjson::Value &val);
};