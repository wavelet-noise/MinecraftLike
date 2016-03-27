#pragma once

#include <vector>
#include <tools\StringIntern.h>
#include <rapidjson\document.h>
#include <list>

class RecipeIn
{
public:
  StringIntern id;
  int count = 1;

  // <"id", [count]>
  virtual void JsonLoad(const rapidjson::Value &val);
};

class RecipeOut
{
public:
  StringIntern id;
  int count = 1;
  float chance = 1.0;

  // <"id", [count[, chance]]>
  virtual void JsonLoad(const rapidjson::Value &val);
};

class Recipe;

using PRecipe = std::shared_ptr<Recipe>;
using PCRecipe = std::shared_ptr<const Recipe>;

class Recipe
{
public:
  std::vector<RecipeIn> input;
  std::vector<RecipeOut> output;
  std::vector<StringIntern> tools;
  StringIntern machine;
  std::vector<StringIntern> materials;
  float duration = 0;

  void DrawSome(const StringIntern & s);

  void DrawGui();

  std::list<std::shared_ptr<Recipe>> Expand();

  virtual void JsonLoad(const rapidjson::Value &val);
};