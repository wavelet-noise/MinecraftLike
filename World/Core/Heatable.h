#ifndef MATERIAL_H
#define MATERIAL_H
#include "Agent.h"

class Heatable : public Agent
{
public:
  Heatable();

  void JsonLoad(const rapidjson::Value &val) override;

  // Унаследовано через Agent
  virtual PAgent Clone(GameObject * parent, const std::string & name = "") override;
  virtual void Update(const GameObjectParams & params) override;

private:
  float T;
};

REGISTER_AGENT(Heatable)

#endif // MATERIAL_H