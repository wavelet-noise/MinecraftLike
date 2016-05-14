



#include "World.h"
#include <tuple>
#include <utility>
#include <GLFW\glfw3.h>
#include "..\tools\CoordSystem.h"
#include "MapGen\WorldWorker.h"
#include "..\Render\Render.h"
#include "Tessellator.h"
#include <Core\Chest.h>
#include <Core\EventBus.h>
#include <memory>

World::World()
{
}


World::~World()
{
}

void World::Update(float dt)
{
	//mEventBus.Update(this);

	for (auto &i : mSectors)
	{
		i.second->Update(this, dt);
	}

	auto a = DB::Get().Create("allagents");
	for (auto &c : a->mAgents)
	{
		c.second->__AfterUpdate();
	}

	static float ro_delay = 0;
	ro_delay += dt;
	if (ro_delay >= 30)
	{
		ro_delay = 0;
		recipe_orders.insert(recipe_orders.begin(), delayed_recipe_orders.begin(), delayed_recipe_orders.end());
		delayed_recipe_orders.clear();
	}
}



std::shared_ptr<Sector> World::GetSector(const SPos &position)
{
	auto it = mSectors.find(position);
	if (it == mSectors.end())
	{
		if (auto psec = WorldWorker::Get().GetSector(position))
		{
			mSectors[position] = psec;
			psec->Draw(mTesselator);
			EventBus::Get().Publish<EventSectorReady>(psec);

			// Добавлен новый сектор, сообщим соседним секторам, что б перестроились.
			/*SPos offset[] =
			{
			  {  1,  0,  0 },
			  { -1,  0,  0 },
			  {  0,  1,  0 },
			  {  0, -1,  0 },
			  {  0,  0,  1 },
			  {  0,  0, -1 },
			};
			for (const auto &i : offset)
			{
			  if (auto &sector = FindSector(position + i))
			  {
				sector->SayChanged();
			  }
			}*/
			return psec;
		}

		return nullptr;
	}

	return it->second;
}

std::shared_ptr<Sector> World::FindSector(const SPos &position)
{
	static SPos last{ 99999 };
	static std::shared_ptr<Sector> plast;

	if (last == position)
		return plast;

	auto it = mSectors.find(position);
	if (it != mSectors.end())
	{
		plast = it->second;
		return it->second;
	}

	return nullptr;
}

bool World::IsWalkable(const WBPos &wbpos)
{
	auto spos = cs::WBtoS(wbpos);
	if (auto sector = FindSector(spos))
	{
		if (auto b = sector->GetBlock(cs::WBtoSB(wbpos, spos)))
			return false;
	}

	spos = cs::WBtoS(wbpos - glm::ivec3(0, 0, 1));
	if (auto sector = FindSector(spos))
	{
		if (auto b = sector->GetBlock(cs::WBtoSB(wbpos - glm::ivec3(0, 0, 1), spos)))
			return true;
	}

	return false;
}

PGameObject World::GetBlock(const WBPos &wbpos)
{
	auto spos = cs::WBtoS(wbpos);
	if (auto sector = FindSector(spos))
	{
		return sector->GetBlock(cs::WBtoSB(wbpos, spos));
	}

	return nullptr;
}

PGameObject World::GetBlock(const WBPos &wbpos, std::shared_ptr<Sector> &contains)
{
	auto spos = cs::WBtoS(wbpos);
	if (auto sector = FindSector(spos))
	{
		contains = sector;
		return sector->GetBlock(cs::WBtoSB(wbpos, spos));
	}

	contains = nullptr;
	return nullptr;
}


static float sdt = 0;
static World *sworld = nullptr;
static glm::ivec3 neib[] = {
	{  1,  0,  0 },
	{  0,  1,  0 },
	{  0,  0,  1 },
	{ -1,  0,  0 },
	{  0, -1,  0 },
	{  0,  0, -1 }
};

PGameObject World::SetBlock(const WBPos &wbpos, PGameObject block)
{
	auto spos = cs::WBtoS(wbpos);
	std::shared_ptr<Sector> sec;
	static StringIntern storage("storage");

	if (auto l = GetBlock(wbpos, sec))
	{
		l->OnDestroy({ this, sec.get(), wbpos, 0 });

		if (l->GetId() == storage)
		{
			storages.remove(std::make_pair(wbpos, block));
		}
	}

	if (auto sector = FindSector(spos))
	{
		sector->SetBlock(cs::WBtoSB(wbpos, spos), block);
		if (block) // may be nullptr on place air
		{
			block->OnCreate({ this, sec.get(), wbpos, 0 });

			if (block->GetId() == storage)
			{
				storages.push_back(std::make_pair(wbpos, block));
			}
		}
	}

	for (const auto &n : neib)
	{
		if (auto l = GetBlock(wbpos + n, sec))
			l->OnAdjacentChanged({ this, sec.get(), wbpos + n, 0 });
	}

	return block;
}

PGameObject World::Spawn(const WBPos & position, PGameObject creature)
{
	if (auto &s = GetSector(cs::WBtoS(position)))
	{
		s->Spawn(position, creature);
		creature->OnCreate({ this, s.get(), position, 0 });
	}
	else
		LOG(error) << "Spawning " << creature->GetId() << " in not existing sector!";

	return creature;
}

PGameObject World::Place(const SBPos & position, PGameObject item)
{
	if (auto &s = GetSector(cs::WBtoS(position)))
	{
		s->Place(position, item);
		item->OnCreate({ this, s.get(), position, 0 });
	}
	else
		LOG(error) << "Placing " << item->GetId() << " in not existing sector!";

	return item;
}

PGameObject World::Replace(const SBPos & position, PGameObject item)
{
	if (auto &s = GetSector(cs::WBtoS(position)))
	{
		s->Repace(position, item);
	}
	else
		LOG(error) << "Replacing " << item->GetId() << " in not existing sector!";

	return item;
}

std::list<std::pair<glm::vec3, PGameObject>>& World::GetStorages()
{
	return storages;
}

std::list<World::RecipeOrder>& World::GetRecipeOrders()
{
	return recipe_orders;
}

std::list<World::RecipeOrder>& World::GetDelayedRecipeOrders()
{
	return delayed_recipe_orders;
}

void World::QueueRecipeOrder(const RecipeOrder & ro)
{
	for (auto &r : recipe_orders)
	{
		if (*r.recipe == *ro.recipe)
		{
			r.elapsed += ro.elapsed;
			if (!r.infinite)
				r.infinite = ro.infinite;
			return;
		}
	}

	recipe_orders.push_back(ro);
}

void World::DelayRecipeOrder(const PRecipe & ro)
{
	for (auto &r : recipe_orders)
	{
		if (*r.recipe == *ro)
		{
			delayed_recipe_orders.push_back(r);
			recipe_orders.remove(r);
		}
	}
}

void World::DoneRecipeOrder(const PRecipe & ro, int count)
{
	for (auto &r : recipe_orders)
	{
		if (*r.recipe == *ro)
		{
			r.elapsed -= count;
			if (r.elapsed <= 0 && !r.infinite)
				recipe_orders.remove(r);
		}
	}
}

int World::GetActiveCount()
{
	return mSectors.size();
}

void World::SetTessellator(Tessellator *tess)
{
	mTesselator = tess;
}

Tessellator * World::GetTessellator()
{
	return mTesselator;
}

void World::SetSlise(int s)
{

	slise = s;
	for (auto &se : mSectors)
	{
		se.second->SetSlise(s);
	}

}
