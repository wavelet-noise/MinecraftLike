// ============================================================================
// ==         Copyright (c) 2016, Samsonov Andrey and Smirnov Denis          ==
// ==                  See license.txt for more information                  ==
// ============================================================================
#pragma once
#ifndef WorldGenFlat_h__
#define WorldGenFlat_h__


#include <core\GameObject.h>
#include <tools\CoordSystem.h>
#include <tools\StringIntern.h>
#include "GenStrategy.h"

class WorldGenFlat : public GenStrategy
{
public:
  WorldGenFlat();

private:
  StringIntern mGrassName;


  // Унаследовано через GenStrategy
  virtual void Generate(Sector & s) override;

};



#endif // WorldGenFlat_h__