#pragma once
#include "GenStrategy.h"
class WorldGenMountains : public GenStrategy
{
public:

  // Унаследовано через GenStrategy
  virtual void Generate(Sector & s) override;
};

