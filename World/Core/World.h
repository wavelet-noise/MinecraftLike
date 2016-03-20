



#pragma once
#ifndef World_h__
#define World_h__

#include <unordered_map>
#include <glm/glm.hpp>
#include <memory>
#include <list>
#include "Sector.h"
#include "Player.h"
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

  // Установить блок в заданную позицию.
  // Существующий блок будет удален.
  void SetBlock(const WBPos &pos, PGameObject block);

  int GetActiveCount();

  Player *GetPlayer();

  void SetTessellator(Tessellator *tess);
  Tessellator * GetTessellator();

  void PushEvent(std::unique_ptr<GameEvent> event);

private:
  std::unordered_map<SPos, std::shared_ptr<Sector>> mSectors;

  Sector *mCurrentSector;

  std::unique_ptr<Player> mPlayer;

private:
  // Найти сектор по позиции сектора.
  std::shared_ptr<Sector> FindSector(const SPos &position);

  Tessellator *mTesselator;

  EventBus mEventBus;
};



#endif // World_h__