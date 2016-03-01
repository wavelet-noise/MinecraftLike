#include "Chest.h"
#include <Serealize.h>

Chest::Chest()
  : Agent(nullptr, "Chest", "")
{
  slots.resize(size);
}

void Chest::JsonLoad(const rapidjson::Value &val)
{

}

PAgent Chest::Clone(GameObject * parent, const std::string &)
{
  return PAgent();
}

void Chest::Update(const GameObjectParams & params)
{
}

bool Chest::Push(PGameObject go, int count, int pos)
{
  if (pos == -1)
  {
    for (auto &i : slots)
    {
      if (!i.obj)
      {
        i.obj = go;
        i.count = count;
        return true;
      }
    }
  }

  return false;
}

