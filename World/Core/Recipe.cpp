#include "Recipe.h"
#include <Serealize.h>

void RecipeIn::JsonLoad(const rapidjson::Value & val)
{
  id = val.Begin()->GetString();
  count = val[1].GetInt();
}

void RecipeOut::JsonLoad(const rapidjson::Value & val)
{
  id = val.Begin()->GetString();
  count = val[1].GetInt();
  chance = val[2].GetDouble();
}

void Recipe::JsonLoad(const rapidjson::Value & val)
{
  JSONLOAD(NVP(input), NVP(output), NVP(tools), NVP(machine), NVP(duration));
}
