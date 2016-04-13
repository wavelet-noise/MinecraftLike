



#pragma once
#ifndef World_h__
#define World_h__

#include <unordered_map>
#include <glm/glm.hpp>
#include <memory>
#include <list>
#include "Sector.h"

#include "../Render/Render.h"
#include "EventBus.h"


class Tessellator;

class World
{
public:
  World();
  ~World();

  void Update(float dt);

  // Получить сектор. Если сектор не загружен, будет произведена попытка 
  // загрузить сектор из загрузчика секторов.
  std::shared_ptr<Sector> GetSector(const SPos &position);

  PGameObject GetBlock(const WBPos &position);

  PGameObject GetBlock(const WBPos & wbpos, std::shared_ptr<Sector> &contains);

  // Установить блок в заданную позицию.
  // Существующий блок будет удален.
  PGameObject SetBlock(const WBPos &pos, PGameObject block);

  PGameObject Spawn(const WBPos &position, PGameObject creature);

  PGameObject Place(const SBPos & position, PGameObject item);
  PGameObject Replace(const SBPos & position, PGameObject item);

  int GetActiveCount();

  void SetTessellator(Tessellator *tess);
  Tessellator * GetTessellator();

private:
  std::unordered_map<SPos, std::shared_ptr<Sector>> mSectors;

  Sector *mCurrentSector;

private:
  // Найти сектор по позиции сектора.
  std::shared_ptr<Sector> FindSector(const SPos &position);

  Tessellator *mTesselator;
};

#endif // World_h__