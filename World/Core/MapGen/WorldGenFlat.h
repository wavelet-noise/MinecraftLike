// ============================================================================
// ==         Copyright (c) 2016, Samsonov Andrey and Smirnov Denis          ==
// ==                  See license.txt for more information                  ==
// ============================================================================
#pragma once
#ifndef WorldGenFlat_h__
#define WorldGenFlat_h__


#include "..\Block.h"
#include "..\..\tools\CoordSystem.h"
#include "..\..\tools\StringIntern.h"


class WorldGenFlat
{
public:
  WorldGenFlat();

  PBlock Create(const WBPos &pos);

private:
  StringIntern mGrassName;

};



#endif // WorldGenFlat_h__