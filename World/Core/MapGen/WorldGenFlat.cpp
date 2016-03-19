



#include "WorldGenFlat.h"
#include "..\DB.h"



WorldGenFlat::WorldGenFlat()
{
}

void WorldGenFlat::Generate(Sector & s)
{
  const size_t size = static_cast<size_t>(SECTOR_SIZE);
  static auto gr = DB::Get().Create("grass");

  for (size_t i = 1; i < size; ++i)
  {
    for (size_t j = 1; j < size; ++j)
    {
      for (size_t k = 1; k < size; ++k)
      {
        if (k + s.GetPos().z*size <= 10)
          s.SetBlock({ i,j,k }, gr);
      }
    }
  }
}
