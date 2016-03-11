#pragma once
#include "Agent.h"
#include "GameObject.h"
#include <vector>
#include "ChestSlot.h"

class Furnance : public Agent
{
public:
  Furnance();

  void JsonLoad(const rapidjson::Value &val) override;

  // Унаследовано через Agent
  virtual PAgent Clone(GameObject * parent, const std::string & name = "") override;
  virtual void Update(const GameObjectParams & params) override;
  virtual void DrawGui(const InteractParams &params) override;

  float T;
  ChestSlot fuel;
};

REGISTER_AGENT(Furnance)