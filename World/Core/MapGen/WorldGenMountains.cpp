#include "WorldGenMountains.h"
#include "Core\MapGen\PerlinNoise.h"

#define GEN_OCT 5
float flatness(float tx, float ty)
{
  return (PerlinNoise2D(tx, ty, 2, 2, GEN_OCT) + 1) / 2.f;
}

float dens(float tx, float ty, float tz)
{
  float flat = 1 / 5.f;// flatness(tx / 1000.f, ty / 1000.f);
  if (tz < -SECTOR_SIZE)
    return 1;
  else if (tz < 0)
    return PerlinNoise3D(tx / 100.f, ty / 100.f, tz / 100.f, 1 + 5 * flat, 2, GEN_OCT) + ((-tz*(SECTOR_SIZE / 1000.f)));
  else
    return PerlinNoise3D(tx / 100.f, ty / 100.f, tz / 100.f, 1 + 5 * flat, 2, GEN_OCT) / ((tz + float(SECTOR_SIZE)) / float(SECTOR_SIZE));
}

bool solid(float tx, float ty, float tz)
{
  return dens(tx, ty, tz) > 0.1;
}
#undef GEN_OCT

void WorldGenMountains::Generate(Sector & s)
{
  const size_t size = static_cast<size_t>(SECTOR_SIZE);
  auto spos = s.GetPos();

  auto bg = DB::Get().Create(StringIntern("water"));
  auto bd4 = DB::Get().Create(StringIntern("dirt4"));
  auto bd3 = DB::Get().Create(StringIntern("dirt3"));
  auto bd2 = DB::Get().Create(StringIntern("dirt2"));
  auto bd = DB::Get().Create(StringIntern("dirt"));

  if (spos.x != 0)
    for (int i = 0; i < SECTOR_SIZE; ++i)
    {
      for (int j = 0; j < SECTOR_SIZE; ++j)
      {
        for (int k = 0; k < SECTOR_SIZE; ++k)
        {
          float tx = static_cast<float>(i + spos.x*SECTOR_SIZE);
          float ty = static_cast<float>(j + spos.y*SECTOR_SIZE);
          float tz = static_cast<float>(k + spos.z*SECTOR_SIZE);

          if (solid(tx, ty, tz))
          {
            if (solid(tx, ty, tz + 15))
              s.SetBlock({ i, j, k }, bd4);
            else if (solid(tx, ty, tz + 10))
              s.SetBlock({ i, j, k }, bd3);
            else if (solid(tx, ty, tz + 5))
              s.SetBlock({ i, j, k }, bd2);
            else if (!solid(tx, ty, tz + 1))
              s.SetBlock({ i, j, k }, bg);
            else
              s.SetBlock({ i, j, k }, bd);
          }
        }
      }
    }
}
