#pragma once
#include <Core\Sector.h>
class GenStrategy
{
public:

  virtual void Generate(Sector &s) = 0;
};

