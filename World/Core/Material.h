#ifndef MATERIAL_H
#define MATERIAL_H
#include "Agent.h"

class Material : public Agent
{

public:
  // Унаследовано через Agent
  virtual PAgent Clone(GameObject * parent, const std::string & name = "") override;
  virtual void Update(const GameObjectParams & params) override;
  void JsonLoad(const rapidjson::Value &val) override;
};

REGISTER_AGENT(Material)

#endif // MATERIAL_H
