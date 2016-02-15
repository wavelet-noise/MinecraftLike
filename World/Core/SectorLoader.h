// ============================================================================
// ==                   Copyright (c) 2015, Smirnov Denis                    ==
// ==                  See license.txt for more information                  ==
// ============================================================================
#pragma once
#ifndef SectorLoader_h__
#define SectorLoader_h__

#include <glm/glm.hpp>
#include <vector>
#include "../tools/CoordSystem.h"
class World;

///???
class SectorLoader
{
public:
  SectorLoader(World &world, const SPos &pos = {}, size_t radius = 3);
  ~SectorLoader();

  /// Радиус зоны загрузки секторов.
  void SetRadius(size_t radius);

  /// Позиция сектора.
  void SetPos(const SPos &pos);

private:
  World &mWorld;

  glm::ivec3 mPos;

  size_t mRadius;

  std::vector<glm::ivec3> mSite;

private:
  SectorLoader(const SectorLoader &) = delete;
  SectorLoader& operator=(const SectorLoader &) = delete;

  void Reload();
};



#endif // SectorLoader_h__

