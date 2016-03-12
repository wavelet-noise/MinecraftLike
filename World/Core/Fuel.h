#ifndef MATERIAL_H
#define MATERIAL_H
#include "Agent.h"

class Fuel : public Agent
{
public:
  Fuel();

  void JsonLoad(const rapidjson::Value &val) override;

  // Унаследовано через Agent
  virtual PAgent Clone(GameObject * parent, const std::string & name = "") override;
  virtual void Update(const GameObjectParams & params) override;
  inline float getHeat()
  {
    return mHeat;
  }

private:
  float mHeat;
};

REGISTER_AGENT(Fuel)

#endif // MATERIAL_H
