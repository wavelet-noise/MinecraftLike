#pragma once
#include "Agent.h"
#include "GameObject.h"
#include <vector>

class ChestSlot
{
public:
  PGameObject obj;
  int count;
};

class Chest : public Agent
{
public:
  Chest();

  void JsonLoad(const rapidjson::Value &val) override;

  // Унаследовано через Agent
  virtual PAgent Clone(GameObject * parent, const std::string & name = "") override;
  virtual void Update(const GameObjectParams & params) override;
  bool Push(PGameObject go, int count = 1, int pos = -1);

  int columns = 10;
  int size = 40;
  std::vector<ChestSlot> slots;
};

REGISTER_AGENT(Chest)