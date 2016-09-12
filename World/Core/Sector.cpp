#include "Sector.h"
#include <core\PositionAgent.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <stdint.h>
#include "../tools/Log.h"
#include "../Render/Render.h"
#include "World.h"
#include "Tessellator.h"
#include "SectorBase.h"
#include <tuple>
#include <Render\ParticleSystem.h>


Sector::Sector(const SPos &position)
	: mPos(position)
{
}

Sector::Sector()
{
}

Sector::~Sector()
{
}

const SPos & Sector::GetPos() const
{
	return mPos;
}

void Sector::SetBlock(const SBPos &pos, PGameObject block)
{
	SectorBase<PGameObject>::SetBlock(pos, block);
	//bool active = block->IsActive();

	//if (active)
	//{
	//	auto finded = std::find_if(mActive.begin(), mActive.end(), [&](const ActiveStruct &as)->bool {return std::get<SBPos>(as) == pos; });
	//	if (finded == mActive.end())
	//		mActive.push_back(std::make_tuple(block, pos));
	//}

	if (mTessellator)
	{
		if (block)
		{
			mTessellator->Set(cs::SBtoWB(pos, mPos), DB::Get().CreateTesselator(block->GetId()));
			mTessellator->SayChanged(mPos);
		}
		else
		{
			mTessellator->Set(cs::SBtoWB(pos, mPos), nullptr);
			mTessellator->SayChanged(mPos);
			mTessellator->SayChanged(mPos - SPos(0, 0, 1));
		}
	}
}

//void Sector::Afterload()
//{
	//for(
//}

void Sector::Spawn(const SBPos & position, PGameObject creature)
{
	creatures.push_back(creature);
	EventBus::Get().Publish<EventCreatureSpawn>(creature);

	if (auto p = creature->GetAgent<PositionAgent>())
		p->Set(position);
	else
		LOG(error) << "Spawning " << creature->GetId() << ", that is not creature!";
}

void Sector::Place(const SBPos & position, PGameObject item)
{
	items.push_back(std::make_tuple(item, position));
	EventBus::Get().Publish<EventItemPlace>(item);
}

void Sector::Repace(const SBPos & position, PGameObject creature)
{
	items.remove(std::make_tuple(creature, position));
}

std::list<PGameObject> Sector::GetCreatures()
{
	return creatures;
}

void Sector::SayChanged()
{
	if (mTessellator)
		mTessellator->SayChanged(mPos);
}

void Sector::Update(World *world, float dt)
{
	std::remove_if(mActive.begin(), mActive.end(), [&](const ActiveStruct &go)->bool {return std::get<0>(go).expired(); });

	GameObjectParams gop{ world, this, {}, dt };
	for (size_t i = 0; i < mUniqueBlocks.size(); ++i)
	{
		gop.pos = cs::SBtoWB(cs::ItoSB(mUniquePoses[i]), mPos);
		if (mUniqueBlocks[i]) mUniqueBlocks[i]->Update(gop);
	}


	for (auto c_iter = creatures.begin(); c_iter != creatures.end(); ++c_iter)
	{
		(*c_iter)->Update(gop);
	}

	for (auto c_iter = creatures.begin(); c_iter != creatures.end();)
	{
		PGameObject c = *c_iter;
		auto p = c->GetAgent<PositionAgent>();

		ParticleSystem::Get().Add(p->Get() + glm::vec3(0, 0, 0.5), StringIntern("car"), dt);

		// если находится за границей сектора -- переносим в новый сектор, если он существует
		if (p->Get().x > (mPos.x + 1) * SECTOR_SIZE ||
			p->Get().x < (mPos.x) * SECTOR_SIZE ||
			p->Get().y >(mPos.y + 1) * SECTOR_SIZE ||
			p->Get().y < (mPos.y) * SECTOR_SIZE)
		{
			auto &nsec = world->GetSector(cs::WtoS(p->Get()));
			if (nsec)
			{
				creatures.erase(c_iter);
				nsec->creatures.push_back(std::move(c));
				break;
			}
		}
		//передвигаем итератор, если существо не перемещено
		++c_iter;
	}

	for (auto c_iter = items.begin(); c_iter != items.end(); ++c_iter)
	{
		std::get<0>(*c_iter)->Update(gop);

		ParticleSystem::Get().Add(std::get<1>(*c_iter) + glm::vec3(0, 0, 0.5), StringIntern("r"), dt);
	}
}



void Sector::Draw(class Tessellator *tess)
{
	mTessellator = tess;
	if (mTessellator /*&& !mBlocks.empty()*/)
	{
		auto currentTime = glfwGetTime();
		std::vector<std::tuple<size_t, StringIntern>> blocks;
		blocks.reserve(mCountBlocks);
		for (size_t i = 0; i < mBlocks.size(); ++i)
		{
			if (mBlocks[i])
			{
				blocks.emplace_back(i, GetBlock(i)->GetId());
			}
		}
		mTessellator->Set(mPos, std::move(blocks));
		mTessellator->SayChanged(mPos);
		//LOG(trace) << "SectorOnDraw: " << glfwGetTime() - currentTime;
	}
}

void Sector::SetSlise(int s)
{
	if (mTessellator)
		mTessellator->SetSlise(s);
}

void Sector::BinSave(std::ostream& val) const
{
	BINSAVE(mPos, mBlocks, mUniquePoses, mCountBlocks);
	BINSAVE(mUniqueBlocks.size());
	for (int i = 1; i < mUniqueBlocks.size(); ++i)
	{
		BINSAVE(mUniqueBlocks[i]->GetId());
	}
}

void Sector::BinLoad(std::istream& val)
{
	size_t size;
	mUniquePoses.clear(); mBlocks.clear(); mUniqueBlocks.clear();

	BINLOAD(mPos, mBlocks, mUniquePoses, mCountBlocks, size);
	mUniqueBlocks.reserve(size);
	mUniqueBlocks.push_back(nullptr);
	for (int i = 1; i < size; ++i)
	{
		StringIntern id;
		BINLOAD(id);
		mUniqueBlocks.push_back(DB::Get().Create(id));
	}

	SayChanged();
}
