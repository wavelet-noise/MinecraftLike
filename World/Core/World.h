



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
#include <boost\serialization\serialization.hpp>

class Tessellator;

enum class RoomType
{
	UNSETTED,
	PERSONAL_ROOM,
	FARMLAND,
	ROOMTYPECOUNT
};

using PRoom = std::shared_ptr<struct Room>;

struct Room
{
	std::vector<glm::ivec3> cells;
	std::string name = "no name";
	glm::vec3 min;
	glm::vec3 max;

	RoomType type = RoomType::UNSETTED;

	friend class boost::serialization::access;

	template<class Archive>
	void serialize(Archive &ar, const unsigned int)
	{
		ar & cells;
		ar & name;
		ar & type;
	}

	static std::string TypeName(RoomType rt);

	void BinLoad(std::istream& val);
	void BinSave(std::ostream& val) const;

	void Update(float dt);
};

class World : public std::enable_shared_from_this<World>
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

	bool World::IsAir(const WBPos &wbpos);

	bool IsWalkable(const WBPos & wbpos);

	// Установить блок в заданную позицию.
	// Существующий блок будет удален.
	PGameObject SetBlock(const WBPos &pos, PGameObject block, bool no_replace = false);

	PGameObject Spawn(const WBPos &position, PGameObject creature);

	PGameObject Place(const SBPos & position, PGameObject item);
	PGameObject Replace(const SBPos & position, PGameObject item);


	std::list<std::pair<glm::vec3, PGameObject>> &GetStorages();
	std::list<RecipeOrder> &GetRecipeOrders();
	std::list<RecipeOrder> &GetDelayedRecipeOrders();
	void QueueRecipeOrder(const RecipeOrder &ro);
	void DelayRecipeOrder(const PRecipe &ro);
	void DoneRecipeOrder(const PRecipe &ro, int count = 1);

	void RegisterCreature(PGameObject go);

	std::list<PGameObject> GetCreaturesAt(const glm::ivec3 &cell);

	int GetActiveCount();

	void SetTessellator(Tessellator *tess);
	Tessellator * GetTessellator();

	void SetSlise(int s);

	void BinLoadRooms(std::istream& val);
	void BinSaveRooms(std::ostream& val) const;

	inline int GetSlise() const
	{
		return slise;
	}

	std::list<std::shared_ptr<Room>> rooms;
	std::list<PGameObject> controlled;


private:
	std::unordered_map<SPos, std::shared_ptr<Sector>> mSectors;
	std::list<std::pair<glm::vec3, PGameObject>> storages;

	std::list<RecipeOrder> recipe_orders;
	std::list<RecipeOrder> delayed_recipe_orders;

	std::map<size_t, PGameObject> creatures;

	Sector *mCurrentSector;

	int slise = 0;

private:
	// Найти сектор по позиции сектора.
	std::shared_ptr<Sector> FindSector(const SPos &position);

	Tessellator *mTesselator;
};

#endif // World_h__