// ============================================================================
// ==         Copyright (c) 2016, Samsonov Andrey and Smirnov Denis          ==
// ==                  See license.txt for more information                  ==
// ============================================================================
#include "WorldGenFlat.h"
#include "..\DB.h"



WorldGenFlat::WorldGenFlat()
{
  mGrassName = StringIntern("grass");
}

PBlock WorldGenFlat::Create(const WBPos &pos)
{
  const auto sbpos = cs::WBtoSB(pos);
  const size_t size = SECTOR_SIZE;
  if (pos.z <= 10)
  {
    if (sbpos.x == 0 || sbpos.x == size - 1 ||
        sbpos.y == 0 || sbpos.y == size - 1 ||
        sbpos.z == 0 || sbpos.z == size - 1)
    {
      return nullptr;
    }
    return DB::Get().Create(mGrassName);
  }

  return nullptr;
}
