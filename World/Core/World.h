



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

  struct RecipeOrder
  {
	  RecipeOrder(PRecipe r, int e = 1, bool i = false) : recipe(r), elapsed(e), infinite(i) {}

	  PRecipe recipe;
	  int elapsed = 1;
	  bool infinite = false;
	  float delayed = 0.f;

	  bool operator == (const RecipeOrder &other) const
	  {
		  return recipe == other.recipe && elapsed == other.elapsed && infinite == other.infinite;
	  }
  };

  // Получить сектор. Если сектор не загружен, будет произведена попытка 
  // загрузить сектор из загрузчика секторов.
  std::shared_ptr<Sector> GetSector(const SPos &position);

  PGameObject GetBlock(const WBPos &position);

  PGameObject GetBlock(const WBPos & wbpos, std::shared_ptr<Sector> &contains);

  bool IsWalkable(const WBPos & wbpos);

  // Установить блок в заданную позицию.
  // Существующий блок будет удален.
  PGameObject SetBlock(const WBPos &pos, PGameObject block);

  PGameObject Spawn(const WBPos &position, PGameObject creature);

  PGameObject Place(const SBPos & position, PGameObject item);
  PGameObject Replace(const SBPos & position, PGameObject item);


  std::list<std::pair<glm::vec3, PGameObject>> &GetStorages();
  std::list<RecipeOrder> &GetRecipeOrders();
  std::list<RecipeOrder> &GetDelayedRecipeOrders();
  void QueueRecipeOrder(const RecipeOrder &ro);
  void DelayRecipeOrder(const PRecipe &ro);
  void DoneRecipeOrder(const PRecipe &ro, int count = 1);

  int GetActiveCount();

  void SetTessellator(Tessellator *tess);
  Tessellator * GetTessellator();

  void SetSlise(int s);
  

  inline int GetSlise() const 
  {
	  return slise;
  }

private:
  std::unordered_map<SPos, std::shared_ptr<Sector>> mSectors;
  std::list<std::pair<glm::vec3, PGameObject>> storages;

  std::list<RecipeOrder> recipe_orders;
  std::list<RecipeOrder> delayed_recipe_orders;

  Sector *mCurrentSector;

  int slise = 0;

private:
  // Найти сектор по позиции сектора.
  std::shared_ptr<Sector> FindSector(const SPos &position);

  Tessellator *mTesselator;
};

#endif // World_h__